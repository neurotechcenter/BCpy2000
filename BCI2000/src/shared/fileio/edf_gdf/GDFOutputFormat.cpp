////////////////////////////////////////////////////////////////////////////////
// $Id: GDFOutputFormat.cpp 4981 2015-08-26 17:04:38Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Output into a GDF data file.
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "GDFOutputFormat.h"

#include "GDF.h"
#include "BCIStream.h"
#include <limits>
#include <cstdlib>

using namespace std;
using namespace GDF;

GDFOutputFormat::GDFOutputFormat()
{
}


GDFOutputFormat::~GDFOutputFormat()
{
}


void
GDFOutputFormat::Publish() const
{
  EDFOutputBase::Publish();

  BEGIN_PARAMETER_DEFINITIONS
    "Storage string InvestigationID= % % % % "
      "// an identifier for the current investigation",
    "Storage matrix EventCodes= 6 "
      "[Condition                                 GDF%20Event] "
      "TargetCode!=0                              0x0300 " // trial begin
      "TargetCode==1                              0x030c " // cue up
      "TargetCode==2                              0x0306 " // cue down
      "(ResultCode!=0)&&(TargetCode==ResultCode)  0x0381 " // hit
      "(ResultCode!=0)&&(TargetCode!=ResultCode)  0x0382 " // miss
      "Feedback!=0                                0x030d " // feedback onset
      "// GDF events entered into the event table when associated conditions become true",
  END_PARAMETER_DEFINITIONS
}


void
GDFOutputFormat::Preflight( const SignalProperties& inProperties,
                            const StateVector& inStatevector ) const
{
  EDFOutputBase::Preflight( inProperties, inStatevector );
  switch( inProperties.Type() )
  {
    case SignalType::int16:
    case SignalType::int32:
    case SignalType::float24:
    case SignalType::float32:
      // These are OK
      break;
    default:
      bcierr << inProperties.Type().Name()
             << " data type unsupported for GDF files"
             << endl;
  }
  GenericSignal preflightSignal( inProperties );
  for( int i = 0; i < Parameter( "EventCodes" )->NumRows(); ++i )
    Expression( Parameter( "EventCodes" )( i, "Expression" ) )
      .SetOptionalAccess()
      .Evaluate( &preflightSignal );

  Parameter( "SubjectName" );
  Parameter( "SubjectYearOfBirth" );
  Parameter( "EquipmentID" );
  Parameter( "InvestigationID" );
}


void
GDFOutputFormat::Initialize( const SignalProperties& inProperties,
                             const StateVector& inStatevector )
{
  EDFOutputBase::Initialize( inProperties, inStatevector );

  mEvents.clear();
  mEventConditions.clear();
  mEventCodes.clear();
  mPreviousConditionValues.clear();

  GenericSignal initSignal( inProperties );
  for( int i = 0; i < Parameter( "EventCodes" )->NumRows(); ++i )
  {
    mEventConditions.push_back( Expression( Parameter( "EventCodes" )( i, "Condition" ) ).SetOptionalAccess() );
    mEventCodes.push_back( Parameter( "EventCodes" )( i, "GDF Event" ) );
    mPreviousConditionValues.push_back( mEventConditions.back().Evaluate( &initSignal ) );
  }
}


