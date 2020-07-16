###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for gMOBIlab

bci2000_include( SourceFilter )
bci2000_add_signal_source_module( gMOBIlab
  gMOBIlabADC.cpp
  lib/spa20a.imports.cpp
)
