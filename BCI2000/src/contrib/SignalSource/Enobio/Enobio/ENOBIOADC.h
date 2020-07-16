////////////////////////////////////////////////////////////////////////////////
// $Id: ENOBIOADC.h 4536 2013-08-05 14:30:13Z mellinger $
// Author:masayo.haneda@starlab.es
// Description: BCI2000 Source Module for ENOBIO devices.
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
#ifndef ENOBIOADCH
#define ENOBIOADCH

#include "GenericADC.h"
#include "lib/EnobioDLL.imports.h"

class ENOBIOADC : public GenericADC
{
public:
	ENOBIOADC();
	~ENOBIOADC();
	void Preflight( const SignalProperties&, SignalProperties& ) const;
	void Initialize( const SignalProperties&, const SignalProperties& );
	void Process( const GenericSignal&, GenericSignal& );
	void Halt();
protected:
	int* mSignalBuffer;
	int mSourceCh;
	int mSampleBlockSize; // sample blocksize to send
	mutable Enobio* mEnobio;
};

#endif // ENOBIOADCH

