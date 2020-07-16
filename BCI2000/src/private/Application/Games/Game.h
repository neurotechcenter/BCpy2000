////////////////////////////////////////////////////////////////////////////////
// $Id: Game.h 3250 2011-05-05 13:38:39Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Describes a BCI2000 application game and manages a QGLWidget
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef GAME_H
#define GAME_H

#include "ApplicationBase.h"
#include "GameWidget.h"
#include "PrecisionTime.h"
#include <vector>

// Put things which need
class Game : public ApplicationBase
{
 public:
  // Constructor/Destructor
  Game();
  virtual ~Game();

  GameWidget* const Widget() const { return mpGameWidget; }

  // Virtual Interface - Usually games shouldn't overload these
  virtual void Preflight( const SignalProperties& Input, SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Halt();

  // Required interface which creates a game widget
  virtual GameWidget* NewGameWidget() const = 0;

protected:
  virtual void OnPreflight( const SignalProperties& Input, GameWidget* widget ) const { }
  virtual void OnInitialize( const SignalProperties& Input ) { }
  virtual void OnProcess( const GenericSignal& Input ) { }
  virtual void OnStartRun() { }
  virtual void OnStopRun() { }
  virtual void OnHalt() { }

  virtual float PacketsPerSecond() const { return (float)(1.0f/MeasurementUnits::SampleBlockDuration()); }
  virtual float SecondsPerPacket() const { return (float)MeasurementUnits::SampleBlockDuration(); }
  
 private:
  GameWidget* mpGameWidget;
  long mGameTime;
  PrecisionTime mLastTime;
};

#endif // GAME_H

