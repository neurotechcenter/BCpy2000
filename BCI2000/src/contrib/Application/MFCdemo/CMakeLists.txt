###########################################################################
## $Id: CMakeLists.txt 5342 2016-04-29 14:43:51Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for the MFCDemo

IF( BUILD_DEMOS )

# This being MFC, this app will only build in MSVC.
IF( MSVC )

# Set the executable name
SET( EXECUTABLE_NAME MFCDemo )

# Set the project specific sources
SET( SRC_PROJECT
  MFCdemo.cpp
  MFCdemoDlg.cpp
  WINMAIN.cpp
  TaskFilter.cpp 
  StdAfx.cpp
  resource.h
  MFCdemo.rc
)

# Set this up as an AFX project
ADD_DEFINITIONS( -D_AFXDLL -D_ATL_ALLOW_CHAR_UNSIGNED )
SET( CMAKE_MFC_FLAG 2 )
set_crt_link_type( DYNAMIC )
SET( USE_PRECOMPILED_HEADERS OFF )

UTILS_INCLUDE( frameworks/LibTiny )
# list( APPEND LIBS ${BCI2000_APP_LIBS} )
UTILS_INCLUDE( frameworks/Core )
ADD_DEFINITIONS( -DIS_LAST_MODULE -DMODTYPE=3 ) 

# Add the global and specific application files into the framework for this project
SET( SRC_BCI2000_FRAMEWORK
  ${SRC_BCI2000_FRAMEWORK}
#  ${BCI2000_APP_FILES}
  ${BCI2000_SRC_DIR}/shared/modules/CoreModule.cpp
  ${BCI2000_SRC_DIR}/shared/bcistream/BCIStream_module.cpp
)

BCI2000_ADD_TARGET( INFO "MFC demo application module" WINAPP ${EXECUTABLE_NAME} ${SRC_PROJECT} )

IF( NOT FAILED )

  BCI2000_ADD_TO_INVENTORY( Application ${EXECUTABLE_NAME} )
  TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME} winmm )

  # Set the project build folder
  SET_PROPERTY( TARGET ${EXECUTABLE_NAME} PROPERTY FOLDER Demos/MFC )

ENDIF( NOT FAILED )

ENDIF( MSVC )

ENDIF( BUILD_DEMOS )
