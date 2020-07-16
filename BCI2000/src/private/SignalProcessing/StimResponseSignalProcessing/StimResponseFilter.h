////////////////////////////////////////////////////////////////////////////////
// Authors: Zhiyang Xu
// Description:  StimResponseFilter header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_XUSPACIALFILTER_H  // makes sure this header is not included more than once
#define INCLUDED_XUSPACIALFILTER_H

#include "GenericFilter.h"
#include "IIRFilter.h"
#include "IIRFilterBase.h"

#include "GenericVisualization.h"

#include <list>
#include <vector>
#include "Color.h"

#define SET_BASELINE

using namespace std;

class StimResponseFilter : public  GenericFilter
{

  //typedef IIRFilterBase Super;

  public:
  StimResponseFilter();
  virtual ~StimResponseFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void StartRun();
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );
  
  //define new type of variables 
  typedef double Real;
  typedef std::complex<Real> Complex;
  typedef IIRFilter<Real>::ComplexVector ComplexVector;
  

 private:
  
  //variables for avg time response visualization  	 
  GenericVisualization mVisVoltageAvg;
  GenericSignal mSigVoltageAccumulated;
  GenericSignal mSigVoltageAvg;
  bool mbVisVoltageAvg;
  //variables for frequency response visualization
  GenericVisualization mVisBandpower;
  GenericSignal SigBandpower;
  bool mbVisBandpower;
  //variables for time response visualization
  GenericVisualization mVisVoltage;
  GenericSignal SigVoltage;
  bool mbVisVoltage;

  //variables for design filter
  IIRFilter<float> *mpFilter; //store the designed filter 
  ComplexVector *mpZeros; //store the designed zeros
  ComplexVector *mpPoles; //store the designed poles
  Real *mpGain; //store the designed gains 
  //store the peoperty for the designed filter 
  GenericSignal *mpDataBuffer; 
  GenericSignal *mpDataOutput;
  //variables to store the user specification of the designed filter  
  struct FilterParameters{
    float HighPassCorner;
    int   HighPassOrder;
	float LowPassCorner;
	int   LowPassOrder;
	 float NotchCenter;
	int   NotchOrder;
	 float FilterGain;
	 int   Downsample;
   } mFilterParamVoltage , mFilterParamBandpower, mFilterParamBandpowerLowpass;
  //function use to design filter 
  void DesignFilter( SignalProperties&,
                    Real& gain,
                    ComplexVector& zeros,
                    ComplexVector& poles ,
					FilterParameters mparameter
					) const;
  //list the name of the filter will be designed 
  enum FilterType
  {
	  Bandpower =0,
	  Voltage,
	  BandpowerLowpass,
  };
  
  //variables for epoching 
  int CurrentNumBlocks; //record current number of blocks in one epoch
  std::list<GenericSignal> mSigBlockContainer; //used to store new block of data 
  std::list<GenericSignal> mSigBandpowerBlock; //store the blocks of data for frequency output 
  std::list<GenericSignal> mSigVoltageBlock; //store the blocks of data for time response 
  SignalProperties mPropThreeBlock; //propery of 3 blocks of data 
  
  //variables for reference 
  std::vector<int> mListRefChannels; //vector to store the user input reference channels 
  GenericSignal mSigReferenceChannel; //store the value for the reference channel
  GenericSignal mSigReferencedSignal; //store the referenced input 
  
  //variables used when have digital input 
  bool mbUseDigitalInput; //flag indicate used digital input or not 
  std::list<GenericSignal> mListSigDigitalInput; //store the digital input  
  GenericSignal mSigDigitalInput; //copy digital input to a signal

  //helper variables 
  GenericSignal mSigHelperSignal; //used when remove artifact
  GenericSignal mSigSecondBlock; //used to store the input bloc with stimulus
  GenericSignal mSigRawSignal; //help to ouput fre and time domain signal
  int mNumUpdateCounter; //help to keep the number of output updates 
  int mNumStimulusOffsetSamples; //record how many ele the user want to shift the stimulus forward 
  int mNumStimulusLength; //record the length of input stimulus 
  double mNumStimulusStartSamples; //record the start point of the stimulus 
  int mNumBlocksPerEpoch; //record how many blocks will be in one epoch
  bool mbStimulusInBlock; //flag indicate if there is a stimulus in one block 
};




#endif // INCLUDED_XUSPACIALFILTER_H
