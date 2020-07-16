////////////////////////////////////////////////////////////////////////////////
// Authors: 
// Description: LSLADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_LSLADC_H  // makes sure this header is not included more than once
#define INCLUDED_LSLADC_H

#include "BufferedADC.h"


#include "liblsl64.imports.h"

class LSLADC : public BufferedADC
{
 public:
  LSLADC();
  ~LSLADC();
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void DoAcquire( GenericSignal& Output ) override;
  void OnStopAcquisition() override;

  void OnTrigger( int ); // for asynchronous triggers only

 private:
   lsl_streaminfo minfo;	/* the streaminfo returned by the resolve call */
   lsl_inlet minlet;		/* a stream inlet to get samples from */
   int merrcode;			/* error code (lsl_lost_error or timeouts) */
   float mcursample[16];		/* array to hold our current sample */
   double mtimestamp;	
};

#endif // INCLUDED_LSLADC_H
