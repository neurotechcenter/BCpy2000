/////////////////////////////////////////////////////////////////////////////
// $Id: MouseFilter.h 2279 2009-02-12 21:40:22Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: The mouse move filter sets the mouse position based on
//   Provided expressions.  
//
// (C) 2000-2009, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#ifndef MOUSE_MOVE_FILTER_H
#define MOUSE_MOVE_FILTER_H

#include "GenericFilter.h"
#include "Expression/Expression.h"

class MouseMoveFilter : public GenericFilter
{
 public:
          MouseMoveFilter();
  virtual ~MouseMoveFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual bool AllowsVisualization() const { return false; }
  
  bool MoveMouse( int x, int y );

 private:
  Expression mExpX, mExpY;
  bool mXMove, mYMove;
  short mHotkey;
  bool mActive;
};

#endif // MOUSE_FILTER_H




