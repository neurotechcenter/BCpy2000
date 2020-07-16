////////////////////////////////////////////////////////////////////////////////
// $Id: BR150.h 4373 2013-02-25 13:21:16Z mellinger $
// $Log$
// Revision 1.2  2006/07/05 15:21:19  mellinger
// Formatting and naming changes.
//
// Revision 1.1  2006/07/04 18:44:25  mellinger
// Put files into CVS.
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef BR150_H
#define BR150_H

#include "BR_defines.h"
#include <string>
#include <Win32Defs.h>

class BR150
{
 public:
                //Constructor
                BR150();
                //Destructor
                ~BR150();
                // starts the bioRadio communication
        int     Start(const char*);
                // kills communications, destroys radio object
        int     Stop(void);
                // purges internal buffer
        void    Purge(void);
                // Returns the number of samples read
        int     SamplesRead(void) const;
                // Get the running state of bioradio
  static bool   GetState(void);
                // gets pointer of filled buffer
  const double* GetData(int block, int chans);
                // gets pointer of filled buffer
  const double* GetData(void);
                // programs bioradio using a configuration file
        int     Program(const std::string& config);
                // determines the port that the bioradio is attached to
  const char*   PortTest(int port);

 private:       // buffer merging (private: isnt required outside the object)
  static void   BufferMerge(double* buffer1,const double* buffer2, int start, int finish);

 private:
                // Storage of the success flags
        bool    mFlags[3];
                // Pointer to a char buffer that stores the COM port, eg "COM3"
  const char*   mpPort;
                // An array that holds the raw data obtained from the bioRadio
        double  mData[BUFFER_SIZE];
                // Number of samples collected
        int     mNumRead;
                // Type DWORD, is equated to the radio object
  static DWORD  sBR150;
                // Stores the running state of the bioradio
  static bool   sRunningState;
};

#endif // BR150_H

