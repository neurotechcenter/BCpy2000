//////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:        NeuroSrv.cpp
//
//  Author:      juergen.mellinger@uni-tuebingen.de
//
//  Date:        Jul 27, 2004
//
//  Description: A class implementing a generic daemon/server for the Neuroscan Acquire
//               protocol.
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "NeuroSrv.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>

#include "SockStream.h"
#include "EDFHeader.h"

using namespace std;

void
NeuroSrv::CloseConnection( ostream& os )
{
  NscCloseRequest().Serialize( os );
  TerminateConnection();
}

void
NeuroSrv::SendBasicInfo( ostream& os )
{
  ostringstream oss;
  mBasicInfo.Serialize( oss );
  NscPacketHeader( 'DATA', DataType_InfoBlock, InfoType_BasicInfo, oss.str().size() ).Serialize( os );
  os.write( oss.str().data(), oss.str().size() ).flush();
}

void
NeuroSrv::SendEDFHeader( std::ostream& os )
{
  mEDFHeader.BlockDuration = ( 1.0 * mBasicInfo.SamplesInBlock() ) / mBasicInfo.SamplingRate();
  for( EDFHeader::ChannelList::iterator i = mChannelInfo.begin(); i != mChannelInfo.end(); ++i )
  {
#if 0
    if( i->PhysicalMaximum == i->PhysicalMinimum )
      cerr << i->Label << ": PhysicalMaximum == PhysicalMinimum == "
           << i->PhysicalMaximum << endl;
#endif
    i->DigitalMaximum = i->PhysicalMaximum / mBasicInfo.Resolution();
    i->DigitalMinimum = i->PhysicalMinimum / mBasicInfo.Resolution();
    i->SamplesPerRecord = mBasicInfo.SamplesInBlock();
    if( i->Filtering.empty() )
    {
      ostringstream oss;
      if( i->HighPass )
        oss << "HP:" << i->HighPass << "Hz ";
      if( i->LowPass )
        oss << "LP:" << i->LowPass << "Hz ";
      if( i->Notch )
        oss << "N:" << i->Notch << "Hz ";
      i->Filtering = oss.str();
    }
  }
  ostringstream oss;
  mEDFHeader.Serialize( oss );
  NscPacketHeader( 'DATA', DataType_InfoBlock, InfoType_EdfHeader, oss.str().size() ).Serialize( os );
  os.write( oss.str().data(), oss.str().size() ).flush();
}

void
NeuroSrv::SendASTSetupFile( std::ostream& os )
{
  NscPacketHeader( 'FILE', SetupFile, 0, 0 ).Serialize( os );
  os.flush();
}

int
NeuroSrv::Run( int argc, const char* argv[] )
{
  enum
  {
    noError = 0,
    errorOccurred,
  } result = noError;

  typedef void ( NeuroSrv::*MessageAction )( ostream& );
  map<NscPacketHeader, MessageAction> messageActions;
//  messageActions[NscVersionRequest()] = &NeuroSrv::SendVersion;
  messageActions[NscCloseRequest()] = &NeuroSrv::CloseConnection;
  messageActions[NscEDFHeaderRequest()] = &NeuroSrv::SendEDFHeader;
  messageActions[NscSetupFileRequest()] = &NeuroSrv::SendASTSetupFile;
  messageActions[NscInfoRequest()] = &NeuroSrv::SendBasicInfo;
  messageActions[NscStartDataRequest()] = &NeuroSrv::StartSendingData;
  messageActions[NscStopDataRequest()] = &NeuroSrv::StopSendingData;
  messageActions[NscStartAcquisition()] = &NeuroSrv::StartAcquisition;
  messageActions[NscStopAcquisition()] = &NeuroSrv::StopAcquisition;

  const char* address = "localhost:4000";
  if( argc > 1 )
    address = argv[ 1 ];

  while( result == noError )
  {
    sockstream client;
    server_tcpsocket serverSocket( address );
    if( serverSocket.is_open() )
      cout << "Listening on " << serverSocket.address() << "... " << flush;
    else
      cerr << "Could not open socket for listening on " << address << endl;
    serverSocket.wait_for_read( tcpsocket::infinite_timeout );
    client.open( serverSocket );
    if( client.is_open() )
      cout << "connected." << endl;
    mSendingData = false;
    mTerminatingConnection = false;
    NscPacketHeader header;
    while( client && !mTerminatingConnection )
    {
      while( client.rdbuf()->in_avail() && header.Unserialize( client ) )
      {
        MessageAction action = messageActions[header];
        if( action != NULL )
          ( this->*action )( client );
        else
        {
          cerr << argv[ 0 ] << ": received unhandled message(" << header << "), aborting" << endl;
          CloseConnection( client );
          result = errorOccurred;
        }
      }
      if( !mSendingData )
        mTerminatingConnection = !serverSocket.wait_for_read( tcpsocket::infinite_timeout );
      else
      {
        int timeout = SendData( client );
        serverSocket.wait_for_read( timeout );
        mTerminatingConnection = !serverSocket.is_open();
      }
    }
    cout << "Connection closed." << endl;
  }
  return result;
}


void
NeuroSrv::Sleep( int inMilliseconds )
{
  tcpsocket::sleep( inMilliseconds );
}
