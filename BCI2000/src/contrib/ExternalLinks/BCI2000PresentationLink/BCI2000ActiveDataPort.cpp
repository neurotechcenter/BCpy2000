////////////////////////////////////////////////////////////////////////////////
// $Id: BCI2000ActiveDataPort.cpp 5293 2016-03-21 17:22:22Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An IActiveDataPort implementation for BCI2000PresentationLink.
//   The ActiveDataPort uses a BCI2000Remote object to communicate with
//   BCI2000.
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
#include "BCI2000ActiveDataPort.h"
#include "BCI2000DataPort.h"
#include "BCI2000DataPortExtension.h"
#include "Utils.h"
#include <algorithm>
#include <cstdio>

using namespace std;

CBCI2000ActiveDataPort* CBCI2000ActiveDataPort::spFirstInstance = NULL;

CBCI2000ActiveDataPort::CBCI2000ActiveDataPort()
: mUseResumePause( false )
{
  if( spFirstInstance == NULL )
    spFirstInstance = this;
}

CBCI2000ActiveDataPort::~CBCI2000ActiveDataPort()
{
  if( spFirstInstance == this )
    spFirstInstance = NULL;
}

STDMETHODIMP
CBCI2000ActiveDataPort::getName(BSTR* pOut)
{
  COM_METHOD_START
  *pOut = ::SysAllocString( CBCI2000DataPortExtension::Name() );
  COM_METHOD_END
}

STDMETHODIMP
CBCI2000ActiveDataPort::getServerTime( double* outTimeMicro, double* outUncertaintyMicro )
{
  COM_METHOD_START
  LARGE_INTEGER now, freq;
  ::QueryPerformanceCounter( &now );
  ::QueryPerformanceFrequency( &freq );
  *outTimeMicro = 1e-6 * now.QuadPart / freq.QuadPart;
  *outUncertaintyMicro = 2 * 1e-6 / freq.QuadPart;
  // We cause a time delay that reflects connection delay.
  // This way, Presentation's uncertainty estimate will be more accurate.
  mBCI2000.Execute( "", 0 );
  COM_METHOD_END
}

STDMETHODIMP
CBCI2000ActiveDataPort::sendData( BSTR inEventType, BSTR inEventCode, double, double, BSTR )
{
  COM_METHOD_START
  string eventType = com::DualString( inEventType );
  for( string::iterator i = eventType.begin(); i != eventType.end(); ++i )
    if( ::isspace( *i ) )
      *i = '_';

  bool success = true;
  if( mUseResumePause && !::stricmp( eventType.c_str(), "Resume" ) )
  {
    string eventCode = com::DualString( inEventCode );
    istringstream iss( eventCode );
    string subject, session, directory;
    if( iss >> subject )
      mBCI2000.SubjectID( subject );
    if( iss >> session )
      mBCI2000.SessionID( session );
    if( std::getline( iss >> ws, directory ) )
      mBCI2000.DataDirectory( directory );
    success = mBCI2000.Start();
  }
  else if( mUseResumePause && !::stricmp( eventType.c_str(), "Pause" ) )
  {
    success = mBCI2000.Stop();
  }
  else if( find( mEventTypes.begin(), mEventTypes.end(), eventType ) != mEventTypes.end() )
  {
    string eventCode = com::DualString( inEventCode );
    istringstream iss( eventCode );
    INT32 code;
    if( !( iss >> code ) ) // For non-numeric event codes, record up to four characters.
    {
      iss.clear();
      iss.seekg( 0 );
      code = 0;
      for( int i = 0; i < sizeof( code ) && iss.peek() != EOF; ++i )
      {
        code <<= 8;
        code += iss.get();
      }
    }
    ostringstream oss;
    oss << "pulse event " << eventType << " " << code;
    mBCI2000.Execute( oss.str(), 0 );
    success = mBCI2000.Result().empty();
  }
  if( !success )
    throw bciexception << "Error when processing event of type " << eventType << ": " << mBCI2000.Result();
  COM_METHOD_END
}

STDMETHODIMP
CBCI2000ActiveDataPort::processMessage(BSTR inMessage, BSTR* outResult)
{
  COM_METHOD_START
  mBCI2000.Execute( com::DualString( inMessage ), 0 );
  *outResult = ::SysAllocString( com::DualString( mBCI2000.Result() ).ToWin().c_str() );
  COM_METHOD_END
}

STDMETHODIMP
CBCI2000ActiveDataPort::getLastError(BSTR* outResult)
{
  *outResult = ::SysAllocString( mLastError.c_str() );
  return S_OK;
}

void
CBCI2000ActiveDataPort::Initialize( const DataPortSettings& inSettings )
{
  mBCI2000.WindowVisible( inSettings[DataPortSettings::HideOperator].empty() );
  mBCI2000.TelnetAddress( inSettings[DataPortSettings::TelnetAddress] );
  if( inSettings[DataPortSettings::StartBCI2000].empty() )
  {
    mBCI2000.OperatorPath( "" );
    if( !mBCI2000.Connect() )
      throw bciexception << mBCI2000.Result();
  }
  else if( inSettings[DataPortSettings::OperatorPath].empty() )
  {
    if( !com::Module::GetLocation() )
      throw bciexception << "Could not determine path to BCI2000PresentationLink";
    com::DualString path = com::Module::GetLocation();
    wchar_t* pOperator = NULL;
    if( ::LoadStringW( com::Module::GetHInstance(), IDS_OperatorName, reinterpret_cast<wchar_t*>( &pOperator ), 0 ) )
      path += pOperator;
    else
      path += L"Operator.exe";
    mBCI2000.OperatorPath( string( path ) );
    if( !mBCI2000.Connect() )
      throw bciexception <<
             "Could not start up BCI2000 -- "
             "does the BCI2000PresentationLink plugin reside in the BCI2000 prog folder?"
             "\nError was:\n"
             << mBCI2000.Result();
  }
  else
  {
    mBCI2000.OperatorPath( inSettings[DataPortSettings::OperatorPath] );
    if( !mBCI2000.Connect() )
      throw bciexception << mBCI2000.Result();
  }
  mBCI2000.WindowTitle( "Connected to Presentation" );
  mBCI2000.Execute( "shutdown system", 0 );
  mUseResumePause = !inSettings[DataPortSettings::UseResumePause].empty();
  mEventTypes.clear();
  istringstream iss( inSettings[DataPortSettings::EventTypes] );
  string eventType;
  while( iss >> eventType )
  {
    mEventTypes.push_back( eventType );
    mBCI2000.Execute( "add event " + eventType + " 32 0", 0 );
  }
  mBCI2000.Execute( "startup system localhost", 0 );
  if( !mBCI2000.Result().empty() )
    throw bciexception << mBCI2000.Result();
}

