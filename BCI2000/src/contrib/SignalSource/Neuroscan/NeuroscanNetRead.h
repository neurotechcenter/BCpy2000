#ifndef NEUROSCANH
#define NEUROSCANH

#define NS_HEADER_SIZE  12

// "CTRL" packet control codes
enum { GeneralControlCode=1, ServerControlCode, ClientControlCode };
// "CTRL" packet requests
enum { RequestForVersion=1, ClosingUp, ServerDisconnected };
enum { StartAcquisition=1, StopAcquisition, StartImpedance, ChangeSetup, DCCorrection };
enum { RequestEdfHeader=1, RequestAstFile, RequestStartData, RequestStopData, RequestBasicInfo };
// "DATA" packet codes and requests
enum { DataType_InfoBlock=1, DataType_EegData };
enum { InfoType_Version=1, InfoType_EdfHeader, InfoType_BasicInfo };
enum { DataTypeRaw16bit=1, DataTypeRaw32bit };

class CAcqMessage
{
public:
    char  m_chId[4];    // ID string, no trailing '\0'
    WORD  m_wCode;    // Code
    WORD  m_wRequest;   // Request
    DWORD m_dwSize;   // Body size (in bytes)
    char* m_pBody;    // Ptr to body data
    // Implementation
    int   GetHeaderSize() { return 12; }  // Header portion includes m_chId,m_wCode,m_wRequest,m_dwSize
    BOOL  IsCtrlPacket () { return (!memcmp(m_chId,"CTRL",4));}
    BOOL  IsDataPacket () { return (!memcmp(m_chId,"DATA",4));}

// Construction
public:
    CAcqMessage()  { memset(m_chId,0,16); m_pBody = NULL;}
    CAcqMessage(char* id,WORD code,WORD request,DWORD size)
        { memcpy(m_chId,id,4);m_wCode=code;m_wRequest=request;m_dwSize=size;m_pBody=NULL;}
    ~CAcqMessage() { if (m_pBody) delete [] m_pBody; m_pBody = NULL; }
  void  Convert(BOOL bSending)
  {
    if (bSending) {
      // Convert from host byte order to network  byte order (Little-Endian to Big-Endian)
      m_wCode=htons(m_wCode);m_wRequest=htons(m_wRequest);m_dwSize=htonl(m_dwSize);
    } else {
      // Convert from network byte order to host byte order (Big-Endian to Little-Endian)
      m_wCode=ntohs(m_wCode);m_wRequest=ntohs(m_wRequest);m_dwSize=ntohl(m_dwSize);
    }
  }
};

class AcqBasicInfo
 {
 public:

 DWORD  dwSize;           // Size of structure, used for version control
 int  nEegChan;   // Number of EEG channels
 int  nEvtChan;   // Number of event channels
 int  nBlockPnts;   // Samples in block
 int  nRate;      // Sampling rate (in Hz)
 int  nDataSize;    // 2 for "short", 4 for "int" type of data
 float  fResolution;          // Resolution for LSB
 //
 AcqBasicInfo()  { Init(); }
 void Init()     { memset((void*)&dwSize, 0, sizeof(AcqBasicInfo));}
 BOOL Validate() { return (nBlockPnts && nRate && nEegChan && nDataSize); }
 };

#endif
