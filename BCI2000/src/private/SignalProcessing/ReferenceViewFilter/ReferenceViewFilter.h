////////////////////////////////////////////////////////////////////////////////
// Authors: 
// Description: ReferenceViewFilter header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_REFERENCEVIEWFILTER_H  // makes sure this header is not included more than once
#define INCLUDED_REFERENCEVIEWFILTER_H

#include "GenericFilter.h"
#include "IIRFilter.h"
#include "IIRFilterBase.h"
#include "IIRPeakFilter.h"
#include <algorithm>

class ReferenceViewFilter : public GenericFilter
{
 public:
  public:
           ReferenceViewFilter();
  virtual ~ReferenceViewFilter();
  virtual void Halt();
  virtual void Preflight(  const SignalProperties& Input,       SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void StartRun();
  virtual void Process(    const GenericSignal&    Input,       GenericSignal&    Output );
  virtual void StopRun();

  typedef double Real;
  typedef std::complex<Real> Complex;
  typedef IIRFilter<Real>::ComplexVector ComplexVector;

 private:

   /* IIRFilter<float> *mpFilter;
    ComplexVector *mpZeros;
    ComplexVector *mpPoles;
    Real *mpGain;
    int m_num_filters;
    GenericSignal *mpDataBuffer; 
    GenericSignal *mpDataOutput;*/

    double *mpFcenter;
	double *mpBandWidth;
	IIRPeakFilter *mpFilter;
	int mpSamplingRate;
	int mpNumFilter; 
    GenericSignal *mpDataOutput;
	double mDeltaF;
	double mDeltaB;
	//filter to the left of the frequency center 
	double *mpFcenterL;
	double *mpBandWidthL;
	IIRPeakFilter *mpFilterL;
    GenericSignal *mpDataOutputL;
	//filter to the right of the frequency center  
	double *mpFcenterR;
	double *mpBandWidthR;
	IIRPeakFilter *mpFilterR;
    GenericSignal *mpDataOutputR;
	//reference to a list of channels
	std::vector<int> mListRefChannels;
	GenericVisualization mRefToChannelsVis;
	GenericSignal mRefToChannelsSig;


	//reference to the rest of channels
	std::vector<int> mListNotRefChannels;
	std::vector<int> mRestRefChannels;
	GenericVisualization mRefToRestVis;
	GenericSignal mRefToRestSig;
	GenericSignal mRefChannelsSig;

	//bipole
	std::vector<int> mCHplus;
	std::vector<int> mCHminor;
	GenericVisualization mBipoleRefVis;
	GenericSignal mBipoleRefSig;

	std::vector<std::string> mUserChannelLabels; 
	std::vector<int> mChannelWillTurnOff;
	std::set<int> mBipolerChannelWillTurnOff;

	


};

#endif // INCLUDED_REFERENCEVIEWFILTER_H
