###########################################################################
## $Id: SetupExtlibDependencies.cmake 5581 2016-12-02 21:01:12Z mellinger $
## Authors: griffin.milsap@gmail.com, mellinger@neurotechcenter.org
## Description: Modifies the framework and sets up libraries to be linked

macro(BCI2000_APPLY_EXTLIB_INCLUDE inName ioSources ioLibs ioFailed)

  IF( NOT EXISTS ${PROJECT_UTILS_DIR}/extlib/${inName}.cmake )
    MESSAGE( FATAL_ERROR
      "Unknown extlib dependency: ${inName}."
      "Make sure all BCI2000_INCLUDE statements have a corresponding .cmake file in src/extlib."
    )
  ELSE()
    SET( EXTLIB_OK FALSE )
    UNSET( INC_EXTLIB )
    UNSET( HDR_EXTLIB )
    UNSET( SRC_EXTLIB )
    UNSET( LIBDIR_EXTLIB )
    UNSET( LIBS_EXTLIB )
    INCLUDE( ${PROJECT_UTILS_DIR}/extlib/${inName}.cmake )
    IF( EXTLIB_OK )
      if(NOT "${ioSources} " STREQUAL " ")
        SET( ${ioSources}
          ${${ioSources}}
          ${SRC_EXTLIB}
          ${HDR_EXTLIB}
        )
      endif()
      INCLUDE_DIRECTORIES( ${INC_EXTLIB} )
      LINK_DIRECTORIES( ${LIBDIR_EXTLIB} )
      SET( ${ioLibs}
        ${${ioLibs}}
        ${LIBS_EXTLIB}
      )
    ELSE( EXTLIB_OK )
      utils_warn( "Could not satisfy dependency on ${inName} extlib." )
      SET( ${ioFailed}
        ${${ioFailed}}
        ${inName}
      )
    ENDIF( EXTLIB_OK )
  ENDIF()

endmacro()

# Sets up the extlib dependencies by looping through the BCI2000_INCLUDING var
macro( BCI2000_SETUP_EXTLIB_DEPENDENCIES ioSrc _ignored ioLibs ioFailed )
  SET( ${ioFailed} "" )
  if(BCI2000_INCLUDING)
    list(REMOVE_DUPLICATES BCI2000_INCLUDING)
    if(BCI2000_USING)
      list(REMOVE_ITEM BCI2000_USING ${BCI2000_INCLUDING})
    endif()
    FOREACH( _inc ${BCI2000_INCLUDING} )
      BCI2000_APPLY_EXTLIB_INCLUDE(${_inc} ${ioSrc} ${ioLibs} ${ioFailed})
    ENDFOREACH()
    set( BCI2000_INCLUDING )
  endif()
  if(BCI2000_USING)
    list(REMOVE_DUPLICATES BCI2000_USING)
    FOREACH( _inc ${BCI2000_USING} )
      BCI2000_APPLY_EXTLIB_INCLUDE(${_inc} "" ${ioLibs} ${ioFailed})
    ENDFOREACH()
    set( BCI2000_USING )
  endif()
endmacro()

# Add an EXTLIB dependency to ${BCI2000_INCLUDING}
FUNCTION( BCI2000_INCLUDE LIB_LC )
  STRING( TOUPPER ${LIB_LC} LIB )
  SET( BCI2000_INCLUDING 
    ${BCI2000_INCLUDING}
    ${LIB}
    PARENT_SCOPE
  )
ENDFUNCTION() 

# Use an EXTLIB that is contained in a library
function( BCI2000_USE LIB_LC )
  STRING( TOUPPER ${LIB_LC} LIB )
  SET( BCI2000_USING
    ${BCI2000_USING}
    ${LIB}
    PARENT_SCOPE
  )
endfunction()

# Is a given extlib being included?
function(utils_extlib_used inLib outResult)

  set(result_)
  string( TOUPPER ${inLib} lib_ )
  foreach(var_ BCI2000_USING BCI2000_INCLUDING)
    list(FIND ${var_} ${lib_} idx_)
    if(NOT idx_ LESS 0)
      list(APPEND result_ ${var_})
    endif()
  endforeach()
  set(${outResult} ${result_} PARENT_SCOPE)

endfunction()

