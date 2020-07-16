#################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: User-configurable build options

# General build options
if( WIN32 )
  OPTION( USE_APPLICATION_BUNDLES "Set to OFF in case of problems with application bundles" ON )
else()
  set( USE_APPLICATION_BUNDLES OFF )
endif()

option( USE_DYNAMIC_CRT "Dynamically link to C/C++ runtime libraries" OFF )

OPTION( USE_OPENMP "Set to OFF to disable OpenMP-based multithreading" ON )

OPTION( USE_ASSERTS_IN_RELEASE_BUILDS "Enable Assert() and SuggestDebugging() in release builds" )
IF( USE_ASSERTS_IN_RELEASE_BUILDS )
  ADD_DEFINITIONS( -DTINY_ASSERT )
ENDIF()

SET( BUILD_OPTIONS
  ${BUILD_OPTIONS}
  USE_DYNAMIC_CRT
  USE_OPENMP
  USE_PRECOMPILED_HEADERS
  USE_ASSERTS_IN_RELEASE_BUILDS
)  

IF( CMAKE_SYSTEM_PROCESSOR MATCHES ".*86" AND CMAKE_SIZEOF_VOID_P EQUAL 4 )
  OPTION( USE_SSE2 "Set to OFF when building for older processors" ON )
  list( APPEND BUILD_OPTIONS USE_SSE2 )
ELSE()
  SET( USE_SSE2 OFF )
ENDIF()

# Whether to include certain projects
OPTION( BUILD_DEMOS "Build demo projects" OFF )
option( BUILD_ALL_TESTS "Build ${PROJECT_NAME} tests plus extlib tests" OFF )
mark_as_advanced( BUILD_ALL_TESTS )
option( BUILD_TESTS "Build ${PROJECT_NAME} executable tests" OFF )
mark_as_advanced( BUILD_TESTS )

option( BUILD_CONTRIB "Build all modules from the contrib section" OFF )
option( BUILD_ALL_SOURCEMODULES "Build all source modules" OFF )
option( BUILD_BCPY2000 "Build BCPy2000 modules" OFF )
IF( EXISTS ${PROJECT_SRC_DIR}/private )
  IF( $ENV{BCI2000_NO_PRIVATE} )
    SET( onoff_ OFF )
  ELSE()
    SET( onoff_ ON )
  ENDIF()
  option( BUILD_PRIVATE "Build contents of src/private directory" ${onoff_} )
ENDIF()
IF( EXISTS ${PROJECT_SRC_DIR}/custom )
  option( BUILD_CUSTOM "Build contents of src/custom directory" ON )
ENDIF()

if( NOT BUILD_MODULES MATCHES "fixed;.*" )
  SET( mod_ "core" )
  IF( BUILD_CONTRIB )
    LIST( APPEND mod_ "contrib" )
  ENDIF()
  IF( BUILD_ALL_SOURCEMODULES )
    LIST( APPEND mod_ "SignalSource" )
  ENDIF()
  IF( BUILD_BCPY2000 )
    LIST( APPEND mod_ "BCPy2000" )
  ENDIF()
endif()
SET( BUILD_MODULES ${mod_} CACHE STRING "List of regular expressions matching subdirectory names" FORCE )
mark_as_advanced( BUILD_MODULES )

IF( BUILD_ALL_TESTS )
  SET( BUILD_TESTS ON )
ENDIF()
IF(NOT BUILD_TESTS )
  ADD_DEFINITIONS( -DDISABLE_UNITTEST )
ENDIF()

# Buildtests for external libraries
SET( buildtests_ SNDFILE PORTAUDIO )
FOREACH( test_ ${buildtests_} )
  OPTION( ${test_}_BUILD_TESTS "Enable build tests for the ${test_} library" OFF )
  MARK_AS_ADVANCED( FORCE ${test_}_BUILD_TESTS )
  IF( BUILD_ALL_TESTS )
    SET( ${test_}_BUILD_TESTS ON )
  ENDIF()
  IF( ${test_}_BUILD_TESTS )
    ENABLE_TESTING()
  ENDIF()
ENDFOREACH()

# MSVC specific
IF( MSVC )
  OPTION( BUILD_MFC "Build modules that use MFC" OFF )
ELSE()
  SET( BUILD_MFC OFF )
ENDIF()

SET( folders_ ON )
option( BUILD_USE_SOLUTION_FOLDERS "Enable target group folders" ${folders_} )
mark_as_advanced( BUILD_USE_SOLUTION_FOLDERS )

