###########################################################################
## $Id: CMakeLists.txt 5601 2017-01-17 20:32:32Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for SignalGenerator

bci2000_include(SourceFilter)
BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
  SignalGenerator
  SignalGeneratorADC.cpp
  ${BCI2000_SRC_DIR}/shared/modules/signalsource/logging/TestLogger.cpp
  SUPPORT /batch/*_SignalGenerator.bat
  SUPPORT /parms/examples/*_SignalGenerator.prm
  SUPPORT /parms/fragments/amplifiers/Signal*.prm
)
