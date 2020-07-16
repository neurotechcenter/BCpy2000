/*
 * Program:   Biosemi2ADC
 * Module:    Biosemi2ADC.CPP

 * Version:   0.01
 * Liecense:
 * Copyright (C) 2005 Samuel A. Inverso (samuel.inverso@gmail.com), Yang Zhen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *
 * Revisions:
 *  Revision 1.1  2005/12/12 00:05:24  sinverso
 *  Initial Revision: Working and tested offline. Not tested in real experiments.
 *
 *  Revision 1.2  2005/12/14 15:24:15  mellinger
 *  Fixed state name typo in Process().
 *
 *  Revision 1.3 2008/10/25  Maria Laura  Blefari
 *  Fixed battery warning
 *
 *  Revison 2.0 2008/11/07  Jeremy Hill
 *  Updated design, including support for an auxiliary Analog Input Box (AIB)
 *  EEG + AIB acquisition tested---triggers not.
 *
 *  Revison 2.1 2009/05/27  Jeremy Hill
 *  Trigger signal acquisition fixed and tested.
 *  Added option to acquire trigger signals simultaneously in one 16-bit channel.
 */
#ifndef Biosemi2ADCH
#define Biosemi2ADCH

#include <vector>
#include <string>
#include "GenericADC.h"
#include "PrecisionTime.h"
#include "Biosemi2Client.h"

class Biosemi2ADC : public GenericADC
{
public:
                  Biosemi2ADC();
    virtual      ~Biosemi2ADC();

    virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
    virtual void Initialize( const SignalProperties&, const SignalProperties& );
    virtual void Process( const GenericSignal&, GenericSignal& );
    virtual void Halt();

protected:
    static const int TRIGGER_HIGH = 1;
    static const int TRIGGER_LOW = 0;
    static const int BATTERY_LOW = true;
    static const int BATTERY_NOT_LOW = false;

    int mSamplingRate;
    int mSourceCh;
    int * mChInd;

    int mSampleBlockSize; // sample blocksize to send
    Biosemi2Client* mpBiosemi;

    Biosemi2Client::DataBlock *mpDataBlock;
    std::vector<std::string> mTriggerNames;
};

#endif // Biosemi2ADCH

