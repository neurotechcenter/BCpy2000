////////////////////////////////////////////////////////////////////////////////
// Authors:
// Description: IIRPeakFilter implementation
////////////////////////////////////////////////////////////////////////////////

#include "IIRPeakFilter.h"
#include "BCIStream.h"
#include <math.h>

using namespace std;

IIRPeakFilter::IIRPeakFilter()
: mSamplingRate(0),
  mfcenter(0),
  mBandWidth(0)
{
	// initialize filter history variable pointers
	mX1=NULL;
	mX2=NULL;
	mY1=NULL;
	mY2=NULL;
}

IIRPeakFilter::~IIRPeakFilter()
{
	// free filter history variable array
	if (mX1!=NULL) delete[]mX1;
	if (mX2!=NULL) delete[]mX2;
	if (mY1!=NULL) delete[]mY1;
	if (mY2!=NULL) delete[]mY2;
}


// Designs a peaking filter with a bandwidth (mBandWidth) 
// at 3 dB level (Ab) 
void IIRPeakFilter::Initialize(int numCh)
{
	// normalize frequency center
	double WO = mfcenter / (mSamplingRate / 2.0);

	// transform filter parameters into radiant 
	WO = WO * M_PI;
	mBandWidth *= M_PI;

	// calculate 3dB attenuation
	double Ab = abs(10 * log10(0.5));

	// calculate gain in dB
	double Gb = pow(10.0 , (-Ab/20.0) );

	// gain factor
	double beta = (Gb / sqrt(1- pow(Gb,2.0)) ) * tan(mBandWidth / 2);
	double gain = 1/(1+beta);

	// calculate Nominator
	mNominator[0]   = (1-gain)*1;
	mNominator[1]   =  0 ;
	mNominator[2]   = (1-gain)*(-1);

	// calculate Denominator
	mDenominator[0] =  1 ;
	mDenominator[1] = -2*gain*cos(WO);
	mDenominator[2] = (2*gain-1);

	// delete filter history if necessary
	if (mX1!=NULL) 
	{ 
		delete[]mX1;
		mX1 = NULL;
	}

	if (mX2!=NULL) 
	{	
		delete[]mX2;
		mX2 = NULL;
	}

	if (mY1!=NULL) 
	{
		delete[]mY1;
		mY1 = NULL;
	}

	if (mY2!=NULL) 
	{
		delete[]mY2;
		mY2 = NULL;
	}

	// allocate new filter history array
	mX1 = new double[numCh];
	mX2 = new double[numCh];
	mY1 = new double[numCh];
	mY2 = new double[numCh];

	// initialize new filter history array
	for (int i = 0; i < numCh; i++)
	{
	  mX1[i]=0;
	  mX2[i]=0;
	  mY1[i]=0;
	  mY2[i]=0;
	}
}


void IIRPeakFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  for(int ch=0;ch<Input.Channels();++ch)
  {
	  for(int el=0;el<Input.Elements();++el)
	  {
		  // calculate new output
		  Output(ch, el) = mNominator[0]*Input(ch,el) + mNominator[1]*mX1[ch]   + mNominator[2]*mX2[ch] 
			                                        - mDenominator[1]*mY1[ch] - mDenominator[2]*mY2[ch];

		  // shift filter history values
		  mX2[ch] = mX1[ch];
		  mY2[ch] = mY1[ch];

		  mX1[ch] = Input(ch,el);
		  mY1[ch] = Output(ch,el);
	  }
  }
}


