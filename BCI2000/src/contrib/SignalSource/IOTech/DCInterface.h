#ifndef DCINTERFACE_H_INCLUDED
#define DCINTERFACE_H_INCLUDED

#include "DCHelpers.h"

class CDaqCOMInterface
{
public:
    CDaqCOMInterface();
    ~CDaqCOMInterface();

    BOOL Initialize();
    BOOL OpenDevice();
    BOOL SetupChannels(unsigned int *, unsigned int);
    BOOL Start(void);
    BOOL Stop(void);
    long FetchData(SAFEARRAY *psa);
    long FlushData();

public:
    float   m_Rate;
    long    m_Scans;

private:
    IDaqSystemPtr                   m_pSys;
    IAvailableDevicesPtr            m_pSysDevs;
    IDevicePtr                      m_pDev;
    IDaq3xxxPtr                     m_Daq;
    IAcqPtr                         m_pAcq;
    IConfigPtr                      m_pConfig;
    IScanListPtr                    m_pScanList;
    IAnalogInputPtr                 m_pAI;
    IAnalogInputsPtr                m_pAIs;
    IPDQ30ChannelPtr                m_pPDQChannel;
    IpDaq3000DirectAIChannelPtr     m_pChannel;
};

#endif // DCINTERFACE_H_INCLUDED
