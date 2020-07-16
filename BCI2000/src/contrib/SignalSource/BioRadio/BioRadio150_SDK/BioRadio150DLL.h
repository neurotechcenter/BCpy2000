//---------------------------------------------------------------------------
#ifndef TUpdateStatusEvent
  typedef void UpdateStatusFunc(char *Message, bool *Abort);
  typedef UpdateStatusFunc* TUpdateStatusEvent;
#endif
//---------------------------------------------------------------------------
#ifndef TDeviceInfoStruct
#define TDeviceInfoStruct
struct TDeviceInfo{
  char PortName[260];
};
#endif
//---------------------------------------------------------------------------
#ifndef _BIORADIO_150_DLL_H_
#define _BIORADIO_150_DLL_H_
#include "BioRadioConfig.h"
#define DllImport __declspec(dllimport)
//---------------------------------------------------------------------------
extern "C" {
DllImport BOOL _stdcall FindDevices(TDeviceInfo *DeviceList, int *DeviceCount, int MaxCount, TUpdateStatusEvent UpdateStatusProc);
DllImport BOOL _stdcall FindDevicesSimple(char *DevicePortNameList[], int *DeviceCount, int MaxDeviceCount, int PortNameStrLen);
DllImport unsigned int _stdcall CreateBioRadio();
DllImport int _stdcall DestroyBioRadio(unsigned int BioRadio150);
DllImport int _stdcall StartCommunication(unsigned int BioRadio150 , char *PortName);
DllImport int _stdcall StartAcq(unsigned int BioRadio150, char DisplayProgressDialog);
DllImport int _stdcall StopAcq(unsigned int BioRadio150);
DllImport int _stdcall DisableBuffering(unsigned int BioRadio150);
DllImport int _stdcall EnableBuffering(unsigned int BioRadio150);
DllImport int _stdcall StopCommunication(unsigned int BioRadio150);
DllImport int _stdcall LoadConfig(unsigned int BioRadio150, char *Filename);
DllImport int _stdcall SaveConfig(unsigned int BioRadio150, char *Filename);
DllImport int _stdcall PingConfig(unsigned int BioRadio150, char DisplayProgressDialog);
DllImport int _stdcall GetConfig(unsigned int BioRadio150, TBioRadioConfig *ConfigStruct, char PingDevice, char DisplayProgressDialog);
DllImport int _stdcall ProgramConfig(unsigned int BioRadio150, char DisplayProgressDialog, const char *Filename);
DllImport int _stdcall SetConfig(unsigned int BioRadio150, TBioRadioConfig *ConfigStruct, char ProgramDevice, char DisplayProgressDialog);
DllImport long _stdcall GetSampleRate(unsigned int BioRadio150);
DllImport long _stdcall GetBitResolution(unsigned int BioRadio150);
DllImport int _stdcall GetFastSweepsPerPacket(unsigned int BioRadio150);
DllImport int _stdcall GetSlowSweepsPerPacket(unsigned int BioRadio150);
DllImport int _stdcall GetNumEnabledInputs(unsigned int BioRadio150, int *FastMainInputs, int *FastAuxInputs, int *SlowAuxInputs);
DllImport int _stdcall GetNumEnabledSlowInputs(unsigned int BioRadio150);
DllImport int _stdcall GetNumEnabledFastInputs(unsigned int BioRadio150);
DllImport long _stdcall GetMode(unsigned int BioRadio150, WORD &Mode);
DllImport long _stdcall GetNumChannels(unsigned int BioRadio150);
DllImport WORD _stdcall GetEnabledFastInputs(unsigned int BioRadio150);
DllImport char _stdcall GetEnabledSlowInputs(unsigned int BioRadio150);
DllImport int _stdcall TransferBuffer(unsigned int BioRadio150);
DllImport int _stdcall ReadScaledData(unsigned int BioRadio150, double *Data, int Size, int *NumRead);
DllImport int _stdcall ReadRawData(unsigned int BioRadio150, WORD *Data, int Size, int *NumRead);
DllImport int _stdcall ReadScaledFastAndSlowData(unsigned int BioRadio150, double *FastInputsData, int FastInputsSize, int *FastInputsNumRead, WORD *SlowInputsData, int SlowInputsSize, int *SlowInputsNumRead);
DllImport int _stdcall ReadRawFastAndSlowData(unsigned int BioRadio150, WORD *FastInputsData, int FastInputsSize, int *FastInputsNumRead, WORD *SlowInputsData, int SlowInputsSize, int *SlowInputsNumRead);
DllImport int _stdcall SetBadDataValues(unsigned int BioRadio150, double BadDataValueScaled, WORD BadDataValueRaw);
DllImport int _stdcall SetPadWait(unsigned int BioRadio150, int numMissingPackets);
DllImport int _stdcall GetRFChannel(unsigned int BioRadio150);
DllImport int _stdcall SetRFChannel(unsigned int BioRadio150, int RFChannel);
DllImport int _stdcall GetUsableRFChannelList(int *UsableRFChannelList, int Size);
DllImport int _stdcall GetFreqHoppingMode(unsigned int BioRadio150);
DllImport int _stdcall GetFreqHoppingModeIndicator();
DllImport int _stdcall SetFreqHoppingMode(unsigned int BioRadio150, bool HoppingEnabled);
DllImport unsigned int _stdcall GetGoodPackets(unsigned int BioRadio150);
DllImport unsigned int _stdcall GetBadPackets(unsigned int BioRadio150);
DllImport unsigned int _stdcall GetDroppedPackets(unsigned int BioRadio150);
DllImport int _stdcall GetUpRSSI(unsigned int BioRadio150);
DllImport int _stdcall GetDownRSSI(unsigned int BioRadio150);
DllImport int _stdcall GetLinkBufferSize(unsigned int BioRadio150);
DllImport int _stdcall GetBitErrCount(unsigned int BioRadio150);
DllImport int _stdcall GetBitErrRate(unsigned int BioRadio150);
DllImport double _stdcall GetBatteryStatus(unsigned int BioRadio150);
DllImport int _stdcall GetBioRadioModelString(unsigned int BioRadio150, char *BioRadioModelString, int StrLength);
DllImport int _stdcall GetDeviceID(unsigned int BioRadio150, WORD *DeviceID);
DllImport int _stdcall GetDeviceIDString(unsigned int BioRadio150, char *DeviceIDCStr, int StrLength);
DllImport int _stdcall GetFirmwareVersionString(unsigned int BioRadio150, char *VersionString, int StrLength);
DllImport void _stdcall GetDLLVersionString(char *VersionString, int StrLength);
}
//---------------------------------------------------------------------------
#endif
