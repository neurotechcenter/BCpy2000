###########################################################################
## Authors:
## Description: Build information for the ReferenceViewFilter project
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATLAB )
BCI2000_INCLUDE(QT)


SET( FILES_PROJECT
  ${FILES_PROJECT}
  batch/ReferenceViewFilter.bat
)


bci2000_add_signal_processing_module(

  ReferenceViewFilter

  PipeDefinition.cpp
  ReferenceViewFilter.cpp
  ChannelVisFilter.cpp
  BitmapImage_QtGlue.cpp
  IIRPeakFilter.cpp
  ${PROJECT_SRC_DIR}/shared/utils/QtMain.cpp

  "${FILES_PROJECT}"
)
