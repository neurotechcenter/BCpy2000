////////////////////////////////////////////////////////////////////////////////
// $Id: BreakoutGame.h 3293 2011-05-30 01:09:57Z jhill $
// Authors: griffin.milsap@gmail.com
// Description: A breakout clone implemented as an application module
//
// (C) 2000-2010, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef BREAKOUTGAME_H
#define BREAKOUTGAME_H

#include "../Game.h"
#include "BreakoutWidget.h"
#include "Expression/Expression.h"
#include <vector>
#include <string>

#include "RandomGenerator.h" // TODO: need more satisfactory solution

class BreakoutGame : public Game
{
 public:
  // Constructor/Destructor
  BreakoutGame();
  virtual ~BreakoutGame();

  virtual GameWidget* NewGameWidget() const { return new BreakoutWidget(); }
  virtual std::string ParamLookup( std::string paramName, std::string key ) const;

protected:

  // Virtual Interface
  virtual void OnPreflight( const SignalProperties& Input, GameWidget* widget ) const;
  virtual void OnInitialize( const SignalProperties& Input );
  virtual void OnProcess( const GenericSignal& Input );
  virtual void OnStartRun();
  virtual void OnStopRun();
  virtual void OnHalt();

  virtual void SetState(const char *name, int val);
  int ReadAsWholeNumberOfSampleBlocks( std::string paramName ) const;

  BreakoutWidget* GameSetup(GameWidget *widget, float paddleX, float paddleY) const; // the setup that is redundant between Preflight and Initialize

private:
  Expression mPaddleExpX;
  Expression mPaddleExpY;
  float mDXmultiplier;
  float mDYmultiplier;

  RandomGenerator mRandGen;

};

#endif // BREAKOUTGAME_H

