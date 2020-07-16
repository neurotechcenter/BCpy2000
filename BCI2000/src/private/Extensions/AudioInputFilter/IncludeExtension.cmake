###########################################################################
## $Id: IncludeExtension.cmake 3348 2011-06-24 15:41:55Z mellinger $
## Authors: jezhill@gmail.com

IF( NOT WIN32 )
  MESSAGE( "**** AudioInputFilter failed: not supported on this platform" )
  RETURN()
ENDIF()

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/AudioInput.cpp
   ${BCI2000_EXTENSION_DIR}/AudioInputFilter.cpp
)
