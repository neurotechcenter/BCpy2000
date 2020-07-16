////////////////////////////////////////////////////////////////////////////////
// Authors: 
// Description: EpochAvgFilter header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_EPOCHAVGFILTER_H  // makes sure this header is not included more than once
#define INCLUDED_EPOCHAVGFILTER_H

#include "GenericFilter.h"

class EpochAvgFilter : public GenericFilter
{
 public:
  EpochAvgFilter();
  ~EpochAvgFilter();
  void Publish() override;
  void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
  void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
  void StartRun() override;
  void Process( const GenericSignal& Input, GenericSignal& Output ) override;
  void StopRun() override;
  void Halt() override;

 private:
   // Use this space to declare any EpochAvgFilter-specific methods and member variables you'll need
   float* mpExampleArray;
};

#endif // INCLUDED_EPOCHAVGFILTER_H
