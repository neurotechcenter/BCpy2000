#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include "nrvdate.h"

// {AF2B3281-7FCE-11d2-B2DE-00104B6FC652}
static const GUID SHORTSAMPLESGUID = 
{ 0xaf2b3281, 0x7fce, 0x11d2, { 0xb2, 0xde, 0x0, 0x10, 0x4b, 0x6f, 0xc6, 0x52 } };


// {89A091B3-972E-4da2-9266-261B186302A9}
static const GUID DELAYLINESAMPLESGUID = 
{ 0x89a091b3, 0x972e, 0x4da2, { 0x92, 0x66, 0x26, 0x1b, 0x18, 0x63, 0x2, 0xa9 } };


// {291E2381-B3B4-44d1-BB77-8CF5C24420D7}
static const GUID GENERALSAMPLESGUID = 
{ 0x291e2381, 0xb3b4, 0x44d1, { 0xbb, 0x77, 0x8c, 0xf5, 0xc2, 0x44, 0x20, 0xd7 } };

// {5F11C628-FCCC-4fdd-B429-5EC94CB3AFEB}
static const GUID FILTERSAMPLESGUID = 
{ 0x5f11c628, 0xfccc, 0x4fdd, { 0xb4, 0x29, 0x5e, 0xc9, 0x4c, 0xb3, 0xaf, 0xeb } };

// {728087F8-73E1-44d1-8882-C770976478A2}
static const GUID DATEXDATAGUID = 
{ 0x728087f8, 0x73e1, 0x44d1, { 0x88, 0x82, 0xc7, 0x70, 0x97, 0x64, 0x78, 0xa2 } };


/*
// {75B6A52A-18F3-11d3-A308-00105AD7ECD1}
static const GUID FLOATSAMPLESGUID = 
{ 0x75b6a52a, 0x18f3, 0x11d3, { 0xa3, 0x8, 0x0, 0x10, 0x5a, 0xd7, 0xec, 0xd1 } };
*/

// Data packet structure
   // GUID (GUID)
   // Size (LONGLONG);
   // TimeStamp (TIMESTAMP);
   // Number of series (LONG)
   // Rest of Data is variable in size.  It looks like this:
   //    Number of samples per serie (4*lcSeries bytes)
   //       Samples per serie 1
   //       ....
   //       Samples per serie N
   //    Data                        (Sum(i = 1..lcSeries:SampleCount(i) * sizeof(sample))
   //       Data1
   //       ....
   //       DataN

// Sizes
#define SIZE_DP_SUBGUID             GUID_SIZE
#define SIZE_DP_TIMESTAMP           sizeof(TIMESTAMP)
#define SIZE_DP_NROFSERIES          sizeof(LONG)
#define SIZE_DP_SERIESSAMPLESUNIT   sizeof(LONG)
#define SIZE_DP_DATASAMPLESUNIT     sizeof(SHORT)

// Offsets
#define OFFSET_DP_SUBGUID           OFFSET_DATA
#define OFFSET_DP_TIMESTAMP         OFFSET_DP_SUBGUID + SIZE_GUID
#define OFFSET_DP_NROFSERIES        (OFFSET_DP_TIMESTAMP + SIZE_DP_TIMESTAMP)
#define OFFSET_DP_SERIESSAMPLES     (OFFSET_DP_NROFSERIES + SIZE_DP_NROFSERIES)


#endif //__DATASTRUCTURES_H__
