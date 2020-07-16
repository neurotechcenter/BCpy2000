////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: Peter Brunner
// Description: IIRFilterBank implementation
//
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "IIRFilterBank.h"
#include "BCIStream.h"


using namespace std;

RegisterFilter( IIRFilterBank, 2.A );

IIRFilterBank::IIRFilterBank()
{


 BEGIN_PARAMETER_DEFINITIONS

   "Filtering:IIRFilterBank matrix IIRFilterBankZeros= "
   "{ Filter1 Filter2} "
   "1 "
   "{ "
     "matrix "
     "{ z1 z2 z3 z4 z5 z6 z7 z8 } "
     "{ real(z) imag(z) } "
     "1 0 "
     "1 0 "
     "1 0 "
     "1 0 "
     "-1 0 "
     "-1 0 "
     "-1 0 "
     "-1 0 "
   "} "
   "{ "
     "matrix "
     "{ z1 z2 z3 z4 z5 z6 z7 z8 } "
     "{ real(z) imag(z) } "
     "1 0 "
     "1 0 "
     "1 0 "
     "1 0 "
     "-1 0 "
     "-1 0 "
     "-1 0 "
     "-1 0 "
   "} "

   " // IIR Filter Bank Zeros ",
   "Filtering:IIRFilterBank matrix IIRFilterBankPoles= "
   "{ Filter1 Filter2 } "
   "1 "
   "{ "
     "matrix "
     "{ p1 p2 p3 p4 p5 p6 p7 p8 } "
     "{ real(p) imag(p) } "
     "0.999463640167419287152484 0.030243055876665370096390 "
     "0.999463640167419287152484 -0.030243055876665370096390 "
     "0.999476004741402301867481 0.029864701730983125166352 "
     "0.999476004741402301867481 -0.029864701730983125166352 "
     "0.999356197647571886655271 0.030127902760410378568690 "
     "0.999356197647571886655271 -0.030127902760410378568690 "
     "0.999361896428409890447142 0.029971218084351779892271 "
     "0.999361896428409890447142 -0.029971218084351779892271 "
   "} "
   "{ "
     "matrix "
     "{ p1 p2 p3 p4 p5 p6 p7 p8 } "
     "{ real(p) imag(p) } "
     "0.999097529424466501701829 0.040573117904895442120683 "
     "0.999097529424466501701829 -0.040573117904895442120683 "
     "0.999113595357401074181780 0.040193917166033661658719 "
     "0.999113595357401074181780 -0.040193917166033661658719 "
     "0.998990996271656106486603 0.040456808956759647555312 "
     "0.998990996271656106486603 -0.040456808956759647555312 "
     "0.998998082161135059209300 0.040299773778670403001634 "
     "0.998998082161135059209300 -0.040299773778670403001634 "
   "} "
   " // IIR Filter Bank Poles ",
   "Filtering:IIRFilterBank matrix IIRFilterBankGains= "
     "{ k1 k2 } " // row labels
     "{ gain } " // num columns
     "1.761317133420864861968642673144e-15 " // filenames
     "1.779726455763113744570732437559e-15 " // angles
     " // IIR Filter Bank Gains ",
  END_PARAMETER_DEFINITIONS


  mpFilter      = NULL;
  mpZeros       = NULL;
  mpPoles       = NULL;
  mpGain        = NULL;
  mpDataBuffer  = NULL;
  mpDataOutput  = NULL;

}


IIRFilterBank::~IIRFilterBank()
{
  Halt();
}

void
IIRFilterBank::Halt()
{
}

