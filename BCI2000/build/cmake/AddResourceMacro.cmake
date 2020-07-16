###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: A macro to add a binary file as a resource.

# Definition of resource list file:
SET( RESOURCE_NAMES )
SET( RESOURCE_FILES )

# Macro definitions
MACRO( utils_add_resource RESNAME FILE )
  SET( RESOURCE_NAMES
    ${RESOURCE_NAMES}
    ${RESNAME}
  )
  GET_FILENAME_COMPONENT( file_ "${FILE}" ABSOLUTE )
  SET( RESOURCE_FILES
    ${RESOURCE_FILES}
    ${file_}
  )
ENDMACRO()


MACRO( utils_setup_resources target_ SOURCES )

  SET( RESOURCE_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/${target_} )
  SET( RESOURCES_INC "${CMAKE_CURRENT_BINARY_DIR}/${target_}/Resources.inc" )

  INCLUDE_DIRECTORIES(
    ${RESOURCE_INCLUDES}
  )
  SET( RESOURCES_H
    ${PROJECT_SRC_DIR}/shared/config/Resources.h
  )
  SET_PROPERTY(
    SOURCE ${RESOURCES_INC}
    PROPERTY GENERATED TRUE
  )
  FILE( WRITE ${RESOURCES_INC} "// File contents created by utils_add_resource -- re-run CMake to update this file\n" )
  SET( RES_CPPS )
  LIST( LENGTH RESOURCE_NAMES NRES )
  IF( NRES )
    MATH( EXPR NRES_1 "${NRES} - 1" )
    FOREACH( IDX RANGE ${NRES_1} )
    
      LIST( GET RESOURCE_NAMES ${IDX} RESNAME )
      IF( ${RESNAME} EQUAL "AUTO" )
        SET( RESNAME "_${IDX}" )
      ENDIF()
      LIST( GET RESOURCE_FILES ${IDX} FILE )
      GET_FILENAME_COMPONENT( file_ ${FILE} ABSOLUTE )
      get_filename_component( RES_CPP ${FILE} NAME )
      SET( RES_CPP ${CMAKE_CURRENT_BINARY_DIR}/${target_}/${RES_CPP}.res.cpp )
      GET_FILENAME_COMPONENT( res_cpp_ ${RES_CPP} ABSOLUTE )
      GET_FILENAME_COMPONENT( dir_ ${res_cpp_} PATH )
      FILE( MAKE_DIRECTORY ${dir_} )
      utils_safe_path( res_cpp_safe_ ${res_cpp_} )
      SET( RES_CPPS
        ${RES_CPPS}
        "${res_cpp_safe_}"
      )
      FILE( APPEND ${RESOURCES_INC} "\#include \"${res_cpp_safe_}\"\n" )
      ADD_CUSTOM_COMMAND(
        OUTPUT "${res_cpp_safe_}"
        COMMAND "${BUILDUTILS_DIR}/create_resource" "${RESNAME}" "${FILE}" > ${res_cpp_safe_}
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        DEPENDS create_resource "${file_}"
        VERBATIM
      )
    ENDFOREACH()
  ENDIF( NRES )
  SET( ${SOURCES}
    ${${SOURCES}}
    ${RES_CPPS}
    ${RESOURCES_INC}
    ${RESOURCES_H}
    ${RESOURCE_FILES}
  )
  SOURCE_GROUP( "Resources\\" FILES ${RESOURCE_FILES} )
  SOURCE_GROUP( "Generated\\Resources" FILES ${RESOURCES_INC} ${RES_CPPS} )
ENDMACRO()

MACRO( utils_setup_resources_post target_ )
  SET( RESOURCE_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/${target_} )
  get_property(_R0 DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
  list(REMOVE_ITEM _R0 ${RESOURCE_INCLUDES})
  set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES ${_R0})
ENDMACRO()

