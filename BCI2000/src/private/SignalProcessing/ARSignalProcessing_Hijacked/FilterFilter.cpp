#include "FilterFilter.h"

using namespace std;

RegisterFilter( FilterFilter, 2.B3 );

FilterFilter::FilterFilter()
{
	BEGIN_PARAMETER_DEFINITIONS
		"Filtering:IIR%20Filter float HighPassCorner=  0      0   0 % // ",
		"Filtering:IIR%20Filter float LowPassCorner=   0      0   0 % // ",
		"Filtering:IIR%20Filter float NotchCenter=     0     60Hz 0 % // ",
		"Filtering:IIR%20Filter float FilterGain=      1.0    1.0 0 % // ",
	END_PARAMETER_DEFINITIONS
	// TODO
}

FilterFilter::~FilterFilter()
{
	Halt();
}

void
FilterFilter::Halt()
{
	
}

void
FilterFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
	double hp = Parameter( "HighPassCorner" ).InHertz() / Input.SamplingRate();
	double lp = Parameter( "LowPassCorner" ).InHertz() / Input.SamplingRate();
	double notch = Parameter( "NotchCenter" ).InHertz() / Input.SamplingRate();
	double gain = Parameter( "FilterGain" );
	Output = Input;
}

void
FilterFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
	double hp = Parameter( "HighPassCorner" ).InHertz() / Input.SamplingRate();
	double lp = Parameter( "LowPassCorner" ).InHertz() / Input.SamplingRate();
	double notch = Parameter( "NotchCenter" ).InHertz() / Input.SamplingRate();
	mGain = Parameter( "FilterGain" );
	mFilter.HPCorner( hp );
	mFilter.LPCorner( lp );
	mFilter.NotchCenter( notch );
	mEnabled = ( hp > 0.0 || lp > 0.0 || notch > 0.0 );
}


void
FilterFilter::StartRun()
{
	mFilter.Reset();
}

void
FilterFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
	//bciout << Input.Channels() << "x" << Input.Elements() << " -> " << Output.Channels() << "x" << Output.Elements() << endl;
	if( mEnabled )
	{
		mFilter.Process( Input, Output );
		for(int ch = 0; ch < Output.Channels(); ch++)
			for(int el = 0; el < Output.Elements(); el++)
				Output( ch, el ) *= mGain;
	}
	else Output = Input;
}

