###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: A macro to add a registry function to a target.
##   In BCI2000, a registry function is a function that references objects
##   created by the registration macros defined in src/config/BCIRegistry.h.
##   Such a function is needed with MSVC to make sure that registration
##   objects contained in a static library will not get stripped during
##   the link stage.
##
##   BCI2000_ADD_REGISTRY( RegistryName SourcesVar DependsVar )
##   A registry with given name is added to the list of source files in
##   SourcesVar, and additional dependencies are returned in the DependsVar
##   variable.

MACRO( BCI2000_ADD_REGISTRY name_ sources_ depends_ )

  SET( REGISTRY_INC ${CMAKE_CURRENT_BINARY_DIR}/${name_}.inc )
  
  ADD_CUSTOM_COMMAND(
    OUTPUT ${REGISTRY_INC}
    COMMAND ${CMAKE_COMMAND} -E echo "// Generated file -- do not edit" > ${REGISTRY_INC}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS  extract_registry
    VERBATIM
  )
  FOREACH( CPP_FILE ${SRC_BCI2000_FRAMEWORK} )
    ADD_CUSTOM_COMMAND(
      OUTPUT ${REGISTRY_INC}
      COMMAND ${BUILDUTILS_DIR}/extract_registry < ${CPP_FILE} >> ${REGISTRY_INC}
      DEPENDS ${CPP_FILE}
      APPEND
      VERBATIM
    )
  ENDFOREACH()

  SET( REGISTRY_CPP
    ${PROJECT_SRC_DIR}/shared/config/BCIRegistry.cpp
  )
  SET_PROPERTY( # Make sure the registry cpp file is re-compiled each time the registry file has changed.
    SOURCE ${REGISTRY_CPP}
    APPEND PROPERTY OBJECT_DEPENDS "${REGISTRY_INC};${INVENTORY_INC}"
  )
  SET( ${sources_}
    ${${sources_}}
    ${INVENTORY_INC}
    ${REGISTRY_INC}
    ${REGISTRY_CPP}
  )
  SOURCE_GROUP( "Generated\\BCI2000 Framework" FILES ${REGISTRY_INC} ${INVENTORY_INC} )

  ADD_DEFINITIONS( "-DREGISTRY_NAME=${name_}" )

  SET( ${depends_}
    ${${depends_}}
    extract_registry
  )

ENDMACRO()


MACRO( FORCE_INCLUDE_OBJECT name_ )
  IF( ( WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4 ) OR APPLE )
    SET( sname_ "_${name_}" )
  ELSE()
    SET( sname_ "${name_}" )
  ENDIF()
  IF( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /include:${sname_}" )
  ELSEIF( COMPILER_IS_GCC_COMPATIBLE )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-u,${sname_}" )
  ENDIF()
ENDMACRO()

