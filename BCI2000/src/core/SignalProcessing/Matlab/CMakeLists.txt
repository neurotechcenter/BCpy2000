###########################################################################
## $Id: CMakeLists.txt 5395 2016-06-09 13:34:53Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for MatlabSignalProcessing

set( SUPPLOC ${PROJECT_ROOT_DIR}/prog/matlab )
bci2000_include( matlab_engine )
bci2000_add_signal_processing_module(
  MatlabSignalProcessing
  PipeDefinition.cpp 
  MatlabFilter.cpp
  MatlabWrapper.cpp
  SUPPORT /prog/matlab/*
  SUPPORT /batch/MatlabDemo_SignalGenerator.bat
  SUPPORT /parms/examples/MatlabDemo_SignalGenerator.prm
)

