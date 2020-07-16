////////////////////////////////////////////////////////////////////////////////
// Authors: Zhiyang Xu
// Description:  StimResponseVis header
////////////////////////////////////////////////////////////////////////////////
#ifndef VISUALIZATION_DEMO_FILTER_H
#define VISUALIZATION_DEMO_FILTER_H

#include "GenericFilter.h"

class StimResponseVis : public GenericFilter
{
 public:
  StimResponseVis();
  ~StimResponseVis();

 protected:
  void Publish() override;
  void Preflight( const SignalProperties& Input,
                        SignalProperties& Output ) const override;
  void Initialize( const SignalProperties& Input,
                   const SignalProperties& Output ) override;
  void Process( const GenericSignal& Input,
                      GenericSignal& Output ) override;
  void StartRun() override;
  void StopRun() override;

 private:
  struct Private;
  Private* p;
  
  
};

#endif // VISUALIZATION_DEMO_FILTER_H

