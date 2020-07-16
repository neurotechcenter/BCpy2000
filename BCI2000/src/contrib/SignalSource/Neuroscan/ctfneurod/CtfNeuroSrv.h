//////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:        CtfNeuroSrv.h
//
//  Author:      juergen.mellinger@uni-tuebingen.de
//
//  Date:        Jul 27, 2004
//
//  Description: A class implementing a Neuroscan Acquire forwarding daemon for CTF's
//               shared memory interface.
//
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CTFNEUROSRV_H
#define CTFNEUROSRV_H

#include "NeuroSrv.h"

#include <iostream>
#include <vector>
#include <regex.h>

class CtfNeuroSrv : public NeuroSrv
{
  static const double cMEGRoughGain = 3e15;  // Order of magnitude for product of Proper Gain and Q Gain.
  static const double cEEGRoughGain = 1.3e9; // Same for EEG channels.
  static const double cEEGUnitGain = 3e-2; // Pseudo-microvolts per MEG A/D unit: 33000 MEG AD units make up 1 mV.
  static const double cHPTimeConstant = 1.0; // High Pass time constant in seconds.
  static const int cBitsToIgnore = 2; // Number of least significant bits that will be ignored from the MEG data.

  public:
    CtfNeuroSrv( const regex_t* inChannelNameRegex = NULL,
                 double         inFreqCorrectionFactor = 1.0,
                 int            inDataOutputFormat = DataTypeRaw16bit,
                 bool           inDoHPFiltering = true,
                 std::ostream&  ioElocFile = std::cout,
                 std::ostream&  ioSourceParamFile = std::cout );
    virtual ~CtfNeuroSrv();

  protected:
    virtual void StartSendingData( std::ostream& );
    virtual void StopSendingData( std::ostream& );
    // This function looks for data and forwards them to the client.
    // The return value is an estimation of how many ms the process may safely sleep
    // before calling SendData() the next time.
    virtual int SendData( std::ostream& );
    virtual void SendASTSetupFile( std::ostream& );

  private:
    void ApplyHP( int channel, double& );

    std::string                 mDsPath;
    int                         mShmHandle,
                                mPacketIndex;
    struct ACQ_MessagePacket*   mpMessageQueue;
    std::vector<size_t>         mChannelIndices;
    typedef std::vector<double> NumVector;
    NumVector                   mChannelGains,
                                mHPChannelOffsets;
    double                      mDecayFactor;
    const regex_t*              mpChannelNameRegex;
    double                      mFreqCorrectionFactor;
    int                         mDataOutputFormat,
                                mBytesPerSample;
    bool                        mDoHPFiltering;
    std::ostream&               mrElocFile,
                &               mrSourceParamFile;
};

#endif // CTFNEUROSRV_H
