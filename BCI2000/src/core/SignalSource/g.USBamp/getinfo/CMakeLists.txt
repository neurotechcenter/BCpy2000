###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Build information for USBampgetinfo

SET( NAME gUSBampgetinfo )

UTILS_INCLUDE( frameworks/Core )
BCI2000_ADD_TARGET( INFO Tool EXECUTABLE ${NAME}
  gUSBampgetinfo.cpp
  ../lib/gUSBamp.imports.cpp
)
IF( NOT FAILED )
  BCI2000_ADD_TO_INVENTORY( Tool ${NAME} )
ENDIF( NOT FAILED )