void
IIRFilterBank::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{

  Parameter("IIRFilterBankZeros");
  Parameter("IIRFilterBankPoles");
  Parameter("IIRFilterBankGains");

  ParamRef IIRFilterBankZeros = Parameter( "IIRFilterBankZeros" );
  ParamRef IIRFilterBankPoles = Parameter( "IIRFilterBankPoles" );
  ParamRef IIRFilterBankGains = Parameter( "IIRFilterBankGains" );

  if (strcmp(IIRFilterBankZeros->Type().c_str(), "matrix") != 0)
    bcierr <<  "Parameters IIRFilterBankZeros must be a matrix." << endl;

  if (strcmp(IIRFilterBankPoles->Type().c_str(), "matrix") != 0)
    bcierr <<  "Parameters IIRFilterBankPoles must be a matrix." << endl;

  if (strcmp(IIRFilterBankGains->Type().c_str(), "matrix") != 0)
    bcierr <<  "Parameters IIRFilterBankGains must be a matrix." << endl;

  if ((IIRFilterBankZeros->NumRows() != IIRFilterBankPoles->NumRows()) || (IIRFilterBankZeros->NumRows() != IIRFilterBankGains->NumRows()))
    bcierr <<  "Parameters IIRFilterBankZeros, IIRFilterBankPoles and IIRFilterBankGains must have the same number of rows." << endl;

  if (IIRFilterBankZeros->NumColumns() != 1)
    bcierr <<  "Parameters IIRFilterBankZeros must have only one column." << endl;

  if (IIRFilterBankPoles->NumColumns() != 1)
    bcierr <<  "Parameters IIRFilterBankPoles must have only one column." << endl;

  if (IIRFilterBankGains->NumColumns() != 1)
    bcierr <<  "Parameters IIRFilterBankGains must have only one column." << endl;
      

  Output = Input; // this simply passes information through about SampleBlock dimensions, etc....

  Output.SetChannels( Input.Channels() * Parameter( "IIRFilterBankZeros" )->NumRows() );
 

  for (int idx_filter = 0; idx_filter < IIRFilterBankZeros->NumRows(); idx_filter++)
  {

    for( int idx_ch = 0; idx_ch < Input.Channels(); idx_ch++ )
    {
      string label_ch     = Input.ChannelLabels()[idx_ch];
      string label_filter = IIRFilterBankZeros->Labels()[idx_filter];
      string label_out = label_ch + string( "@" ) + label_filter;
      Output.ChannelLabels()[idx_ch + idx_filter * Input.Channels()] = label_out;
    }

  }


}


void
IIRFilterBank::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{

  ParamRef IIRFilterBankZeros = Parameter( "IIRFilterBankZeros" );
  ParamRef IIRFilterBankPoles = Parameter( "IIRFilterBankPoles" );
  ParamRef IIRFilterBankGains = Parameter( "IIRFilterBankGains" );

  m_num_filters = IIRFilterBankZeros->NumRows();

  if (mpFilter     != NULL) delete[]mpFilter;
  if (mpZeros      != NULL) delete[]mpZeros;
  if (mpPoles      != NULL) delete[]mpPoles;
  if (mpGain       != NULL) delete[]mpGain;
  if (mpDataBuffer != NULL) delete[]mpDataBuffer;
  if (mpDataOutput != NULL) delete[]mpDataOutput;

  mpFilter      = new IIRFilter<float>[m_num_filters];
  mpZeros       = new ComplexVector[m_num_filters]; 
  mpPoles       = new ComplexVector[m_num_filters]; 
  mpGain        = new Real[m_num_filters];
  mpDataBuffer  = new GenericSignal[m_num_filters];
  mpDataOutput  = new GenericSignal[m_num_filters];

  for (int idx_filter=0; idx_filter<m_num_filters; idx_filter++) 
  {
    
    int num_coefficients = IIRFilterBankZeros(idx_filter,0)->NumRows();

    mpGain[idx_filter] = IIRFilterBankGains(idx_filter,0);
    
    mpFilter[idx_filter].SetGain(mpGain[idx_filter]);

    ParamRef ZerosCoefs = IIRFilterBankZeros( idx_filter, 0 );
    ParamRef PolesCoefs = IIRFilterBankPoles( idx_filter, 0 );

    for (int idx_coefficient=0; idx_coefficient<num_coefficients; idx_coefficient++)  
    {      

      Complex zero_coef(ZerosCoefs(idx_coefficient,0),ZerosCoefs(idx_coefficient,1));
      mpZeros[idx_filter].push_back(zero_coef);

      Complex poles_coef(PolesCoefs(idx_coefficient,0),PolesCoefs(idx_coefficient,1));
      mpPoles[idx_filter].push_back(poles_coef);

    }

    mpFilter[idx_filter].SetZeros(mpZeros[idx_filter]);
    mpFilter[idx_filter].SetPoles(mpPoles[idx_filter]);
    mpFilter[idx_filter].Initialize(Input.Channels());

    mpDataBuffer[idx_filter].SetProperties(Input);
    mpDataOutput[idx_filter].SetProperties(Input);
   

  }

}

void
IIRFilterBank::StartRun()
{

}


void
IIRFilterBank::Process( const GenericSignal& Input, GenericSignal& Output )
{

  for (int idx_filter=0; idx_filter<m_num_filters; idx_filter++) 
  {
    mpFilter[idx_filter].Process(Input,mpDataOutput[idx_filter]); 
  }

  for (int idx_filter=0; idx_filter<m_num_filters; idx_filter++) 
  {
    for( int idx_channel = 0; idx_channel < Input.Channels(); idx_channel++ )
    {
      for( int idx_sample = 0; idx_sample < Output.Elements(); idx_sample++ )
      {
        Output( idx_channel + idx_filter*Input.Channels(), idx_sample ) = mpDataOutput[idx_filter]( idx_channel, idx_sample );
      }
    }
  }
  
}

void
IIRFilterBank::StopRun()
{

}

