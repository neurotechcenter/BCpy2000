#ifndef __PACKETDEF_H__
#define __PACKETDEF_H__

typedef BYTE* PACKET;

struct PACKETHEADER
{
   GUID guid;
   ULONGLONG llSize;
};

// Sizes
#define SIZE_GUID             sizeof(GUID)
#define SIZE_SIZE             sizeof(ULONGLONG)

// Offsets
#define OFFSET_GUID           0
#define OFFSET_SIZE           SIZE_GUID
#define OFFSET_DATA           (sizeof(PACKETHEADER))

#define GETPACKETGUID(x) (((PACKETHEADER*)(x))->guid)
#define GETPACKETSIZE(x) (((PACKETHEADER*)(x))->llSize)

#endif //__PACKETDEF_H__
