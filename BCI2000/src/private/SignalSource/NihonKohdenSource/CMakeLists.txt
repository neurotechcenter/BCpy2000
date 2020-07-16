###########################################################################
## $Id:  $
## Authors:
## Description: Build information for the NihonKohdenSource source module
###########################################################################

if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)

# Set the executable name
SET( EXECUTABLE_NAME NihonKohdenSource )

# Set the project specific sources
SET( SRC_PROJECT
  NihonKohdenADC.cpp
)

SET( HDR_PROJECT
  NihonKohdenADC.h
  CmdParamInfo.h
  lib/EegDataSource.h
)

INCLUDE_DIRECTORIES(

)

# specify dll files to be copied
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET( FILES_PROJECT
      ${FILES_PROJECT}
# todo: acquire 64-bit dll
# there ain't one afaik -- Griff
#   lib/coff64/EegDataSource.dll
    )
ELSE()
  SET( FILES_PROJECT
    ${FILES_PROJECT}
    lib/coff/EegDataSource.dll
  )
  
  LINK_DIRECTORIES(
    ${BCI2000_SRC_DIR}/private/SignalSource/NihonKohdenSource/lib/coff/
  )
ENDIF()

# specify batch files 
SET( FILES_PROJECT
  ${FILES_PROJECT}
  batch/ContinuousRecording_NihonKohden.bat
  batch/CursorTask_NihonKohden.bat
  batch/P3Speller_NihonKohden.bat
  batch/StimulusPresentation_NihonKohden.bat
)

# Add the source module to the Makefile or compiler project file:
LIST( APPEND SRC_PROJECT ${BCI2000_SRC_DIR}/shared/modules/CoreMain.cpp )
BCI2000_ADD_SIGNAL_SOURCE_MODULE(
  "${EXECUTABLE_NAME}"
  "${SRC_PROJECT}" "${HDR_PROJECT}" "${FILES_PROJECT}"
)

TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} EegDataSource )

else()
  message("NihonKohdenSource: 64 bit library not available, will not build")
endif()