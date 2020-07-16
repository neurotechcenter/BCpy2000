////////////////////////////////////////////////////////////////////////////////
// $Id: BAlertADC.h 4536 2013-08-05 14:30:13Z mellinger $ BAlertADC.cpp, 2011-12-12 v0.1 $
// Authors: Dr.Giby Raphael (graphael@b-alert.com)
// Version: v0.1, 12-12-11
// Description: BCI2000 interface module for Advanced Brain Monitoring Inc, 
//				B-Alert devices (X10 & X24)
//
// (C)2000-2011, BCI2000 Project 
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_BAlertADC_H  // makes sure this header is not included more than once
#define INCLUDED_BAlertADC_H

#include "GenericADC.h"
#include "PrecisionTime.h"

class BAlertADC : public GenericADC
{
 public:
           BAlertADC();
  virtual ~BAlertADC();
  virtual void Halt();
  virtual void Preflight(  const SignalProperties& Input,       SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input, const SignalProperties& Output );
  virtual void StartRun();
  virtual void Process(    const GenericSignal&    Input,       GenericSignal&    Output );
  virtual void StopRun();

 private:
  int mABMDeviceType,
	  mSourceCh,
	  mSampleBlockSize,
	  mSamplingRate;
};

#endif // INCLUDED_BAlertADC_H
