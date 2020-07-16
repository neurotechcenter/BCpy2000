###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for gUSBampSource

ADD_SUBDIRECTORY( getinfo )

BCI2000_ADD_SIGNAL_SOURCE_MODULE( gUSBampSource
  gUSBampADC.cpp
  lib/gUSBamp.imports.cpp
)
