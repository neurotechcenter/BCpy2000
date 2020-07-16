////////////////////////////////////////////////////////////////////////////////
// $Id: GaugeAppMain.h 1723 2008-01-16 17:46:33Z mellinger $
// Authors: jhizver@wadsworth.org, schalk@wadsworth.org,
//   juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: An application which displays various gauges given control
//              signals.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef GAUGE_APP_MAIN_H
#define GAUGE_APP_MAIN_H

#include <vector>
#include "GenericStimulusTask.h"
#include "Gauge.h"

namespace SequenceTypes
{
  enum
  {
    Deterministic = 0,
    Random,
  };
}

struct parsedSignal
{
  int channel;
  int element;
  bool success;
  parsedSignal() { channel = 0; element = 0; success = false; }
};

class GaugeApp : public GenericStimulusTask
{
 public:
  GaugeApp();
  ~GaugeApp();

 protected:
  // GenericStimulusTask events
  virtual void    OnPreflight( const SignalProperties& Input ) const;
  virtual void    OnInitialize( const SignalProperties& Input );
  virtual void    OnStartRun();
  virtual void    OnStopRun();
  virtual void    Process( const GenericSignal &input, GenericSignal &output );

 private:
  void DeleteGauges();
  parsedSignal parseSignal( const std::string str ) const;
  std::vector< Gauge* > mpGauges;
  int mUpdateTime;
  int mTimeSinceLastUpdate;
};
#endif // GAUGE_APP_MAIN_H