void
GDFOutputFormat::StartRun( ostream& os, const string& inFileName )
{
  EDFOutputBase::StartRun( os, inFileName );

  time_t now;
  struct tm* time;
  AcquireTime( now, time );

  ostringstream patient;
  patient << GDF::EncodedString( Parameter( "SubjectName" ) );

  ostringstream recording;
  recording << GDF::EncodedString( Parameter( "InvestigationID" ) )
            << ' ';
  size_t pos = inFileName.find_last_of( ":/\\" );
  recording << inFileName.substr( pos == string::npos ? 0 : pos + 1 );

  GDF::uint64::ValueType numEquipmentID = 0;
  istringstream iss;
  iss.str( Parameter( "EquipmentID" ) );
  iss >> numEquipmentID;
  if( numEquipmentID == 0 )
  {
    iss.clear();
    char* p = reinterpret_cast<char*>( &numEquipmentID );
    size_t i = 0;
    while( i < sizeof( numEquipmentID ) && iss.peek() != EOF )
      p[ i++ ] = iss.get();
  }

  ostringstream bci2000info;
  bci2000info << *Parameters << ends;
  size_t bci2000infoLength = bci2000info.str().length();
  if( bci2000infoLength >= 1 << 24 )
    bciout << "Losing BCI2000 header information due to GDF tag length limitation"
           << endl;
  size_t varHeaderLength = Channels().size(),
         tlvHeaderLength = ( 4 + bci2000infoLength + 256 - 1 ) / 256,
         tlvPaddingLength = tlvHeaderLength * 256 - 4 - bci2000infoLength;

  const float cNaN = numeric_limits<float>::quiet_NaN();

  PutField< Str<8>              >( os, "GDF 2.10" );
  PutField< Str<66>             >( os, patient.str() );
  PutField< Num<GDF::uint8,10>  >( os, 0 );
  PutField< Num<GDF::uint8>     >( os, 0 );
  PutField< Num<GDF::uint8>     >( os, 0 );
  PutField< Num<GDF::uint8>     >( os, 0 );
  PutField< Num<GDF::uint8>     >( os, int( Parameter( "SubjectSex" ) ) );
  PutField< Str<80>             >( os, recording.str() );
  PutField< Num<GDF::uint64>    >( os, GDF::DateTimeToGDFTime( now ) );
  PutField< Num<GDF::uint64>    >( os, GDF::YearToGDFTime( Parameter( "SubjectYearOfBirth" ) ) );
  PutField< Num<GDF::uint16>    >( os, 1 + varHeaderLength + tlvHeaderLength );
  PutField< Num<GDF::uint8,6>   >( os, 0 );
  PutField< Num<GDF::uint64>    >( os, numEquipmentID );
  PutField< Num<GDF::uint8,6>   >( os, 0 );
  PutField< Num<GDF::uint16,3>  >( os, 0 );
  PutField< Num<GDF::float32,3> >( os, cNaN );
  PutField< Num<GDF::float32,3> >( os, cNaN );
  PutField< Num<GDF::int64>     >( os, -1 );
  PutField< Num<GDF::uint32>    >( os, int( Parameter( "SampleBlockSize" ) ) );
  PutField< Num<GDF::uint32>    >( os, int( Parameter( "SamplingRate" ).InHertz() ) );
  PutField< Num<GDF::uint16>    >( os, Channels().size() );
  PutField< Num<GDF::uint16>    >( os, 0 );

  PutArray< Str<16>             >( os, Channels(), &ChannelInfo::Label );
  PutArray< Str<80>             >( os, Channels(), &ChannelInfo::TransducerType );
  PutArray< Str<6>              >( os, Channels(), &ChannelInfo::PhysicalDimension );
  PutArray< Num<GDF::uint16>    >( os, Channels(), &ChannelInfo::PhysicalDimensionCode );
  PutArray< Num<GDF::float64>   >( os, Channels(), &ChannelInfo::PhysicalMinimum );
  PutArray< Num<GDF::float64>   >( os, Channels(), &ChannelInfo::PhysicalMaximum );
  PutArray< Num<GDF::float64>   >( os, Channels(), &ChannelInfo::DigitalMinimum );
  PutArray< Num<GDF::float64>   >( os, Channels(), &ChannelInfo::DigitalMaximum );
  PutArray< Str<68>             >( os, Channels(), &ChannelInfo::Filtering );
  PutArray< Num<GDF::float32>   >( os, Channels(), &ChannelInfo::LowPass );
  PutArray< Num<GDF::float32>   >( os, Channels(), &ChannelInfo::HighPass );
  PutArray< Num<GDF::float32>   >( os, Channels(), &ChannelInfo::Notch );
  PutArray< Num<GDF::uint32>    >( os, Channels(), &ChannelInfo::SamplesPerRecord );
  PutArray< Num<GDF::uint32>    >( os, Channels(), &ChannelInfo::DataType );
  PutArray< Num<GDF::float32,3> >( os, Channels(), &ChannelInfo::ElectrodePosition );
  PutArray< Num<GDF::uint8>     >( os, Channels(), &ChannelInfo::ElectrodeImpedance );
  PutArray< Num<GDF::uint8,19>  >( os, Channels() );

  PutField< Num<GDF::uint8>     >( os, int( GDF::BCI2000Tag ) );
  PutField< Num<GDF::uint8>     >( os, bci2000infoLength & 0xff );
  PutField< Num<GDF::uint8>     >( os, bci2000infoLength >> 8 & 0xff );
  PutField< Num<GDF::uint8>     >( os, bci2000infoLength >> 16 & 0xff );
  os.write( bci2000info.str().data(), bci2000info.str().length() );
  for( size_t i = 0; i < tlvPaddingLength; ++i )
    os.put( 0 );
}


void
GDFOutputFormat::StopRun( ostream& os )
{
  // Event Table
  if( !mEvents.empty() && os.seekp( 0, ios_base::end ) )
  {
    PutField< Num<GDF::uint8>   >( os, 1 );
    PutField< Num<GDF::uint8>   >( os, mEvents.size() & 0xff );
    PutField< Num<GDF::uint8>   >( os, mEvents.size() >> 8 & 0xff );
    PutField< Num<GDF::uint8>   >( os, mEvents.size() >> 16 & 0xff );
    PutField< Num<GDF::float32> >( os, float( Parameter( "SamplingRate" ).InHertz() ) );
    PutArray< Num<GDF::uint32>  >( os, mEvents, &EventInfo::SamplePosition );
    PutArray< Num<GDF::uint16>  >( os, mEvents, &EventInfo::Code );
  }
  // Number of Records
  const int NumRecFieldPos = 236;
  if( os.seekp( NumRecFieldPos ) )
    PutField< Num<GDF::int64> >( os, NumRecords() );

  EDFOutputBase::StopRun( os );
}


void
GDFOutputFormat::Write( ostream& os,
                        const GenericSignal& inSignal,
                        const StateVector&   inStatevector )
{
  EDFOutputBase::Write( os, inSignal, inStatevector );

  int sampleBlockSize = inSignal.Elements();
  for( size_t i = 0; i < mEventConditions.size(); ++i )
  {
    bool curValue = mEventConditions[ i ].Evaluate( &inSignal );
    if( curValue != mPreviousConditionValues[ i ] )
    {
      EventInfo event;
      event.SamplePosition = NumRecords() * sampleBlockSize;
      event.Code = mEventCodes[ i ];
      if( !curValue )
        event.Code |= GDF::endOfEvent;
      mEvents.push_back( event );
    }
    mPreviousConditionValues[ i ] = curValue;
  }
}

