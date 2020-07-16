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
#include "ApplicationWindow.h"
#include "ApplicationBase.h"
#include "Gauge.h"

class GaugeApp : public ApplicationBase
{
 public:
  GaugeApp();
  ~GaugeApp();

 protected:
  virtual void Preflight( const SignalProperties& Input, SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Process( const GenericSignal &input, GenericSignal &output );

  bool InitDimension( Gauge* gauge, unsigned int gaugeIndex, unsigned int whichDim, unsigned int nChannels, bool force=false );

 private:
  void DeleteGauges();
  std::vector< Gauge* > mpGauges;
  int mUpdateTime;
  int mTimeSinceLastUpdate;
  double mTheoreticalMilliseconds;
  ApplicationWindow& mrDisplay;

  bool        GaugeHasAttr( int inGaugeIndex, std::string inAttrName ) const;
  double      GetGaugeAttr( int inGaugeIndex, std::string inAttrName, double      inDefaultValue, bool inRequired=false ) const;
  std::string GetGaugeAttr( int inGaugeIndex, std::string inAttrName, std::string inDefaultValue, bool inRequired=false ) const;
  ParamRef    GetGaugeAttr( int inGaugeIndex, std::string inAttrName, bool &      outFound,       bool inRequired=false ) const;
};
#endif // GAUGE_APP_MAIN_H