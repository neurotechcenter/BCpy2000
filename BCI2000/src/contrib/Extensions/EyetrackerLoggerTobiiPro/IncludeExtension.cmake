###########################################################################
## $Id: IncludeExtension.cmake 4536 2013-08-05 14:30:13Z mellinger $
## Authors: kaleb.goering@gmail.com



SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/EyetrackerLoggerTobiiPro.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/include/tobii_research.imports.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/include/tobii_research_stream.imports.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/include/tobii_research_eyetracker.imports.cpp
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET ( BCI2000_SIGSRC_FILES
		${BCI2000_SIGSRC_FILES}
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x64/tobii_pro.dll 
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x64/tobii_research.dll 
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x64/tobii_stream_engine.dll 
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x64/tobii_firmware_upgrade.dll 
	)
ELSE()
	SET ( BCI2000_SIGSRC_FILES
		${BCI2000_SIGSRC_FILES}
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x86/tobii_pro.dll 
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x86/tobii_research.dll 
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x86/tobii_stream_engine.dll
		${BCI2000_EXTENSION_DIR}/extlib/dylib/x64/tobii_firmware_upgrade.dll 
	)
ENDIF()
