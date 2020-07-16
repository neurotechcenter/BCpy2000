#include <stdio.h>

#ifndef MicromedH
#define MicromedH

#pragma pack( push )
#pragma pack( 2 ) // set structure alignment to 2 bytes

  #define MICROMED_HEADER_SIZE  10  //remember to set structure alignment on 2 bytes!
  #define POS_NOTE        83072

  #define HEADER_TYPE   0
  #define DATA_TYPE   1
  #define NOTE_TYPE   2
  #define DIGTRIG_TYPE    3

  static const int cMmConnectionTimeout = 60000; // ms
  int MICROMED_PACKET_RATE;
  int    num_notes;
  unsigned long sampleNumber;
  FILE *hNotes;
  bool   waitforconn;

  class CAcqMessage
{
public:
    char  m_chId[4];    // ID string, no trailing '\0'
    WORD  m_wCode;    // Code
    DWORD m_dwSize;   // Body size (in bytes)
    char* m_pBody;    // Ptr to body data
    // Implementation
    int   GetHeaderSize() { return MICROMED_HEADER_SIZE; }  // Header portion includes m_chId,m_wCode,m_wRequest,m_dwSize
    BOOL  IsHeaderPacket () { return (m_wCode-HEADER_TYPE);}
    BOOL  IsDataPacket () { return (m_wCode-DATA_TYPE);}
    BOOL  IsNotePacket () { return (m_wCode-NOTE_TYPE);}
    BOOL  IsDigTrigPacket () { return (m_wCode-DIGTRIG_TYPE);}

// Construction
public:
    CAcqMessage()  { memset(m_chId,0,16); m_pBody = NULL;}
    CAcqMessage(char* id,WORD code,WORD request,DWORD size)
        { memcpy(m_chId,id,4);m_wCode=code;m_dwSize=size;m_pBody=NULL;}
    ~CAcqMessage() { if (m_pBody) delete [] m_pBody; m_pBody = NULL; }
  void  Convert(BOOL bSending)
  {
    if (bSending) {
      // Convert from host byte order to network  byte order (Little-Endian to Big-Endian)
      m_wCode=htons(m_wCode);m_dwSize=htonl(m_dwSize);
    } else {
      // Convert from network byte order to host byte order (Big-Endian to Little-Endian)
      m_wCode=ntohs(m_wCode);m_dwSize=ntohl(m_dwSize);
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

#pragma pack( pop ) // undo structure alignment setting

#endif
