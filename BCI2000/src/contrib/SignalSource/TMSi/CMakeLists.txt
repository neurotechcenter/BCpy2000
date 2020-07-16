###########################################################################
## $Id: CMakeLists.txt 5342 2016-04-29 14:43:51Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for TMSi source module

INCLUDE_DIRECTORIES( TMSiSDK )

BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  TMSi
  TMSiADC.cpp
  TMSiSDK/Feature.cpp
  TMSiSDK/RTDevice.cpp
  TMSiSDK/Sadio.h
)
