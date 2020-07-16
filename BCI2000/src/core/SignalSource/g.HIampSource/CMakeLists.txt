###########################################################################
## $Id: CMakeLists.txt 5240 2016-02-15 16:22:38Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for the gHIampSource source module
###########################################################################

BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  gHIampSource
  gHIampADC.cpp
  gHIampDevice.cpp
  lib/gHIamp.imports.cpp
)
