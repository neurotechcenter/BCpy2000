////////////////////////////////////////////////////////////////////////////////
// $Id: HijackFilter.cpp 3798 2012-02-01 18:07:06Z mellinger $
// Authors: jezhill@gmail.com
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

#include "HijackFilter.h"
#include "Gaussian.h"

using namespace std;

RegisterFilter( HijackFilter, 2.A2 );

HijackFilter::HijackFilter()
{
 BEGIN_PARAMETER_DEFINITIONS
	
  "Filtering:Hijacking int Hijack= 0 0 0 2 "
      "// Turn on Hijacking? "
      " 0: no hijacking,"
      " 1: replace signal,"
      " 2: append channels,"
	  " (enumeration)",
	
  "Filtering:Hijacking matrix ControlSignal= "
	  " 2 { State%20name Take%20Derivative?   Offset   Gain  Take%20RMS?  Output%20Channel } "
	  "     MousePosX          1                0.0    -1.0      0                Left  "
	  "     MousePosX          1                0.0     1.0      0                Right "
      " % % % // Hijack controller matrix ",

  "Filtering:Hijacking float HijackNoise= 0.0 1.0 0 % "
      "// baseline noise level ",

 END_PARAMETER_DEFINITIONS

 mEnabled = false;
 mVirgin = true;

}


HijackFilter::~HijackFilter()
{
  Halt();
}


void
HijackFilter::Preflight( const SignalProperties& Input,
                               SignalProperties& Output ) const
{
	Output = Input;
	bool enabled = (int) Parameter( "Hijack" );
	if( !enabled )
		return;

	bool append = ( Parameter( "Hijack" ) == 2 );

	ParamRef matrix = Parameter( "ControlSignal" );
	if( matrix->NumColumns() != 6 ) {bcierr << "ControlSignal parameter must have 6 columns" << endl; return;}
	if( matrix->NumRows() == 0 ) {bcierr << "ControlSignal parameter must have at least 1 row" << endl; return;}
	int nRows = matrix->NumRows();
	vector<string> outputNames;
	vector<int> outputIsRMS;
	for( int row = 0; row < nRows; row++ )
	{
		string stateName = matrix(row, 0);
		int takeDerivative = matrix(row, 1);
		float offset = matrix(row, 2);
		float gain = matrix(row, 3);
		int takeRMS = matrix(row, 4);
		string outputName = matrix(row, 5);

		State( stateName );
		if( takeDerivative != 0 && takeDerivative != 1 ) bcierr << "error in ControlSignal parameter, row " << row+1 << ": value in column 2 (\"Take Derivative?\") must be either 0 or 1" << endl;
		if( takeRMS        != 0 && takeRMS        != 1 ) bcierr << "error in ControlSignal parameter, row " << row+1 << ": value in column 5 (\"Take RMS?\") must be either 0 or 1" << endl;
		
		unsigned int match;
		for( match = 0; match < outputNames.size(); match++ )
			if( outputNames[match] == outputName ) break;

		if( match < outputNames.size() )
		{
			//bciout << "Row " << row+1 << ": already found \"" << outputName << "\" at output channel #" << match+1 << endl;
			if( outputIsRMS[match] != takeRMS )
				bcierr << "error in ControlSignal parameter, column 5 (\"Take RMS?\"): values corresponding to output channel \"" << outputName << "\" are not consistent" << endl;
		}
		else
		{
			//bciout << "Row " << row+1 << ": new name \"" << outputName << "\" for output channel #" << match+1 << endl;
			outputNames.push_back( outputName );
			outputIsRMS.push_back( takeRMS );
		}

	}
	int keepch = ( append ? Input.Channels() : 0 );
	Output.SetChannels( keepch + outputNames.size() );
	for( int outch = 0; outch < Output.Channels(); outch++ )
		if( outch < keepch ) Output.ChannelLabels()[outch] = Input.ChannelLabels()[outch];
		else Output.ChannelLabels()[outch] = outputNames[outch-keepch];

	float baseNoise = Parameter( "HijackNoise" );

	PreflightCondition( Input.UpdateRate() > 0.0 );
}


void
HijackFilter::Initialize( const SignalProperties& Input,
                          const SignalProperties& Output )
{
	mEnabled = (int) Parameter( "Hijack" );
	if( !mEnabled ) return;
	mAppend = ( Parameter( "Hijack" ) == 2 );
	ParamRef matrix = Parameter( "ControlSignal" );

	unsigned int nHijackSignals = ( mAppend ? Output.Channels() - Input.Channels() : Output.Channels() );
	mNumberInRMS.clear();
	for( unsigned int ch = 0; ch < nHijackSignals; ch++ )
		mNumberInRMS.push_back( 0 );

	int nRows = matrix->NumRows();

	float millisecondsPerPacket = static_cast<float>( 1000.0 / Input.UpdateRate() );

	for( int row = 0; row < nRows; row++ )
	{
		string stateName = matrix(row, 0);
		int takeDerivative = matrix(row, 1);
		float offset = matrix(row, 2);
		float gain = matrix(row, 3);
		int takeRMS = matrix(row, 4);
		string outputName = matrix(row, 5);

		int outputIndex = (int) Output.ChannelIndex( outputName );
		mEachState.push_back( new HijackSignal( stateName, takeDerivative, offset, gain, takeRMS, outputIndex, millisecondsPerPacket ) );
		if( takeRMS ) mNumberInRMS[outputIndex]++;
	}
	mBaseNoise = Parameter( "HijackNoise" );
}

