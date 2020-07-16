////////////////////////////////////////////////////////////////////////////////
// Authors: 
// Description: ChannelVisFilter header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_CHANNELVISFILTER_H  // makes sure this header is not included more than once
#define INCLUDED_CHANNELVISFILTER_H

#include "GenericFilter.h"

class ChannelVisFilter : public GenericFilter
{
  public:
  ChannelVisFilter();
  ~ChannelVisFilter();
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

#endif // INCLUDED_CHANNELVISFILTER_H
