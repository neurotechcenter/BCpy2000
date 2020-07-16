###########################################################################
## $Id: Core.cmake 5751 2018-05-07 15:14:39Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Usage header for BCI2000FrameworkCore library

UTILS_INCLUDE( frameworks/LibTiny )

INCLUDE_DIRECTORIES(
  ${PROJECT_SRC_DIR}/shared
  ${PROJECT_SRC_DIR}/shared/accessors
  ${PROJECT_SRC_DIR}/shared/bcistream
  ${PROJECT_SRC_DIR}/shared/config
  ${PROJECT_SRC_DIR}/shared/modules
  ${PROJECT_SRC_DIR}/shared/filters
  ${PROJECT_SRC_DIR}/shared/types
  ${PROJECT_SRC_DIR}/shared/utils
  ${PROJECT_SRC_DIR}/shared/utils/Lib
  ${PROJECT_SRC_DIR}/shared/utils/Expression
  ${PROJECT_SRC_DIR}/shared/utils/Scripting
  ${PROJECT_SRC_DIR}/shared/fileio
  ${PROJECT_SRC_DIR}/shared/fileio/dat
)
ADD_DEFINITIONS( -DHAVE_CORELIB )

#SET( REGISTRY_NAME CoreRegistry )
#FORCE_INCLUDE_OBJECT( ${REGISTRY_NAME} )

if( crt_link_type STREQUAL STATIC )
  list( APPEND BCI2000_STATIC_LIBS BCI2000FrameworkCore_StaticCrt )
  list( REMOVE_ITEM BCI2000_STATIC_LIBS BCI2000FrameworkCore_DynamicCrt )
elseif( crt_link_type STREQUAL DYNAMIC )
  list( APPEND BCI2000_STATIC_LIBS BCI2000FrameworkCore_DynamicCrt )
  list( REMOVE_ITEM BCI2000_STATIC_LIBS BCI2000FrameworkCore_StaticCrt )
else()
  message( FATAL_ERROR "Unexpected crt_link_type value: ${crt_link_type}" )
endif()

IF( NOT DEFINED USE_PRECOMPILED_HEADERS OR USE_PRECOMPILED_HEADERS )
  UNSET( pchsrc_ )
  SET( CORE_PCH PCHIncludes.h )
  IF( MSVC )
    SET( pchsrc_ ${CMAKE_CURRENT_BINARY_DIR}/CorePCH.cpp )
    IF( NOT EXISTS ${pchsrc_} )
      FILE( WRITE "${pchsrc_}" "#include \"${CORE_PCH}\"" )
    ENDIF()
    SET( pchfile_ "$(OutDir)/$(TargetName)_Core.pch" )
    SET_SOURCE_FILES_PROPERTIES(
      ${pchsrc_} PROPERTIES
      COMPILE_FLAGS "/Yc\"${CORE_PCH}\" /Fp\"${pchfile_}\""
      OBJECT_OUTPUTS "\"${pchfile_}\""
    )
    SET( CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} /Yu\"${CORE_PCH}\" /FI\"${CORE_PCH}\" /Fp\"${pchfile_}\""
    )
  ELSEIF( 0 ) # COMPILER_IS_GCC_COMPATIBLE )
    SET( pchsrc_ ${PROJECT_SRC_DIR}/shared/config/${CORE_PCH} )
    IF( NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${CORE_PCH}" )
      FILE( WRITE "${CMAKE_CURRENT_BINARY_DIR}/${CORE_PCH}" "#error Not using precompiled header file." )
    ENDIF()
    SET_SOURCE_FILES_PROPERTIES(
      ${pchsrc_} PROPERTIES
      COMPILE_FLAGS "-x c++ -o \"${CMAKE_CURRENT_BINARY_DIR}/${CORE_PCH}.gch\""
      OBJECT_OUTPUTS "${CORE_PCH}.gch"
    )
    SET( CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -I \"${CMAKE_CURRENT_BINARY_DIR}\" -include ${CORE_PCH}"
    )
  ENDIF()
  IF( pchsrc_ )
    OPTION( USE_PRECOMPILED_HEADERS "Set to OFF in case of linking or runtime problems" ON )
    LIST( APPEND SRC_BCI2000_FRAMEWORK ${pchsrc_} )
    SOURCE_GROUP( "Generated\\BCI2000 Framework" FILES ${pchsrc_} )
    ADD_DEFINITIONS( -DPRECOMPILED_HEADERS=1 )
  ENDIF()
ENDIF()
