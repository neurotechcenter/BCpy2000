###########################################################################
## $Id: IncludeExtension.cmake 4536 2013-08-05 14:30:13Z mellinger $
## Authors: kaleb.goering@gmail.com

# Add the gEstimFilter to all application modules

IF( NOT WIN32 )
  MESSAGE( "**** gEstimFilter failed: Not supported on this platform" )
  RETURN()
ENDIF()

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET( BCI2000_APP_FILES
	   ${BCI2000_APP_FILES}
	   ${BCI2000_EXTENSION_DIR}/gEstimFilter.cpp
	   ${BCI2000_EXTENSION_DIR}/lib/gEstimAPI.imports.cpp
	   ${BCI2000_EXTENSION_DIR}/lib/ftd2xx.dll
	   ${BCI2000_EXTENSION_DIR}/lib/gEstimAPI.Device.dll
	   ${BCI2000_EXTENSION_DIR}/lib/gEstimAPI.dll
	)
ELSE()
	MESSAGE("**** gEstimFilter failed: Not supported on x86 platforms")
	RETURN()
ENDIF()
