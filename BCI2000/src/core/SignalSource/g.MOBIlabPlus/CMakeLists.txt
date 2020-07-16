###########################################################################
## $Id: CMakeLists.txt 5286 2016-03-16 16:26:47Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for gMOBIlabPlus

bci2000_include( SourceFilter )
add_definitions( -DGMOBILABPLUS )
bci2000_add_signal_source_module( gMOBIlabPlus
  ../g.MOBIlab/gMOBIlabADC.cpp
  lib/gMOBIlabplus.imports.cpp
)
