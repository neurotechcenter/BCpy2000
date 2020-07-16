/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
/*/
// dtfun.h

#include <olmem.h>
#include <olerrors.h>
#include <oldaapi.h>
#include <stdio.h>

#include <scktcomp.hpp>


#define MAXCHANS     64
#define MAXPOINTS    0x7ffff
#define BUFNUM       10        // was 3 and 5

DECLARE_HANDLE(HDRVR);

class DTFUN
{
public:
        TCriticalSection *data_critsec;         // critical section for data FIFO
        TEvent  *bdone;
        int     BufferCount;
        int     BufferPtr;

        DTFUN::DTFUN( void );
        DTFUN::~DTFUN( void );

        short data[MAXPOINTS];

        int  Start( void );
        int  Stop( void );
        int  Reset( void );
        int  CleanUp( void );
        int  Terminator( void );
        void Save( void );
        void SetWindow( HWND );
        void SetFunction( void );
	void InitBoard( const char *, bool );
	void SetBoardName(const char *);
        void Add_to_data( short *, ULNG );
        ECODE ConfigAD( UINT, UINT, DBL, int, DBL, DWORD);

private:
        int     ClockHz;
        ECODE   errc;
	int     iMsg;
        int     save_flag;
        char    DatFile[80];
        int     n_display;

        char    BoardName[128];
        HDASS lphDass;          //   Subsystem Handle
        HDASS lphDassCT;        //   Subsystem Handle for Counter/Timer

        HWND Ad_Win_Msg;        //   handle to window for A/D messages
        UINT LSize;             //   channel list size
        FILE *dfile;
        long d_count;           //  data counter

	ECODE status;
        LPSTR lpszName;
        HDRVR lphDev;           //   Board handle
        UINT uiDataFlow;

    //    DBL dfFreq;             //   sampling frequency
        LPARAM lParam;          //   user-defined message parameter
        DWORD dwSize;           //   size of data buffers
        UINT uiWinFlags;        //   windows memory allocation flags
        HBUF hbuf[BUFNUM];      //   buffer pointers
        int BufferSize;         //   buffer size
        int ADSize;             //   size of AD units in bits (12 or 16)

        bool    outputclockonusercounter;       // do we want to output the sample clock onto the user counter pin?

        UINT  SetChanType( UINT );
        UINT  SetChanList( UINT, DBL );
        DBL   SetClock(int , DBL  );
        ECODE SetWndHandle( void );
        ECODE SetBuffers( DWORD );
};

__stdcall BufferDone( UINT, unsigned int, LPARAM );
