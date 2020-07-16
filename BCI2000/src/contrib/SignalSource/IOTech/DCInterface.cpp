#include "DCInterface.h"
#include "common.h"

CDaqCOMInterface::CDaqCOMInterface(void)
{

}

CDaqCOMInterface::~CDaqCOMInterface()
{
    try
    {
        m_pSys->Close();
        m_pSys=NULL;
    }
    catch (_com_error &e)
    {
        dump_daqcom_error(e);
    }
}

BOOL CDaqCOMInterface::Initialize()
{
    OSYSDEBUG("CDaqCOMInterface::Initialize\n");
    long nDevices = 0;
    dcTRY

    ::CoInitialize(NULL);

    //Create DaqCOM objects.
    IDaqSystemPtr pSys(__uuidof(DaqSystem));

    m_pSys = pSys;
    m_pAcq = m_pSys->Add();
    m_pAcq->DataStore->BufferMode = dbmDataStore;
    m_pAcq->DataStore->AutoSizeBuffers = FALSE;
    m_pAcq->DataStore->BufferSizeInScans = 100000;
    m_pAcq->DataStore->IgnoreDataStoreOverruns = FALSE;
    m_pAcq->DataStore->IgnoreDeviceOverruns = FALSE;
    m_pAcq->DataStore->IgnoreDriverOverruns = FALSE;

    USES_CONVERSION;
    m_pSysDevs = m_pAcq->AvailableDevices;
    m_pConfig = m_pAcq->Config;

    nDevices = m_pAcq->AvailableDevices->Count;
    OSYSDEBUG("- %ld devices available.\n",nDevices);

    dcCATCH

    return nDevices > 0;
}

BOOL CDaqCOMInterface::OpenDevice()
{
    OSYSDEBUG("CDaqCOMInterface::OpenDevice\n");
    dcTRY
    long lastDeviceId = m_pAcq->AvailableDevices->Count;
    m_pDev = m_pSysDevs->CreateFromIndex(lastDeviceId); //This is our PDAQ3000
    m_pAIs = m_pDev->AnalogInputs;  //This holds the channels that we care about. (AI = Analog Input)
    m_Daq = m_pDev;
    m_Daq->Open();

    //setup idle interval
    long updateRate = (long)(1000.0/m_Rate);
    if (updateRate == 0)  updateRate = 1;
    m_pDev->IdleInterval = updateRate;

    //add Channel 0
    m_pAI = m_Daq->AnalogInputs->Add(aitDirect,dbcDaqChannel0,dmpPosition0);
    m_pChannel = m_Daq->AnalogInputs->GetItem(1)->Channels->GetItem(1);

    dcCATCH
    return TRUE;
}

