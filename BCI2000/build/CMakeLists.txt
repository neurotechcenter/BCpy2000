###########################################################################
## $Id: CMakeLists.txt 5745 2018-04-18 15:29:08Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

cmake_minimum_required( VERSION 3.1 )
cmake_policy( VERSION 2.8 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

project( BCI2000 )
set( LATEST_RELEASE "3.05" )
string( TIMESTAMP PROJECT_TIMESPAN "2000-%Y" UTC )

get_filename_component( PROJECT_SOURCE_ROOT "${CMAKE_SOURCE_DIR}/.." ABSOLUTE )
set( PROJECT_SRC_DIR "${PROJECT_SOURCE_ROOT}/src" )
if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  # in-source build
  set( PROJECT_BUILD_ROOT ${PROJECT_SOURCE_ROOT} )
else()
  # out-of-source build
  get_filename_component( PROJECT_BUILD_ROOT "${CMAKE_BINARY_DIR}/${PROJECT_NAME}" ABSOLUTE )
  file( COPY Configure.sh.cmd DESTINATION . )
endif()
set( PROJECT_ICON_TEMPLATE "${PROJECT_SRC_DIR}/shared/gui/images/brain-outline" CACHE FILEPATH "" )
set( BUILDUTILS_DIR "${CMAKE_BINARY_DIR}/buildutils" )
set( PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR}/CMakeFiles )
set( PROJECT_OUTPUT_DIR "${PROJECT_BUILD_ROOT}/prog" )
configure_file( BuildEnv.sh.cmd.in BuildEnv.sh.cmd @ONLY )

INCLUDE( ${PROJECT_SOURCE_ROOT}/build/cmake/Utils.cmake NO_POLICY_SCOPE )
UTILS_INCLUDE( BuildOptions )
UTILS_INCLUDE( SetupBCI2000Macros )
UTILS_INCLUDE( BuildConfigurations )
if( USE_DYNAMIC_CRT )
  set_crt_link_type( DYNAMIC )
else()
  set_crt_link_type( STATIC )
endif()

SET( BCI2000_SRC_DIR "${PROJECT_SRC_DIR}" )

# Tell the user what we're about to setup
MESSAGE( STATUS "Configuring project ${PROJECT_NAME} Version ${PROJECT_VERSION}" )

utils_scan_for_files(globalLists ${PROJECT_SRC_DIR} ".*.CMakeLists.txt")
utils_sort_by_filename(globalLists ${globalLists})
foreach(globalList ${globalLists})
  include(${globalList})
endforeach()

INCLUDE( "${PROJECT_SRC_DIR}/contrib/Extensions/Extensions.cmake" )
IF( EXISTS ${PROJECT_SRC_DIR}/private AND BUILD_PRIVATE )
  INCLUDE( "${PROJECT_SRC_DIR}/private/PrivateConfig.cmake" )
ENDIF()
IF( EXISTS "${PROJECT_SRC_DIR}/custom/CustomConfig.cmake" AND BUILD_CUSTOM )
  INCLUDE( "${PROJECT_SRC_DIR}/custom/CustomConfig.cmake" )
ENDIF()
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_ROOT}/build/cmake/frameworks "${PROJECT_BUILD_DIR}/frameworks" )
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_ROOT}/build/cmake/extlib "${PROJECT_BUILD_DIR}/extlib" )
ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/shared/utils/Expression/test "${PROJECT_BUILD_DIR}/shared" )

# Recurse down into all project subdirectories

# src/private : stuff we use that is not released yet
IF( EXISTS ${PROJECT_SRC_DIR}/private AND BUILD_PRIVATE )
  ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/private "${PROJECT_BUILD_DIR}/private" )
ENDIF()

# the standard distro: build/buildutils, src/core and src/contrib
ADD_SUBDIRECTORY( buildutils "${PROJECT_BUILD_DIR}/buildutils" )
ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/core "${PROJECT_BUILD_DIR}/core" )
ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/contrib "${PROJECT_BUILD_DIR}/contrib" )

# src/custom : a convenient place for users' own development without causing svn chaos
IF( EXISTS ${PROJECT_SRC_DIR}/custom/CMakeLists.txt AND BUILD_CUSTOM )
  SET( saved_ ${DIR_NAME} )
  SET( DIR_NAME "${DIR_NAME}Custom/" )
  ADD_SUBDIRECTORY( ${PROJECT_SRC_DIR}/custom "${PROJECT_BUILD_DIR}/custom" )
  SET( DIR_NAME ${saved_} )
ENDIF()

MARK_AS_ADVANCED( CMAKE_INSTALL_PREFIX )