double
HijackFilter::Randn( const GenericSignal& Input )
{
	if( mVirgin || mRand.Seed() == 0 )
	{
		RandomGenerator::SeedType s = 0;
		// TODO: need much more satisfactory solution than this
		for( int ch = 0; ch < Input.Channels(); ch++ )
		{
			for(int el = 0; el < Input.Elements(); el++ )
			{
				double x = Input( ch, el);
				x = fabs( x );
				if( x == 0.0 )
				{
					x = 1.0;
					//bciout << "Output( " << ch << ", " << el << " ) is 0" << endl;
				}
				while( x < 100.0 ) x *= 10.0;
				s += (RandomGenerator::SeedType)x;
			}
		}
		mRand.SetSeed(s);
		mVirgin = false;
	}
	double randu = ( 1.0 + (double)mRand.Random() ) / ( 2.0 + (double)mRand.RandMax() ); // 0.0 < randu < 1.0
	return InverseCumulativeGaussian( randu );
}

void
HijackFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
	//bciout << Input.Channels() << "x" << Input.Elements() << " -> " << Output.Channels() << "x" << Output.Elements() << endl;
	if( !mEnabled )
	{
		Output = Input;
		return;
	}

	// zero the output signal
	for( int ch = 0; ch < Output.Channels(); ch++ )
	{
		for( int el = 0; el < Output.Elements(); el++ )
		{
			if( mAppend && ch < Input.Channels() )
				Output( ch, el ) = Input( ch, el );
			else
				Output( ch, el ) = 0.0f;
		}
	}

	// deal with each row/state: for all output elements, read the state, then {take derivative, scale, square} as instructed, then add the result to the relevant output channel at that time-point
	for( vector<HijackSignal*>::iterator i = mEachState.begin(); i != mEachState.end(); i++ )
	{
		StateRef s = State( (*i)->StateName() );
		(*i)->Process( s, Input, Output );
	}
	
	unsigned int keepch = ( mAppend ? Input.Channels() : 0 );
	unsigned int nHijackSignals = Output.Channels() - keepch;
	for( unsigned int ch = 0; ch < nHijackSignals; ch++ )
	{
		for( int el = 0; el < Output.Elements(); el++ )
		{
			// For RMS outputs, take the mean, then the square root, of each element.
			if( mNumberInRMS[ch] )
				Output( keepch+ch, el ) = ::sqrt( Output( keepch+ch, el ) / (float)mNumberInRMS[ch] );
			// For others, half-wave rectify
			else if( Output( keepch+ch, el ) < 0.0f )
				Output( keepch+ch, el ) = 0.0f;
			
			Output( keepch+ch, el ) *= Randn( Input );
			Output( keepch+ch, el ) += Randn( Input ) * mBaseNoise;
		}
	}

}

void
HijackFilter::StartRun()
{
	for( vector<HijackSignal*>::iterator i = mEachState.begin(); i != mEachState.end(); i++ )
		(*i)->Reset();
	
	mVirgin = true;
}

void
HijackFilter::Halt()
{
	while( mEachState.size() )
	{
		delete mEachState.back();
		mEachState.pop_back();
	}
}



HijackSignal::HijackSignal(string & stateName, bool takeDerivative, float offset, float gain, bool takeRMS, int outputIndex, float millisecondsPerPacket )
{
	mVirgin = true;
	mPrev = 0.0f;

	mStateName = stateName;
	mTakeDerivative = takeDerivative;
	mOffset = offset;
	mGain = gain;
	mTakeRMS = takeRMS;
	mOutputIndex = outputIndex;
	mMillisecondsPerPacket = millisecondsPerPacket;
}

HijackSignal::~HijackSignal()
{
	
}

void
HijackSignal::Process( StateRef &stateRef, const GenericSignal& Input, GenericSignal& Output )
{
	bool wholeBlock = true; // TODO: explore setting to false?

	if( mVirgin )
		mPrev = 2.0f * (float) stateRef( 0 ) - (float) stateRef( Input.Elements()-1 );  // initial derivative element will be equal to the subsequent element if possible (no spike in control signal) or 0 if not

	float x = 0.0f;
	for( int el = 0; el < Input.Elements(); el++ )
	{
		if( el == 0 || !wholeBlock )
		{
			x = (float) stateRef( el );
			if( mTakeDerivative )
			{
				float prev = mPrev;
				mPrev = x;
				x -= prev;
				if( wholeBlock ) x /= mMillisecondsPerPacket;
				else x /= ( mMillisecondsPerPacket / Input.Elements() );
			}
			x -= mOffset;
			x *= mGain;
			if( mTakeRMS ) x *= x;
		}
		if( mVirgin ) x = 0.0;  // suppress first block's output entirely
		Output( mOutputIndex, el ) = Output( mOutputIndex, el ) + x;
	}
	mVirgin = false;
}

