#include "stdafx.h"
#include "LiveData.h"
#include "BCIStream.h"
#include <cassert>

// Abandon all hope, ye who enter here.

CLiveData::CLiveData()
: m_bNewTsInfo( false ),
  m_bTsInfoReceived( false ),
  m_bDataReceived( false ),
  m_bLogging( false ),
  mDataLock( NULL )
{
   m_nrvdateFirst.dDate = -1;
   mDataLock = new CLock( "Data" );
}

CLiveData::~CLiveData()
{
	if( mDataLock ) delete mDataLock;
	mDataLock = NULL;
}

bool CLiveData::ConnectToComputer(BSTR wcComputer, long lPort)
{
   m_pcSocket->SetReceiver(this);
   if( m_pcSocket->ConnectToSocket(wcComputer, lPort) == E_FAIL )
	   return true;
   return false;
}

void CLiveData::ReceivePacket(PACKET pPacket)
{
   mDataLock->Lock();
   bool bChanged = false;
   //*************************************************************************************
   //The m_cTSInfo, m_cSensorInfo, m_cRecordingHelper, m_cSignalInfo packets should all
   //arrive before the first m_cData packet. There is no check for that here
   //but it should never happen
   //***********************************************************************************
   if (m_cData.SetPacket(pPacket))
   {
      _ASSERTE(m_cData.GetSerieCount() > 0);
      _ASSERTE(m_cSensorInfo.IsAllocated());
      _ASSERTE(m_cTSInfo.IsAllocated());
      _ASSERTE(m_cSignalInfo.IsAllocated());
      if (m_nrvdateFirst.dDate < 0)
         m_nrvdateFirst = m_cData.GetTimeStamp().nrvStamp;
	  // Do something with the Data Packet
	  _GrabData();
	  m_bDataReceived = true;
   }
   if (m_cTSInfo.SetPacketPermanent(pPacket))
   {
      long lcChannels = m_cTSInfo.GetElementCount();
      _ASSERTE(lcChannels > 0);
      m_bNewTsInfo = true;
	  m_bTsInfoReceived = true;
   }
   if (m_cSignalInfo.SetPacketPermanent(pPacket))
   {
      long lcSignals = m_cSignalInfo.GetElementCount();
      _ASSERTE(lcSignals > 0);
   }
   if (m_cSensorInfo.SetPacketPermanent(pPacket))
   {
      long lcSensors = m_cSensorInfo.GetElementCount();
      _ASSERTE(lcSensors > 0);
   }
   if (m_cRecordingHelper.SetPacket(pPacket))
   {
       bool bIsRunning = m_cRecordingHelper.IsRunning();
   }
   ULONG ul = static_cast<ULONG>(GETPACKETSIZE(pPacket));
   mDataLock->Unlock();
}

// CLiveData::_GrabData()
// Grabs the data and packs it into a buffered map of FIFO queues
void CLiveData::_GrabData()
{
   if( m_bLogging )
   {
      // For each dataserie or for each active sensor in segment liDataSegment
      int s_count = m_cData.GetSampleCount( 0 );
      for (long liDataSerie = 0; liDataSerie < m_cData.GetSerieCount(); liDataSerie++)
      {
         short* pData = m_cData[liDataSerie];
		 assert( s_count == m_cData.GetSampleCount(liDataSerie) );
		 double dResolution = m_cTSInfo[liDataSerie].dResolution;
         double dOffset = m_cTSInfo[liDataSerie].dOffset;

         // For each sample in sensor
         for (long liSample = 0; liSample < m_cData.GetSampleCount(liDataSerie); liSample++)
         {
            // 's' is the datagram from channel 'liDataSerie' and sample 'liSample' in segment 'liDataSegment'
            short s = *pData++;
    		    mCurData[liDataSerie].push( static_cast<float>( s * dResolution + dOffset ) );
         }
			 
      }
	  //if( PrecisionTime::TimeDiff( mStartTime, PrecisionTime::Now() ) < 1000 )
	  //  mCount += s_count;
	  //else
	  //  mCount ++;
   }
}

// Start logging data from the device
void CLiveData::StartLogging()
{
    mDataLock->Lock();
	m_bLogging = true;
	mStartTime = PrecisionTime::Now();
	mCount = 0;
	mDataLock->Unlock();
}

// Stop logging data from the device
void CLiveData::StopLogging()
{
	mDataLock->Lock();
	m_bLogging = false;

	// Clear any buffered data
	mCurData.clear();
	mDataLock->Unlock();
}

bool CLiveData::GetData( int ChannelSize, int DataBlockSize, float** DataBlock, bool &newTSInfo )
{
	mDataLock->Lock();
	// Return false if we're not currently logging
	if( !m_bLogging )
	{
		mDataLock->Unlock();
		return true;
	}
	// Return false if we don't have any data for this channel
	if( static_cast<int>( mCurData[0].size() ) < DataBlockSize )
	{
		mDataLock->Unlock();
		return true;
	}

	// Else, fill the data block
	for( int i = 0; i < ChannelSize; i++ )
	{
		for( int j = 0; j < DataBlockSize; j++ )
		{
			DataBlock[i][j] = mCurData[i].front();
			mCurData[i].pop();
		}
	}

	// Return if we have new TSInfo
	newTSInfo = m_bNewTsInfo;
	if( m_bNewTsInfo ) m_bNewTsInfo = false;
	mDataLock->Unlock();

	return false;
}

void CLiveData::ClearBufferedData()
{
	// Clear any buffered data
	mDataLock->Lock();
	mCurData.clear();
	mDataLock->Unlock();
}

// Gets the current sample rate from the device
bool CLiveData::GetSampleRate( double *rate )
{
	mDataLock->Lock();
	if( !m_bTsInfoReceived )
	{
		mDataLock->Unlock();
		return true;
	}

	// Grab the sample rate from the TSInfo
	*rate = m_cTSInfo.GetMaxSamplingRate();
	mDataLock->Unlock();
	return false;
}

// Gets the number of channels from the device
bool CLiveData::GetNumChannels( int *channels )
{
	mDataLock->Lock();
	if( !m_bTsInfoReceived )
	{
		mDataLock->Unlock();
		return true;
	}

	// Grab the number of channels from the TSInfo Packet
	*channels = m_cTSInfo.GetElementCount();
	mDataLock->Unlock();
	return false;
}

// Gets SourceChOffset List
bool CLiveData::GetDCOffset( double*& offset )
{
	mDataLock->Lock();
	// Offset is a float array of size[channels]
	if( !m_bTsInfoReceived )
	{
		mDataLock->Unlock();
		return true;
	}

	// Size the float array accordingly
	int numChannels = m_cTSInfo.GetElementCount();
	if( offset )
		delete offset;
	offset = NULL;
	offset = new double[ numChannels ];

	// Populate the array with the offsets
	for( int i = 0; i < numChannels; i++ )
		offset[i] = m_cTSInfo[i].dOffset;
	mDataLock->Unlock();
	return false;
}

// Gets SourceChGain List
bool CLiveData::GetDCResolution( double*& resolution )
{
	mDataLock->Lock();
	// Resolution is a float array of size[channels]
	if( !m_bTsInfoReceived )
	{
		mDataLock->Unlock();
		return true;
	}

	// Size the float array accordingly
	int numChannels = m_cTSInfo.GetElementCount();
	if( resolution )
		delete resolution;
	resolution = NULL;
	resolution = new double[ numChannels ];
		
	// Populate the array with the resolutions
	for( int i = 0; i < numChannels; i++ )
		resolution[i] = m_cTSInfo[i].dResolution;
	mDataLock->Unlock();
	return false;
}




