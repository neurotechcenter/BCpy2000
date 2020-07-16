////////////////////////////////////////////////////////////////////////////////
// $Id: gEstimFilter.h 4536 2013-08-05 14:30:13Z mellinger $
// Author: kaleb.goering@gmail.com
// Description:  A filter to interact with gEstim device
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
#ifndef G_ESTIM_FILTER_H
#define G_ESTIM_FILTER_H

#include "GenericFilter.h"
#include "Expression/Expression.h"
#include "Thread.h"
#include "Mutex.h"
#include "gEstimAPI.imports.h"

#include <map>
#include <vector>

class gEstimFilter : public GenericFilter
{
public:
	// Constructor/Destructor
	gEstimFilter();
	~gEstimFilter();
	friend class gEstimThread;

 protected:
	// Virtual Interface
	void Publish() override;
	void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
	void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
	void Process( const GenericSignal& Input, GenericSignal& Output ) override;
	void StartRun() override;
	void StopRun() override;
	void Halt() override;

 private:
	// Private member methods
	void DisablePorts();
	// Private member variables
	PGDevice mEstim;
	bool mEstimActive;
	Expression mActivateExp;
	Expression mAbortExp;
	bool mStimulating;
	class gEstimThread : public Thread
	{
	public:
		gEstimThread( gEstimFilter* inFilt ) : mpFilter( inFilt ) {}
		virtual ~gEstimThread() {}
		virtual int OnExecute() override;
		LockableObject& GetLock() { return mDataLock; }
	private:
		Lockable<Mutex> mDataLock;
		gEstimFilter *mpFilter;
	} *mpgEstimThread;
};

#endif // G_ESTIM_FILTER_H
