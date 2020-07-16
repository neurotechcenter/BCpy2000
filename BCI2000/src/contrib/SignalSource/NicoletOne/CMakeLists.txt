###########################################################################
## $Id: CMakeLists.txt 5360 2016-05-18 18:03:16Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for the Viasys NicoletOne Amp
##              This module 

# Unfortunately, this lib being COM based, we can't do this with ANYTHING
# but Visual Studio.  Bleh.
IF( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4 )

# Set the executable name
SET( EXECUTABLE_NAME NicoletOne )

# Set the project specific sources
SET( SRC_PROJECT
  NicoletOne.cpp
  WINMAIN.cpp
  LiveData.cpp
  SocketImplDR.cpp
  NicoletOneADC.cpp
  StdAfx.cpp
  NicoletThread.cpp
  Lock.cpp
  resource.h
  ${PROJECT_SRC_DIR}/shared/modules/CoreMain.cpp
  ${PROJECT_SRC_DIR}/shared/modules/CoreModule.cpp
  ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_module.cpp
)

# Define the library
LINK_DIRECTORIES( ${PROJECT_SRC_DIR}/contrib/SignalSource/NicoletOne/lib/Libraries )
INCLUDE_DIRECTORIES( ${PROJECT_SRC_DIR}/contrib/SignalSource/NicoletOne/lib/Include )

set_crt_link_type( DYNAMIC )
SET( USE_PRECOMPILED_HEADERS OFF )

# We need to define wchar_t as not defined by the compiler.
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-" )
# NicoletSDKd requires special flags
set( CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:libcmtd /SAFESEH:NO" )

# Set this up as an MFC Project
ADD_DEFINITIONS( -D_AFXDLL -D_ATL_ALLOW_CHAR_UNSIGNED )
SET( CMAKE_MFC_FLAG 2 )

list( APPEND LIBS ${BCI2000_SIGSRC_LIBS} )
UTILS_INCLUDE( frameworks/LibTiny )
UTILS_INCLUDE( frameworks/Core )
INCLUDE_DIRECTORIES(
  ${PROJECT_SRC_DIR}/shared/modules/signalsource
)
ADD_DEFINITIONS( -DIS_FIRST_MODULE )

BCI2000_ADD_TARGET( INFO "Signal source module" WINAPP ${EXECUTABLE_NAME} ${SRC_PROJECT} ${BCI2000_SIGSRC_FILES} )

IF( NOT FAILED )
  BCI2000_ADD_TO_INVENTORY( SignalSource ${EXECUTABLE_NAME} )
  TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} winmm debug NicoletSDKd optimized NicoletSDK  )
ENDIF( NOT FAILED )

ENDIF()
