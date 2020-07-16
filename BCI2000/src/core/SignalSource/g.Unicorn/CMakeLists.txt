###########################################################################
## Authors:
## Description: Build information for the gTecUnicorn source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )
IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )

ELSE()
  set(	FILES_PROJECT
		${FILES_PROJECT}
		${SRC_PROJECT}/lib/x64/Unicorn.dll
  )
ENDIF()
BCI2000_INCLUDE( "SOURCEFILTER" )

BCI2000_ADD_SIGNAL_SOURCE_MODULE(
  gUnicornADC
  gTecUnicornADC.cpp
  imports/unicorn.h
  imports/Unicorn.imports.cpp
  imports/Unicorn.imports.h
  ${FILES_PROJECT}
)



