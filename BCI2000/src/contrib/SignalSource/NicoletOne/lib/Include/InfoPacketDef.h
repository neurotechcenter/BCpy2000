#ifndef __INFOPACKETDEF_H__
#define __INFOPACKETDEF_H__

#include <InfoPacket.h>
#include <InfoStructures.h>

class IPACCINFO      : public CSimpleInfoPacket<ACCINFOHEADER> { };
class IPPAGEINFO     : public CSimpleInfoPacket<PAGEHEADER> { };
class IPRECCTRL      : public CSimpleInfoPacket<RECCTRL> { };
class IPHWINFO       : public CSimpleInfoPacket<HWHEADER> { };

class IPPHOTICINFO   : public CInfoPacket<NOINFO,                    PHOTICINFO> { };
class IPFOURIER      : public CInfoPacket<FOURIERSETTINGSHEADER,     POWERBAND> { };
class IPAUDIO        : public CInfoPacket<NOINFO,                    AUDIOINFO> { };
class IPDERIVATION   : public CInfoPacket<NOINFO,                    DERIVATIONINFO> { };
class IPFILTER       : public CInfoPacket<NOINFO,                    FILTERINFO> { };
class IPDISPLAY      : public CInfoPacket<NOINFO,                    DISPLAYINFO> { };
class IPSRINFO       : public CInfoPacket<NOINFO,                    SRINFO> { };
class IPVIDEOSYNC    : public CInfoPacket<NOINFO,                    SYNCINFO> { };
class IPFILEINFO     : public CInfoPacket<FILEINFOHEADER,            FILEINFO> { };

#endif //__INFOPACKETDEF_H__
