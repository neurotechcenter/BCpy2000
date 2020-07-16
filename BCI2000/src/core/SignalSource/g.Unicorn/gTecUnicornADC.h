////////////////////////////////////////////////////////////////////////////////
// Authors: 
// Description: gTecUnicornADC header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_GTECUNICORNADC_H  // makes sure this header is not included more than once
#define INCLUDED_GTECUNICORNADC_H

#include "BufferedADC.h"
#include "Unicorn.imports.h"


class gTecUnicornADC : public BufferedADC
{
 public:
  
  gTecUnicornADC();
  ~gTecUnicornADC();
  void OnPublish() override;
  void OnAutoConfig() override;
  void OnPreflight( SignalProperties& Output ) const override;
  void OnInitialize( const SignalProperties& Output ) override;
  void OnStartAcquisition() override;
  void DoAcquire( GenericSignal& Output ) override;
  void OnStopAcquisition() override;
  


 private:
  // Use this space to declare any gTecUnicornADC-specific methods and member variables you'll need
  
  UNICORN_HANDLE mDeviceHandle;
  float* mpBuffer;
  unsigned int mNumberOfSignalChannels;
  int mNumberOfSamples; //samplingfreq & blocksize
  int mbufferSize;
  float mSamplingRate;

  std::string ChannelNames[UNICORN_EEG_CHANNELS] = { "Fz", "Cz","P3", "Pz","P5","Oz","Po8","Po7" };
};

#endif // INCLUDED_GTECUNICORNADC_H
