###########################################################################
## $Id$
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000MediaPlayer

# Set the executable name
SET( EXECUTABLE_NAME BCI2000MediaPlayer )

# Set the project specific sources
SET( UTILS ${PROJECT_SRC_DIR}/shared/utils )
SET( GUI ${PROJECT_SRC_DIR}/shared/gui )
SET( AV ${PROJECT_SRC_DIR}/shared/utils/AV )

SET( SRC_PROJECT
  BCI2000MediaPlayer.cpp

  ${UTILS}/ClockTick.cpp
  ${UTILS}/QtProxyThread.cpp

  ${GUI}/DisplayWindow.cpp
  ${GUI}/GraphDisplay.cpp
  ${GUI}/GraphObject.cpp
  ${GUI}/OpenGLContext.cpp
  ${GUI}/DisplayClock.cpp
  ${GUI}/SpinningWheel.cpp
  ${GUI}/TextField.cpp
  ${GUI}/GUI.cpp
  ${GUI}/Bitmap.cpp

  ${AV}/AVUtils.cpp
  ${AV}/AVDecoder.cpp
  ${AV}/AVPlayer.cpp
  ${AV}/AudioDevice.cpp
  ${AV}/AudioSink.cpp
  ${AV}/VideoPlayer.cpp
  ${AV}/AudioPlayer.cpp
)

if( WIN32 )
  list( APPEND SRC_PROJECT ${PROJECT_SRC_DIR}/extlib/com/ComPtr.h )
endif()

ADD_DEFINITIONS( -DUSE_QT )
BCI2000_INCLUDE( "LibAV" )
BCI2000_INCLUDE( "Statistics" )
BCI2000_INCLUDE( "OpenGL" )
UTILS_INCLUDE( extlib/PORTAUDIO )
utils_set_appicon( film )
BCI2000_ADD_TOOLS_GUIAPP( ${EXECUTABLE_NAME} ${SRC_PROJECT} )
