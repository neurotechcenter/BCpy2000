###########################################################################
## $Id: AppModuleFramework.cmake 3483 2011-08-23 18:24:55Z mellinger $
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
## Description: List of application module files.

SET( SRC_BCI2000_FRAMEWORK 
  ${SRC_BCI2000_FRAMEWORK}

  ${PROJECT_SRC_DIR}/shared/gui/GUI.cpp
  ${PROJECT_SRC_DIR}/shared/gui/GraphicResource.h
  ${PROJECT_SRC_DIR}/shared/gui/GraphObject.cpp
  ${PROJECT_SRC_DIR}/shared/gui/GraphDisplay.cpp
  ${PROJECT_SRC_DIR}/shared/gui/Bitmap.cpp
  ${PROJECT_SRC_DIR}/shared/gui/DisplayWindow.cpp
  ${PROJECT_SRC_DIR}/shared/gui/OpenGLContext.cpp
  ${PROJECT_SRC_DIR}/shared/gui/DisplayClock.cpp
  ${PROJECT_SRC_DIR}/shared/utils/ClockTick.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Rate.h

  ${PROJECT_SRC_DIR}/shared/gui/TextField.cpp
  ${PROJECT_SRC_DIR}/shared/gui/Shapes.cpp
  ${PROJECT_SRC_DIR}/shared/gui/SpinningWheel.cpp
  ${PROJECT_SRC_DIR}/shared/gui/DisplayStatisticsField.cpp

  ${PROJECT_SRC_DIR}/shared/modules/application/ApplicationBase.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/StimulusTask.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/FeedbackTask.cpp

  ${PROJECT_SRC_DIR}/shared/utils/AV/AudioDevice.cpp
  ${PROJECT_SRC_DIR}/shared/utils/AV/AudioSink.cpp
  ${PROJECT_SRC_DIR}/shared/utils/AV/AVUtils.cpp
  ${PROJECT_SRC_DIR}/shared/utils/AV/AVDecoder.cpp
  ${PROJECT_SRC_DIR}/shared/utils/AV/AVPlayer.cpp
  ${PROJECT_SRC_DIR}/shared/utils/AV/VideoPlayer.cpp
  ${PROJECT_SRC_DIR}/shared/utils/AV/AudioPlayer.cpp
  ${PROJECT_SRC_DIR}/shared/utils/AV/ImageViewer.cpp
  
  ${PROJECT_SRC_DIR}/shared/modules/application/audio/TextToSpeech.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/audio/WavePlayer.cpp

  ${PROJECT_SRC_DIR}/shared/modules/application/gui/ApplicationWindow.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/gui/TextWindow.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/gui/StatusBar.cpp

  ${PROJECT_SRC_DIR}/shared/modules/application/utils/TrialStatistics.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/utils/BlockRandSeq.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/utils/Localization.cpp

  ${PROJECT_SRC_DIR}/shared/modules/application/stimuli/Association.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/stimuli/AudioStimulus.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/stimuli/AVStimulus.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/stimuli/ImageStimulus.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/stimuli/TextStimulus.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/stimuli/VisualStimulus.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/stimuli/SpeechStimulus.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/stimuli/SoundStimulus.cpp

  ${PROJECT_SRC_DIR}/shared/modules/application/speller/SpellerCommand.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/speller/Speller.cpp
  ${PROJECT_SRC_DIR}/shared/modules/application/speller/AudioSpellerTarget.h
  
  ${PROJECT_SRC_DIR}/shared/modules/application/ConnectorFilters.cpp

  ${PROJECT_SRC_DIR}/shared/utils/QtMain.cpp
  ${PROJECT_SRC_DIR}/shared/utils/QtProxyThread.cpp
)

IF( WIN32 )
  SET( SRC_BCI2000_FRAMEWORK 
    ${SRC_BCI2000_FRAMEWORK}
    ${PROJECT_SRC_DIR}/shared/modules/application/audio/MidiPlayer.cpp
    ${PROJECT_SRC_DIR}/shared/modules/application/human_interface_devices/KeystrokeFilter.cpp
    ${PROJECT_SRC_DIR}/extlib/com/ComPtr.h
  )
ENDIF( WIN32 )
