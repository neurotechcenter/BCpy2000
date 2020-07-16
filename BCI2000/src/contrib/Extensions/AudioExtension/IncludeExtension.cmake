###########################################################################
## $Id: IncludeExtension.cmake 4970 2015-08-21 16:46:07Z mellinger $
## Authors: griffin.milsap@gmail.com

SET( BCI2000_SIGSRC_LIBS
  ${BCI2000_SIGSRC_LIBS}
  PortAudio
  Sndfile
)

SET( BCI2000_SIGSRC_FILES
  ${BCI2000_SIGSRC_FILES}
  ${BCI2000_SRC_DIR}/extlib/libsndfile/include/sndfile.h
  ${BCI2000_SRC_DIR}/extlib/portaudio/portaudio/include/portaudio.h
  ${BCI2000_EXTENSION_DIR}/AudioExtension.cpp
  ${BCI2000_SRC_DIR}/extlib/math/FilterDesign.cpp
)
