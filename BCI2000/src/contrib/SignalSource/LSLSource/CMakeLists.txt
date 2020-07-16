###########################################################################
## Authors:
## Description: Build information for the LSLSource source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

bci2000_include(SourceFilter)
bci2000_add_signal_source_module(
  LSLSource
  LSLADC.cpp
  extlib/include/liblsl64.imports.cpp
  extlib/dylib/liblsl32.dll
  extlib/dylib/liblsl64.dll
  batch/CursorTask_LSLSource.bat
  batch/P3Speller_LSLSource.bat
  batch/StimulusPresentation_LSLSource.bat
)

#LINK_DIRECTORIES(
#  ${BCI2000_SRC_DIR}/contri/SignalSource/LSLSource/extlib/lib/
#)

#TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} liblsl64.lib )
