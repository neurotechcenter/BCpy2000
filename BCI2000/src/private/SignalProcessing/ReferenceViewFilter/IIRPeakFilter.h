////////////////////////////////////////////////////////////////////////////////
// Authors: 
// Description: IIRPeakFilter header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IIRPEAKFILTER_H  // makes sure this header is not included more than once
#define INCLUDED_IIRPEAKFILTER_H

#include "GenericFilter.h"

class IIRPeakFilter {
 public:
  IIRPeakFilter();
  ~IIRPeakFilter();
  void Initialize(int numCh);
  void Process( const GenericSignal& Input, GenericSignal& Output ) ;
  void SetSamplingRate(const int SamplingRate) {mSamplingRate = SamplingRate;}
  void SetBandWidth(const double BandWidth) {mBandWidth = BandWidth;}
  void SetFcenter(const double fcenter) {mfcenter = fcenter;}
  int GetSamplingRate()const {return mSamplingRate;}
  double Getfcenter()const {return mfcenter;}
  double GetBandWidth()const {return mBandWidth;}

 private:
   // Use this space to declare any IIRPeakFilter-specific methods and member variables you'll need
  int mSamplingRate;
  double mfcenter;
  float mBandWidth;
  double mNominator[3];
  double mDenominator[3];

  double *mX1;
  double *mX2;
  double *mY1;
  double *mY2;

};

#endif // INCLUDED_IIRPEAKFILTER_H
