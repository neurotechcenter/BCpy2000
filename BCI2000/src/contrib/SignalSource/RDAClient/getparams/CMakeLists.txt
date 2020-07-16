###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for Neuroscan getparams tool

SET( NAME RDAgetparams )

SET( SOURCES
  RDAgetparams.cpp
  ${BCI2000_SRC_DIR}/contrib/SignalSource/RDAClient/RDAProtocol.cpp
  ${BCI2000_SRC_DIR}/shared/bcistream/BCIStream_tool.cpp
)

UTILS_INCLUDE( frameworks/Core )
BCI2000_ADD_TARGET( INFO Tool EXECUTABLE ${NAME} ${SOURCES} )
BCI2000_ADD_TO_INVENTORY( Tool ${NAME} )
