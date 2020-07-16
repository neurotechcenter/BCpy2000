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
// dtadfun.cpp

#include "PCHIncludes.h"
#pragma hdrstop

#include <stdlib.h>

#include "dtfun.h"

//---------------------------------------------------------------------------
DTFUN::DTFUN( void )
{
 data_critsec=new TCriticalSection();
 bdone=NULL;
 outputclockonusercounter=false;         // this is to output the sample clock onto the user counter
}
//---------------------------------------------------------------------------

DTFUN::~DTFUN( void )
{
 if (data_critsec) delete data_critsec;
 if (bdone) delete bdone;
 data_critsec=NULL;
 bdone=NULL;
}

//---------------------------------------------------------------------------
int DTFUN::Start( void )
{
int     ret;
ECODE   status;

 // if we output the driving A/D sample clock on the user counter pin,
 // then we start the system by starting the counter subsystem (A/D input pre-started in Config before)
 if (outputclockonusercounter)
    status= olDaStart( lphDassCT );
 else
    status= olDaStart( lphDass );

 ret=1;
 if (status != OLNOERROR) ret=0;

 return(ret);
}

//----------------------------------------------------------------------------
int DTFUN::Stop( void )
{
ECODE status;

 status= olDaAbort(lphDass);
 if (outputclockonusercounter)
    status= olDaAbort(lphDassCT);

 return( status );
}
//-----------------------------------------------------------------------------

int DTFUN::Reset( void )
{
ECODE status;

 status= olDaReset( lphDass );
 if (outputclockonusercounter)
    status= olDaReset(lphDassCT);

 return( status );
}

//-----------------------------------------------------------------------------

int DTFUN::CleanUp( void )
{
int     i, bcount, status;

 status= olDaFlushBuffers( lphDass );

 bcount= BUFNUM;

 for (i=0; i<bcount; i++)
  {
  status+= olDaGetBuffer( lphDass, &hbuf[i] );
  status+= olDmFreeBuffer( hbuf[i] );
  }

 if (outputclockonusercounter)
    status+= olDaReleaseDASS( lphDassCT );

 status+= olDaReleaseDASS( lphDass );
 return( status );
}

//-----------------------------------------------------------------------------

int DTFUN::Terminator( void )
{
ECODE status;

 status= olDaTerminate( lphDev );

 return( status );
}

//-----------------------------------------------------------------------------
void DTFUN::SetWindow( HWND msgw )
{
 Ad_Win_Msg= msgw;
}

//---------------------------------------------------------------------------

void DTFUN::InitBoard( const char *board_name, bool new_outputclockonusercounter )
{
UINT    uiElement;
UINT    uiBits;

 outputclockonusercounter=new_outputclockonusercounter;

 SetBoardName(board_name);

 lpszName= BoardName;  // "BCI_IN";
 lphDev = NULL;
 uiElement= 0;

 // initialize this board
 status= olDaInitialize( lpszName, &lphDev );

 // get a handle to the A/D subsystem
 status= olDaGetDASS( lphDev, OLSS_AD, uiElement, &lphDass) ;

 // do we want to output the sample clock onto the user counter pin?
 // (this user counter pin is used as an A/D sample clock input for this board
 // and can also be used to synchronize boards
 if (outputclockonusercounter)
    status= olDaGetDASS(lphDev, OLSS_CT, 0, &lphDassCT);

 uiDataFlow= OL_DF_CONTINUOUS;
 status= olDaSetDataFlow( lphDass, uiDataFlow );

 iMsg= status;

 status= olDaGetResolution( lphDass, (LPUINT)&uiBits );
 ADSize= (int)uiBits;
}

//--------------------------------------------------------------------

UINT DTFUN::SetChanType( UINT ChanType )
{
 errc= olDaSetChannelType( lphDass, ChanType);

 return( errc );
}

UINT DTFUN::SetChanList( UINT ListSize, DBL dGain )
{
     int count;

     LSize= ListSize;

     errc= olDaSetChannelListSize( lphDass, ListSize );

     for(count=0; count< (int)ListSize; count++)
     {
        status= olDaSetChannelListEntry( lphDass, count ,count );

        status= olDaSetGainListEntry( lphDass, count, dGain );
     }
     return( status );
}


DBL DTFUN::SetClock(int ClockSource, DBL Freq )
{
        ECODE status;

        if(ClockSource == 0)
        {
                status= olDaSetClockSource( lphDass, OL_CLK_INTERNAL );
        }
        else
        {
                status= olDaSetClockSource( lphDass, OL_CLK_EXTERNAL );
        }

        status= olDaSetClockFrequency( lphDass, Freq );

        status= olDaConfig( lphDass );

        status= olDaGetClockFrequency( lphDass, &Freq );

        return( Freq );
}

ECODE DTFUN::SetWndHandle( void )
{

        errc= olDaSetWndHandle( lphDass, Ad_Win_Msg, lParam );
        return( errc );
}

