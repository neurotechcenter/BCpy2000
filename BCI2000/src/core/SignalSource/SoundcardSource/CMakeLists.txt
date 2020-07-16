###########################################################################
## $Id: CMakeLists.txt 4888 2015-07-06 14:45:04Z mellinger $
## Description: Build information for the SoundcardSource source module

UTILS_INCLUDE( extlib/PORTAUDIO )
BCI2000_INCLUDE( "Statistics" )
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  SoundcardSource
  SoundcardADC.cpp
  ${BCI2000_SRC_DIR}/shared/utils/AV/AudioDevice.cpp
  ${BCI2000_SRC_DIR}/shared/utils/ClockTick.cpp
)

