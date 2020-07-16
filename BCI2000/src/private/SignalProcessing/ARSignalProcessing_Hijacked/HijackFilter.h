////////////////////////////////////////////////////////////////////////////////
// $Id: HijackFilter.h 3798 2012-02-01 18:07:06Z mellinger $
// Authors: jezhill@gmail.com
//
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
#ifndef HIJACK_FILTER_H
#define HIJACK_FILTER_H

#include "GenericFilter.h"
#include "RandomGenerator.h"
#include <vector>
#include <string>

class HijackSignal
{
public:
	HijackSignal( std::string & stateName, bool takeDerivative, float offset, float gain, bool takeRMS, int outputIndex, float millisecondsPerPacket );
	virtual ~HijackSignal();

	void        Reset() { mVirgin = true; }
	std::string StateName() { return mStateName; }
	void        Process( StateRef & stateRef, const GenericSignal& Input, GenericSignal& Output );

private:
	bool        mVirgin;
	float       mPrev;
	std::string mStateName;
	bool        mTakeDerivative;
	float       mOffset;
	float       mGain;
	bool        mTakeRMS;
	int         mOutputIndex;
	float       mMillisecondsPerPacket;
};

class HijackFilter : public GenericFilter
{

 public:
  HijackFilter();
  virtual ~HijackFilter();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void StartRun();
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual void Halt();
  virtual double Randn( const GenericSignal& Input );

private:
  bool                       mEnabled;
  bool                       mAppend;
  bool                       mVirgin;
  RandomGenerator            mRand;
  std::vector<HijackSignal*> mEachState;
  std::vector<int>           mNumberInRMS;
  float                      mBaseNoise;
};

#endif // HIJACK_FILTER_H
