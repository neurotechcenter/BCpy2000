/////////////////////////////////////////////////////////////////////////////
// $Id: PPJoystickFilter.h 2279 2009-02-12 21:40:22Z gmilsap $
// Author: griffin.milsap@gmail.com
// Description: Emulates a PPJoy virtual joystick for output to applications
//
// (C) 2000-2009, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#ifndef PPJOYSTICK_FILTER_H
#define PPJOYSTICK_FILTER_H

#include "GenericFilter.h"
#include "ppjioctl.h"
#include "Expression/Expression.h"

#ifdef WIN32
  #include <windows.h>
#endif // WIN32

#define	NUM_ANALOG	2		// Number of analog values which we will provide
#define	NUM_DIGITAL	1		// Number of digital values which we will provide 

#pragma pack(push,1)		// All fields in structure must be byte aligned.
typedef struct
{
 unsigned long	Signature;				// Signature to identify packet to PPJoy IOCTL
 char			NumAnalog;				      // Num of analog values we pass
 long			Analog[NUM_ANALOG];		  // Analog values
 char			NumDigital;				      // Num of digital values we pass
 char			Digital[NUM_DIGITAL];	  // Digital values
}	JOYSTICK_STATE;
#pragma pack(pop)

class PPJoystickFilter : public GenericFilter
{
 public:
          PPJoystickFilter();
  virtual ~PPJoystickFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual bool AllowsVisualization() const { return false; }

 private:
  // Other IOCTL required parameters
  DWORD				      mRetSize;
  DWORD				      mRc;
  char				      mCh;
  long*             mpAnalog;
  char*             mpDigital;
  char              mpDevName[64];
  
  // Windows Joystick Handle Information
#ifdef WIN32
  HANDLE				    mH;
  JOYSTICK_STATE		mJoyState;
#endif // WIN32

  std::string       mJoyName;
  Expression        mExpX, mExpY, mExpDig1;
  
  bool              mActive;
};

#endif // PPJOYSTICK_FILTER_H