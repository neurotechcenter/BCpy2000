#ifndef VHT_TRACKER_DATA_H
#define VHT_TRACKER_DATA_H

/********************************************************************
 FILE: $Id: vhtTrackerData.h,v 1.4 2000/06/21 22:12:47 ullrich Exp $
 AUTHOR: 
 DATE: 

 DESCRIPTION:

 HISTORY:
 
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

//: Tracker data structure.
// A generic tracker data container.
//!vhtmodule: Device
class vhtTrackerData
{
 public:
    double position[1024]; //: Data buffer.
};

#endif


