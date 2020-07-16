###########################################################################
## Authors:
## Description: Build information for the StimResponseSignalProcessing project
###########################################################################
add_subdirectory( cmdline )

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
BCI2000_INCLUDE(QT)
bci2000_include(MATH)

SET( FILES_PROJECT
  ${FILES_PROJECT}
  batch/StimulusResponseVis.bat
)



bci2000_add_signal_processing_module(

  StimResponseSignalProcessing

  PipeDefinition.cpp
  StimResponseFilter.cpp
  StimResponseVis.cpp
  BitmapImage_QtGlue.cpp # will be moved into a CMake-managed glue library
  ${PROJECT_SRC_DIR}/shared/utils/QtMain.cpp

  "${FILES_PROJECT}"

)


