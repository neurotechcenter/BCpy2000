###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including PortAudio in a project

INCLUDE_DIRECTORIES( ${PROJECT_SRC_DIR}/extlib/portaudio/portaudio/include )
SET( LIBS
  ${LIBS}
  PortAudio
)
