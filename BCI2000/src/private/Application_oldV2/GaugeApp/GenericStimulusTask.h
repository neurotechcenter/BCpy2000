////////////////////////////////////////////////////////////////////////////////
// $Id: GenericStimulusTask.h 2100 2008-08-19 13:40:43Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de, griffin.milsap@gmail.com
// Description: A base class for application modules that present stimuli
//   Inheriting from ApplicationBase, descendants of StimulusTask have access
//   to the AppLog, AppLog.File and AppLog.Screen streams declared in
//   ApplicationBase.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef GENERIC_STIMULUS_TASK_H
#define GENERIC_STIMULUS_TASK_H

#include "ApplicationBase.h"
#include "DisplayWindow.h"

class GenericStimulusTask : public ApplicationBase
{
 protected:
  // Events to be handled by GenericStimulusTask descendants.
  //  Events triggered by the GenericFilter interface
  virtual void OnPreflight( const SignalProperties& Input ) const {}
  virtual void OnInitialize( const SignalProperties& Input ) {}
  virtual void OnStartRun() {}
  virtual void OnStopRun() {}
  virtual void OnHalt() {}

 protected:
  // Access to the Display property.
  GUI::GraphDisplay& Display()
    { return mDisplay; }
  const GUI::GraphDisplay& Display() const
    { return mDisplay; }

 protected:
  GenericStimulusTask();

 public:
  virtual ~GenericStimulusTask();

  // Implementation of the GenericFilter interface.
  // Normally, there should be no need to override these functions from
  // a descendant class.
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Halt();

 private:
  // Display elements.
  GUI::DisplayWindow mDisplay;
};

#endif // GENERIC_STIMULUS_TASK_H

