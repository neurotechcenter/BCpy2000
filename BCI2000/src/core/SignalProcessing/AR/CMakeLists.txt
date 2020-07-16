###########################################################################
## $Id: CMakeLists.txt 5473 2016-07-23 01:02:25Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for ARSignalProcessing in Qt

# Create the signal processing module
BCI2000_ADD_SIGNAL_PROCESSING_MODULE( 
  ARSignalProcessing
  PipeDefinition.cpp
)

