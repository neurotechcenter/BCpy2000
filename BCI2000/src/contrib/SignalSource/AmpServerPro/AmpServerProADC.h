////////////////////////////////////////////////////////////////////////////////
// $Id: AmpServerProADC.h 4653 2013-11-22 18:55:50Z mellinger $
// Description: A source module that interfaces withEGI's Amp Server Pro SDK.
// Authors: Joshua Fialkoff, Robert Bell (EGI),
//          juergen.mellinger@uni-tuebingen.de
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
#ifndef AmpServerProADCH
#define AmpServerProADCH

#include "BufferedADC.h"
#include "SockStream.h"

class AmpServerProADC : public BufferedADC
{
 public:
  AmpServerProADC();
  ~AmpServerProADC();

 protected:
  void Publish();
  void OnAutoConfig();
  void OnPreflight( SignalProperties& ) const;
  void OnInitialize( const SignalProperties& );
  void OnStartAcquisition();
  void OnStopAcquisition();
  void DoAcquire( GenericSignal& );

 private:
  class Connection
  {
   public:
    Connection();
    ~Connection();

    const std::string& Error() const
    { return mError; }
    bool Open( const std::string& inIP, int inPort, int inAmpId );
    void Close();

    bool IsOpen() const;
    const std::string& Address() const
    { return mAddress; }
    int NotificationPort() const
    { return mNotificationPort; }
    int StreamPort() const
    { return mStreamPort; }

    int Amps() const
    { return mAmps; }
    int Channels() const
    { return mChannels; }
    int SampleBlockSize() const
    { return mSampleBlockSize; }
    double Gain_muV() const
    { return mGain_muV; }
    double SamplingRate() const
    { return mSamplingRate; }

    bool SendCommand( const std::string&, std::string& resultName = std::string(), std::string& resultValue = std::string() );
    bool StartStreaming();
    bool StopStreaming();
    bool ReadData( GenericSignal& );

  private:
    std::string BuildCommand( const std::string& ) const;
    bool DoRead2( GenericSignal& );
    bool DoRead( GenericSignal& );

    std::string mError;

    int64_t mSamplesInStream;
    int mSamplesInOutput;
    bool mFirstRead;

    int mTimeout, mInitialTimeout;

    std::string mAddress;
    int mNotificationPort, mStreamPort, mAmpId;
    client_tcpsocket mCommandSocket, mStreamSocket, mNotificationSocket;
    sockstream mCommands, mStream, mNotifications;

    int mAmps, mChannels, mSampleBlockSize;
    double mGain_muV, mSamplingRate;

  } mConnection;
};

#endif // AmpServerProADCH

