////////////////////////////////////////////////////////////////////////////////
// $Id: KeystrokeFilter.h 2279 2009-02-12 21:40:22Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A filter that watches a given state for changes, and simulates
//         a key press for the respective number key.
//
// (C) 2000-2009, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef KEYSTROKE_FILTER_H
#define KEYSTROKE_FILTER_H

#include "GenericFilter.h"
#include "Expression/Expression.h"
#include <string>
#include <vector>

struct KeystrokeOutput
{
  short key;
  short modifier1;
  short modifier2;
  Expression exp;
  bool on;
  unsigned char mode; // 0 = repeat, 1 = hold, 2 = stroke
  KeystrokeOutput()
  {
    on = false;
	mode = 0;
	key = modifier1 = modifier2 = 0;
  }
};

class KeystrokeFilter : public GenericFilter
{
 public:
          KeystrokeFilter();
  virtual ~KeystrokeFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual bool AllowsVisualization() const { return false; }

 private:
  void SendKeystroke( short );
  void SendKeyDown( short );
  void SendKeyUp( short );
  int ParseKey( std::string str, KeystrokeOutput &out );
  int SetKey( short key, KeystrokeOutput &out );
  int SetModifier( short mod, KeystrokeOutput &out );

  std::string mKeystrokeStateName;
  short       mPreviousStateValue;
  std::vector<KeystrokeOutput> mOutputs;
};

#endif // KEYSTROKE_FILTER_H


