#include_next <devicetopology.h>
#ifdef _KS_
#ifndef DEVICETOPOLOGY2_H__
#define DEVICETOPOLOGY2_H__

typedef enum __WIDL_devicetopology_generated_name_00000002 {
    eConnTypeUnknown = 0,
    eConnType3Point5mm = 1,
    eConnTypeQuarter = 2,
    eConnTypeAtapiInternal = 3,
    eConnTypeRCA = 4,
    eConnTypeOptical = 5,
    eConnTypeOtherDigital = 6,
    eConnTypeOtherAnalog = 7,
    eConnTypeMultichannelAnalogDIN = 8,
    eConnTypeXlrProfessional = 9,
    eConnTypeRJ11Modem = 10,
    eConnTypeCombination = 11
} EPcxConnectionType;
typedef enum __WIDL_devicetopology_generated_name_00000003 {
    eGeoLocRear = 1,
    eGeoLocFront = 2,
    eGeoLocLeft = 3,
    eGeoLocRight = 4,
    eGeoLocTop = 5,
    eGeoLocBottom = 6,
    eGeoLocRearPanel = 7,
    eGeoLocRiser = 8,
    eGeoLocInsideMobileLid = 9,
    eGeoLocDrivebay = 10,
    eGeoLocHDMI = 11,
    eGeoLocOutsideMobileLid = 12,
    eGeoLocATAPI = 13,
    eGeoLocReserved5 = 14,
    eGeoLocReserved6 = 15
} EPcxGeoLocation;
typedef enum __WIDL_devicetopology_generated_name_00000004 {
    eGenLocPrimaryBox = 0,
    eGenLocInternal = 1,
    eGenLocSeparate = 2,
    eGenLocOther = 3
} EPcxGenLocation;
typedef enum __WIDL_devicetopology_generated_name_00000005 {
    ePortConnJack = 0,
    ePortConnIntegratedDevice = 1,
    ePortConnBothIntegratedAndJack = 2,
    ePortConnUnknown = 3
} EPxcPortConnection;
typedef struct __WIDL_devicetopology_generated_name_00000006 {
    DWORD ChannelMapping;
    COLORREF Color;
    EPcxConnectionType ConnectionType;
    EPcxGeoLocation GeoLocation;
    EPcxGenLocation GenLocation;
    EPxcPortConnection PortConnection;
    WINBOOL IsConnected;
} KSJACK_DESCRIPTION;
typedef KSJACK_DESCRIPTION *PKSJACK_DESCRIPTION;
typedef enum __WIDL_devicetopology_generated_name_00000007 {
    KSJACK_SINK_CONNECTIONTYPE_HDMI = 0,
    KSJACK_SINK_CONNECTIONTYPE_DISPLAYPORT = 1
} KSJACK_SINK_CONNECTIONTYPE;
typedef struct _tagKSJACK_SINK_INFORMATION {
    KSJACK_SINK_CONNECTIONTYPE ConnType;
    WORD ManufacturerId;
    WORD ProductId;
    WORD AudioLatency;
    WINBOOL HDCPCapable;
    WINBOOL AICapable;
    UCHAR SinkDescriptionLength;
    WCHAR SinkDescription[32];
    LUID PortId;
} KSJACK_SINK_INFORMATION;
typedef struct _tagKSJACK_DESCRIPTION2 {
    DWORD DeviceStateInfo;
    DWORD JackCapabilities;
} KSJACK_DESCRIPTION2;
typedef struct _tagKSJACK_DESCRIPTION2 *PKSJACK_DESCRIPTION2;

#endif /* DEVICETOPOLOGY2_H__ */
#endif /* _KS_ */
