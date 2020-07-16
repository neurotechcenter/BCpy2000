//////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:        NeuroSrv.h
//
//  Author:      juergen.mellinger@uni-tuebingen.de
//
//  Date:        Jul 27, 2004
//
//  Description: A class implementing a generic daemon/server for the Neuroscan Acquire
//               protocol.
//
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef NEUROSRV_H
#define NEUROSRV_H

#include "NeuroscanProtocol.h"
#include "EDFHeader.h"
#include <iostream>
#include <vector>
#include <string>

class NeuroSrv
{
  public:
    NeuroSrv() : mSendingData( false ), mTerminatingConnection( false ), mChannelInfo( mEDFHeader.Channels ) {}
    virtual ~NeuroSrv() {}
    int Run( int argc, const char** argv );

  protected:
    // This virtual interface corresponds to the messages that the server responds to.
    // A descendant may hook in by overriding certain functions.
    virtual void SendVersion( std::ostream& )      { /* not yet */ }
    virtual void CloseConnection( std::ostream& );
    virtual void SendEDFHeader( std::ostream& );
    virtual void SendASTSetupFile( std::ostream& );
    virtual void SendBasicInfo( std::ostream& );
    virtual void StartAcquisition( std::ostream& ) {}
    virtual void StopAcquisition( std::ostream& )  {}
    virtual void StartSendingData( std::ostream& ) { mSendingData = true; }
    virtual void StopSendingData( std::ostream& )  { mSendingData = false; }

    // This function looks for data and forwards them to the client.
    // It must be implemented by a descendant.
    // The return value is an estimation of how many ms the process may safely sleep
    // before calling SendData() the next time.
    virtual int  SendData( std::ostream& ) = 0;

  protected:
    void TerminateConnection() { mTerminatingConnection = true; }
    void Sleep( int milliseconds );

  protected:
    NscBasicInfo mBasicInfo;
    EDFHeader::ChannelList& mChannelInfo;

  private:
    EDFHeader mEDFHeader;
    // Program state:
    bool mSendingData,
         mTerminatingConnection;
};

#endif // NEUROSRV_H
