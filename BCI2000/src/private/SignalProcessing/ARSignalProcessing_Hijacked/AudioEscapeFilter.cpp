////////////////////////////////////////////////////////////////////////////////
//  $Id: AudioEscapeFilter.cpp 4981 2015-08-26 17:04:38Z mellinger $
//  Author:      juergen.mellinger@uni-tuebingen.de
//  Description: A (working) tutorial low pass filter demonstrating
//               parameter access, visualization, and unit conversion.
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "AudioEscapeFilter.h"

#include <vector>
#include <cmath>
#include <limits>

using namespace std;

RegisterFilter( AudioEscapeFilter, 2.A3 );

AudioEscapeFilter::AudioEscapeFilter()
: mLeftChIndex( -1 ),
  mRightChIndex( -1 )
{
	BEGIN_PARAMETER_DEFINITIONS
		"Filtering:Audio%20Escape string AudioEscapeLeft= % % % % // name or index of left audio signal channel",
		"Filtering:Audio%20Escape string AudioEscapeRight= % % % % // name or index of right audio signal channel",
	END_PARAMETER_DEFINITIONS

	BEGIN_STATE_DEFINITIONS
		"AudioLeft  16 0 0 0",
		"AudioRight 16 0 0 0",
	END_STATE_DEFINITIONS
}


AudioEscapeFilter::~AudioEscapeFilter()
{
}


void
AudioEscapeFilter::Preflight( const SignalProperties& Input,
						            SignalProperties& Output ) const
{
	string inputName;
	inputName = (string)Parameter( "AudioEscapeLeft" ) ;
	if( inputName.size() )
	{
		double ind = Input.ChannelIndex( inputName );
		if( ::ceil(ind) < 0.0 || ind >= Input.Channels() ) bcierr << "invalid channel specification\"" << inputName << "\"" << endl;
	}
	inputName = (string)Parameter( "AudioEscapeRight" ) ;
	if( inputName.size() )
	{
		double ind = Input.ChannelIndex( inputName );
		if( ::ceil(ind) < 0.0 || ind >= Input.Channels() ) bcierr << "invalid channel specification\"" << inputName << "\"" << endl;
	}
	Output = Input;
}


void
AudioEscapeFilter::Initialize( const SignalProperties& Input,
					  const SignalProperties& /*Output*/ )
{
	if ( Parameter( "AudioEscapeLeft" ) == "" )
		mLeftChIndex = -1;
	else
		mLeftChIndex = (int)( 0.5 + Input.ChannelIndex( Parameter( "AudioEscapeLeft" ) ) );
	
	if ( Parameter( "AudioEscapeRight" ) == "" )
		mRightChIndex = -1;
	else
		mRightChIndex = (int)( 0.5 + Input.ChannelIndex( Parameter( "AudioEscapeRight" ) ) );

	mWarned = false;
	//bciout << "mLeftChIndex = " << mLeftChIndex << endl;
	//bciout << "mRightChIndex = " << mRightChIndex << endl;
}


void
AudioEscapeFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
	//bciout << Input.Channels() << "x" << Input.Elements() << " -> " << Output.Channels() << "x" << Output.Elements() << endl;
	bool warned = false;
	if( mLeftChIndex >= 0 )
	{
		double rms = 0.0;
		double nEl = Input.Elements();
		for( int el=0; el < nEl; ++el )
		{
 			double val = Input( mLeftChIndex, el );
			rms += val * val;
		}
		rms = sqrt( rms / nEl );
		if( rms > 65535.0 )
		{
			rms = 65535.0;
			if( !mWarned ) { bciout << "AudioLeft maxed out" << endl; warned = true; }
		}
		State( "AudioLeft" )  = int( rms + 0.5 );
	}
	if( mRightChIndex >= 0 )
	{
		double rms = 0.0;
		double nEl = Input.Elements();
		for( int el=0; el < nEl; ++el )
		{
 			double val = Input( mRightChIndex, el );
			rms += val * val;
		}
		rms = sqrt( rms / nEl );
		if( rms > 65535.0 )
		{
			rms = 65535.0;
			if( !mWarned ) { bciout << "AudioRight maxed out" << endl; warned = true; }
		}
		State( "AudioRight" ) = int( rms + 0.5 );
	}
	if( warned ) mWarned = true;
	Output = Input;
}
