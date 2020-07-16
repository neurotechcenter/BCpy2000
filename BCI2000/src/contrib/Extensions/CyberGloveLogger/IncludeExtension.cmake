###########################################################################
## $Id: IncludeExtension.cmake 3348 2011-06-24 15:41:55Z mellinger $
## Authors: sidhant.sharma@gmail.com

IF( NOT WIN32 )
  MESSAGE( "**** CyberGloveLogger failed: CGS_VirtualHandDevice.dll not found for this platform" )
  RETURN()
ENDIF()


IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  IF( MSVC )
    SET( CGS_LIB_DIR ${BCI2000_EXTENSION_DIR}/extlib/cgs/lib/winnt_386/Release )
    SET( BCI2000_SIGSRC_LIBS
       ${BCI2000_SIGSRC_LIBS}
       "${CGS_LIB_DIR}/CGS_VirtualHandDevice.lib"
       "${CGS_LIB_DIR}/CGS_VirtualHandRegistry.lib"
    )
  ENDIF()
ENDIF()

IF( NOT CGS_LIB_DIR )
  MESSAGE( "**** CyberGloveLogger failed: CGS_VirtualHandDevice and CGS_VirtualHandRegistry libraries not found for this platform/compiler" )
  RETURN()
ENDIF()

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/extlib/include/vhandtk/
   ${BCI2000_EXTENSION_DIR}/extlib/include/vtidm/
   ${BCI2000_EXTENSION_DIR}/CyberGloveInputLogger.cpp
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/CGS_VirtualHandDevice.dll
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/CGS_VirtualHandRegistry.dll
   ${BCI2000_EXTENSION_DIR}/extlib/dylib/CGS_DeviceManagerCore.dll
)
IF( WIN32 ) # header conflict
  ADD_DEFINITIONS( -D_WINCRYPT_NO_DPAPI )
ENDIF()