ECODE DTFUN::SetBuffers( DWORD BufSize )
{
        int i;
        ECODE retval= 0;

        status= olDaSetWrapMode( lphDass, OL_WRP_MULTIPLE  );

        dwSize= BufSize;

        for(i=0;i<BUFNUM;i++)
        {
                retval+= olDmAllocBuffer( 0, dwSize, &hbuf[i] );
                retval+= olDaPutBuffer( lphDass, hbuf[i] );
        }

        retval+= olDaConfig( lphDass );

        d_count= 0;   // initialize data counter
        BufferCount= 0;
        BufferPtr= 0;

        return( status );
}

//----------------------------------------------------------------
void DTFUN::SetBoardName(const char *new_board_name)
{
 strcpy(BoardName, (const char *)new_board_name);
}


void DTFUN::Add_to_data(short lphBuf[], ULNG samples)
{
unsigned        i;

 if (ADSize == 16)       // check for resolution of A/D converter
    {
    for (i=0; i<samples; i++)
     {
     data[BufferPtr]= (short)(lphBuf[i]-32678);
     BufferPtr++;
     }
    }
 else
    {
    for (i=0; i<samples; i++)
     {
     data[BufferPtr]= (short)(lphBuf[i]-2048) * 16;
     BufferPtr++;
     }
    }

 BufferCount++;
}


void DTFUN::SetFunction(  void )
{
OLNOTIFYPROC    lpfnNotifyProc;
ECODE           errc;

 lpfnNotifyProc=(OLNOTIFYPROC)&BufferDone;
 errc= olDaSetNotificationProcedure( lphDass, lpfnNotifyProc, (LPARAM)this );   // tell the callback function which instance of DTADC called it
 if (bdone) delete bdone;
 #if( __BORLANDC__ >= 0x0570 ) // no VCL version macro available
 bdone = new TEvent( NULL, false, false, "", false );
 #else
 bdone= new TEvent(NULL,false,false,"");
 #endif 
}

//----------------------------------------------------------------


// this function is called directly by the driver whenever data is ready
// there is an anomaly in the (current version of the) Data Translation driver
// that causes this function to be called unevenly spaced in time
// this only happens under certain circumstances (i.e., number of channels,
// SampleBlockSize, and sampling frequency)
// see corresponding description in BCI2000 documentation
__stdcall BufferDone( UINT uiMsg, unsigned int Dass, LPARAM lParam )
{
ECODE   status;
int     arrays;
int     i;
short   *buffer;
HBUF    hBuf;
LPHBUF  lphBuf;
ULNG    samples;
HDASS   lphDass;

 hBuf= NULL;
 lphBuf= NULL;
 lphDass= (void *)Dass;

 status= olDaGetBuffer(lphDass, &hBuf);

 if ( hBuf != NULL )
    {
    status= olDmGetMaxSamples(hBuf,&samples);
    status= olDmGetBufferPtr( hBuf, (LPVOID FAR*)&lphBuf );
    status= olDaPutBuffer(lphDass, hBuf);

    buffer= (short *)lphBuf;

    ((DTFUN *)(lParam))->data_critsec->Acquire();
    ((DTFUN *)(lParam))->Add_to_data( buffer, samples );  // add data to FIFO
    ((DTFUN *)(lParam))->data_critsec->Release();
    }

 // notify ADReadDataBlock() that data is here
 ((DTFUN *)(lParam))->bdone->SetEvent();
 return(0);
}


ECODE DTFUN::ConfigAD( UINT ChanType,
                                 UINT ListSize,
                                 DBL  Gain,
                                 int  ClockSource,
                                 DBL  Freq,
                                 DWORD BufSize   )
{
int status;

        ECODE result= 0;
        BufferSize= (int)BufSize;

        result+= SetChanType( ChanType );
        result+= SetChanList( ListSize, Gain );
        Freq= SetClock(ClockSource, Freq );       // ClockSource=0 ... internal, 1=external
        result+= SetWndHandle();
        result+= SetBuffers( BufSize );

        ClockHz= Freq;

        // do we want to output the sample clock onto the user counter pin?
        // (this user counter pin can then be used as an A/D sample clock input for another board
        // and is also used to drive this particular board
        // NOTE that the user counter pin has to be wired externally to the A/D Sample Clock In pin
        if (outputclockonusercounter)
           {
           // set the cascade mode
           status= olDaSetCascadeMode(lphDassCT, OL_CT_SINGLE);
           // set up the clocks and gates
           // use an internal clock
           status= olDaSetClockSource(lphDassCT, OL_CLK_INTERNAL);
           // set the clock frequency
           status= olDaSetClockFrequency(lphDassCT, ClockHz);
           // specify the gate to enable the C/T operation
           status= olDaSetGateType(lphDassCT, OL_GATE_NONE);
           // status= olDaSetGateType(lphDassCT, OL_GATE_HIGH_LEVEL);
           // specify the mode for continuous output
           status= olDaSetCTMode(lphDassCT, OL_CTMODE_RATE);    // as opposed to OL_CTMODE_ONESHOT
           // specify the output pulse type
           status= olDaSetPulseType(lphDassCT, OL_PLS_HIGH2LOW);
           // specify the duty cycle or pulse width
           status= olDaSetPulseWidth(lphDassCT, 100);
           // configure the subsystem
           status= olDaConfig( lphDassCT );
           // if we use a self-generated clock, then pre-start the A/D conversion here
           // start of sampling will be triggered when counter/clock is started
           status= olDaStart( lphDass );
           }

 return( result );
}