/* 'Channels' is the list of channel numbers that we would like to use.  numChannel is the number of items in this list */
BOOL CDaqCOMInterface::SetupChannels(unsigned int * Channels, unsigned int numChannels)
{
    OSYSDEBUG("CDaqCOMInterface::SetupChannels\n");

    int i = 0;
    long cnt = 0;
    char str[48];

    dcTRY

    m_pAIs->RemoveAll();

    IAvailableBaseChannelsPtr m_pAvailableChannels = m_pDev->AvailableBaseChannels;

    IBaseChannelPtr pBaseChannel;

    for (i=0;i<16;i++) // PDAQ3000 (main device, PDQ30 is expansion module) has 16 'main' channels
    {
        pBaseChannel = m_pAvailableChannels->GetItem(1); // Every time we call this we get a new BaseChannel
        // We take that BaseChannel and we add it to our Analog Input list
        m_pAI = m_pAIs->Add(aitDirect,pBaseChannel->BaseChannel,(DeviceModulePosition)0);

        // Each analog input has a channel
        m_pChannel = (IpDaq3000DirectAIChannelPtr)m_pAI->Channels->GetItem(1);
        m_pChannel->EngrUnits = utVolts;
        m_pChannel->DifferentialMode = FALSE;

        if (bContains(Channels, numChannels, i+1))
        {
            m_pChannel->AddToScanList();
            sprintf(str,"CH%2d",i);
            OSYSDEBUG("%s (#%d), ", str, i+1);
        }
    }

    cnt = m_pAIs->Count;

    m_Daq->DetectExpansions();
    if (m_pAIs->Count > cnt) // if the count increased then we found a PDQ30
    {
        m_pAI = m_pAIs->GetItem(cnt+1); /* Gives new AI from expansion module each time we call it */
        long pdqCount = m_pAI->Channels->Count;
        for (i=0;i<pdqCount-6;i++) // subtract 6 because the last 6 channels are CJC which don't get a range
        {

            m_pPDQChannel = m_pAI->Channels->GetItem(i+1);
            if (m_pPDQChannel->IsAvailable)
            {
                m_pPDQChannel->EngrUnits = utVolts;
                m_pPDQChannel->DifferentialMode = false;
                sprintf(str,"PDQ%d",i);
                if (bContains(Channels, numChannels, i+17))
                {
                    m_pPDQChannel->AddToScanList();
                    OSYSDEBUG("%s (#%d), ", str, i+17);
                }
            }
        }
    }

    OSYSDEBUG("\n");
    OSYSDEBUG("- min scan rate: %f\n", m_pConfig->MinScanRate);
    OSYSDEBUG("- max scan rate: %f\n", m_pConfig->MaxScanRate);
    OSYSDEBUG("- scan rate: %f\n", m_Rate);
    OSYSDEBUG("- internal thread freq: %ld\n",m_pDev->IdleInterval);

    //Set the rate a which to collect the above scans.
    m_pConfig->ScanRate = m_Rate;

    //Specify the start/stop conditions.
    //Lets use a manual start and stop on a scan count.
    m_pAcq->Starts->GetItemByType((StartType)sttManual)->UseAsAcqStart();
    m_pAcq->Stops->GetItemByType((StopType)sptManual)->UseAsAcqStop();

    m_pScanList = m_pConfig->GetScanList();

    dcCATCH
    return TRUE;
}

BOOL CDaqCOMInterface::Start()
{
    OSYSDEBUG("CDaqCOMInterface::Start\n");
    dcTRY

    m_pAcq->Arm();      //Arm the acquistion
    m_pAcq->Start();    //Start data collection

    dcCATCH
    return TRUE;
}

BOOL CDaqCOMInterface::Stop()
{
    OSYSDEBUG("CDaqCOMInterface::Stop\n");
    dcTRY
    m_pAcq->Stop();
    m_pAcq->Disarm();
    dcCATCH
    return TRUE;
}

/* This pulls samples from the device over USB*/
long CDaqCOMInterface::FetchData(SAFEARRAY *psa)
{
    OSYSDEBUG("CDaqCOMInterface::FetchData\n");

    long retScans=0;
    dcTRY

    switch ((AcqCompletionStatus)m_pAcq->CompletionStatus)
    {
    case acsAborted:
        MessageBox(0, "ACQ Aborted!", "Error", 0);
        return FALSE;
    case acsComplete:
        break; //acquisition in process okay to proceed
    case acsPending:
        break;//acquisition in process okay to proceed
    case acsUserAborted:
        MessageBox(0, "ACQ Aborted by User!", "Error", 0);
        return FALSE;
    default:
        break;
    }

    // wait until we have enough scans
    while (m_pAcq->DataStore->AvailableScans < m_Scans)
        DoEvents();

    //get data
    retScans = m_pAcq->DataStore->FetchData(&psa, m_Scans);

    dcCATCH
    return retScans;
}

long CDaqCOMInterface::FlushData()
{
    OSYSDEBUG("CDaqCOMInterface::FlushData\n");
    long retScans = 0;
    dcTRY
    switch ((AcqCompletionStatus)m_pAcq->CompletionStatus)
    {
    case acsAborted:
        MessageBox(0, "ACQ Aborted!", "Error", 0);
        return FALSE;
    case acsComplete:
        break; //acquisition in process okay to proceed
    case acsPending:
        break;//acquisition in process okay to proceed
    case acsUserAborted:
        MessageBox(0, "ACQ Aborted by User!", "Error", 0);
        return FALSE;
    default:
        break;
    }

    //flush data
    retScans = m_pAcq->DataStore->FlushData(m_pAcq->DataStore->AvailableScans);

    dcCATCH
    return retScans;
}

