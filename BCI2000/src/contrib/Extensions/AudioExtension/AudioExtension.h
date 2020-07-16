/////////////////////////////////////////////////////////////////////////////
// $Id: AudioExtension.h 5818 2018-11-09 15:50:15Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The audio extension enables input and mixed output from/to 
//   sound cards on the machine. The extension also handles filtering of
//   audio input/output envelope to states
//
// Documentation located at: 
//   http://www.bci2000.org/wiki/index.php/Contributions:AudioExtension
//
// $BEGIN_BCI2000_LICENSE$
// $END_BCI2000_LICENSE$
/////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_EXTENSION_H
#define AUDIO_EXTENSION_H

#include <string>
#include <iostream>
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <portaudio.h>
#include <sndfile.h>
#include "Environment.h"
#include "Thread.h"
#include "RandomGenerator.h"
#include "Expression/Expression.h"
#include "IIRFilter.h"

class AudioExtension : public EnvironmentExtension, public Thread
{
 public:
  //Constructors and virtual interface
  AudioExtension();
  virtual ~AudioExtension();
  virtual void Publish();
  virtual void Preflight() const;
  virtual void Initialize();
  virtual void Process();
  virtual void StartRun();
  virtual void StopRun();
  
  // Inherited from Thread
  virtual int OnExecute() override;

 private:
  typedef std::vector< Expression > ExpressionVector;
  typedef std::vector< ExpressionVector > ExpressionMatrix;
  typedef std::vector< std::pair< std::string, int > > ChannelDef;
  typedef std::vector< IIRFilter< FilterDesign::Real > > FilterBank;
  ExpressionMatrix mMixer;
  std::vector< std::vector< float > > mGainMatrix;
  
  // Internal configuration methods
  bool GetHostApiConfig( PaHostApiTypeId hostApi, const PaHostApiInfo* &apiInfo, PaHostApiIndex &hostIdx ) const;
  void GetDeviceConfig( const PaHostApiInfo* apiInfo, PaHostApiIndex hostIdx, 
    int &inputIdx, int &outputIdx, int &inputChannels, int &outputChannels ) const;
  void LoadMixer( const ParamRef &matrix, ExpressionMatrix &mixer,
    ChannelDef &channelDef, int audioIns, int audioOuts, int fileIns ) const;
  void EvaluateMixer( ExpressionMatrix &mixer, std::vector< std::vector< float > > &gainMatrix ) const;
  void DesignFilterbank( const ParamRef &matrix, IIRFilter< FilterDesign::Real > &filter, size_t numCh ) const;
  bool KillStream();
  
  // Static audio callback
  static int AudioCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags, void *userData );
  
  // Member variables
  std::set< int >                    mSupportedAPIs;
  int                                mHostAPI;
  
  Synchronized<bool>                 mBufferProcessed;
  GenericSignal                      mAudioInputBlock,
                                     mAudioOutputBlock,
                                     mAudioInputEnvelope,
                                     mAudioOutputEnvelope;
  
  unsigned int                       mFrameCount,
                                     mFramesPerBuffer,
                                     mMaxInputEnvelopes,
                                     mMaxOutputEnvelopes;
  
  int                                mInputChannels,
                                     mOutputChannels,
                                     mFileChannels;
  
  ChannelDef                         mChannelDef;
  RandomGenerator                    mRand;
  
  PaStream                          *mpAudioStream;
  
  SNDFILE                           *mpAudioInputFile,
                                    *mpAudioRecInputFile,
                                    *mpAudioRecOutputFile;
  
  FilterBank                         mInputEnvelopeFilter,
                                     mOutputEnvelopeFilter;
  IIRFilter< FilterDesign::Real >    mInputFilter,
                                     mOutputFilter;
};

#endif // AUDIO_EXTENSION_H
