###########################################################################
## $Id: IncludeExtension.cmake 4970 2015-08-21 16:46:07Z mellinger $
## Authors: jezhill@gmail.com

IF( NOT WIN32 )
  MESSAGE( "**** DataGloveLogger failed: fglove.dll not found for this platform" )
  RETURN()
ENDIF()

list( APPEND BCI2000_SIGSRC_LIBS
)

list( APPEND BCI2000_SIGSRC_FILES
   ${BCI2000_EXTENSION_DIR}/5DTDataGloveUltraLogger.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/include/fglove.imports.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/fglove64.dll
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/fglove32.dll
)
