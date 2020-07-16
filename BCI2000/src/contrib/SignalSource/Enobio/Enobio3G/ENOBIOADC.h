////////////////////////////////////////////////////////////////////////////////
// $Id: ENOBIOADC.h 4755 2014-10-15 20:33:55Z mellinger $
// Author:masayo.haneda@starlab.es
// Description: BCI2000 Source Module for ENOBIO devices.
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2011: BCI2000 team and many external contributors ]
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
#ifndef ENOBIO_ADCH
#define ENOBIO_ADCH

#include "GenericADC.h"
#include "lib/Enobio3G.h"
#include "EnobioStatusConsumer.h"
#include "EnobioDataConsumer.h"
class Enobio_ADC : public GenericADC
{
public:
	Enobio_ADC();
	~Enobio_ADC();
	void Preflight( const SignalProperties&, SignalProperties& ) const;
	void Initialize( const SignalProperties&, const SignalProperties& );
	void Process( const GenericSignal&, GenericSignal& );
	void Halt();
	
	
protected:
	int* mpSignalBuffer;
	int mSourceCh;
	int mSampleBlockSize; // sample blocksize to send
	mutable Enobio3G mEnobio;
	mutable EnobioStatusConsumer statusHandler;
	mutable EnobioDataConsumer dataHandler;
	
};

#endif // ENOBIOADCH

