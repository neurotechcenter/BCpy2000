////////////////////////////////////////////////////////////////////////////////
// $Id: NeuroscanADC.h 4557 2013-08-29 16:52:31Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A source module connecting to a Neuroscan Acquire server.
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
#ifndef NeuroscanADCH
#define NeuroscanADCH

#include "GenericADC.h"
#include "SockStream.h"
#include "NeuroscanProtocol.h"

class NeuroscanADC : public GenericADC
{
 public:
  NeuroscanADC();
  ~NeuroscanADC();

  void Publish();
  void AutoConfig( const SignalProperties& );
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void Process( const GenericSignal&, GenericSignal& );
  void Halt();

 private:
  void AwaitResponse();
  template<typename T> void ReadData( GenericSignal& );
 
  client_tcpsocket mSocket;
  sockstream mServer;
  NscBasicInfo mAcqSettings;
  int mEventChannels;
};

#endif // NeuroscanADCH
