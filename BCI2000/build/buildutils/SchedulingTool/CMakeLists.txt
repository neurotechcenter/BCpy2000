###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for SchedulingTool.

SET( DIR_NAME Build/Tools )
UTILS_INCLUDE( frameworks/LibTiny )
ADD_DEFINITIONS( -DUSE_QT )
BCI2000_ADD_TARGET( QTAPP NOBUNDLE SchedulingTool
  SchedulingTool.cpp
  ${PROJECT_SRC_DIR}/shared/utils/QtSignalConnector.h
  OUTPUT_DIRECTORY ${BUILDUTILS_DIR}
  LIBRARY pdh
)

