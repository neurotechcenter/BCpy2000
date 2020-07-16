/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
/*/
#ifndef DTADCH
#define DTADCH

#include <windows.h>
#include <olmem.h>
#include <olerrors.h>
#include <oldaapi.h>

#include "dtfun.h"
#include "GenericADC.h"
#include "GenericSignal.h"


class DTADC : public GenericADC
{
 public:
               DTADC();
  virtual      ~DTADC();

  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual void Halt();

 private:
  int   samplerate;
  int   blocksize;
  int   channels;
  int   SleepTime;
  // DBL   dGain;
  // int   ClkSource;
  // DBL   dfFreq;
  // UINT  Bufferpts;
  int   StartFlag;
  bool  Board2Active;
  int   channelsb1, channelsb2;
};

#endif // DTADCH
