###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Adds the CoreMain.cpp source file to a project if no
##   other main file exists.
##

MACRO( BCI2000_ADD_CORE_MAIN NAME )
  SET( sources ${ARGV} )
  LIST( REMOVE_AT sources 0 )
  LIST( FIND sources "${NAME}.cpp" IDX )
  if( NOT IDX EQUAL -1 )
    list( GET sources ${IDX} source_ )
    file( STRINGS ${source_} main_ NEWLINE_CONSUME REGEX "int[ \\n\\t]+main[ \\n\\t]*\\(" )
    if( NOT main_ )
      set( IDX -1 )
    endif()
  endif()
  IF( IDX EQUAL -1 )
    SET( CORE_MAIN ${PROJECT_SRC_DIR}/shared/modules/CoreMain.cpp )
    SET( SRC_BCI2000_FRAMEWORK
      ${SRC_BCI2000_FRAMEWORK}
      ${CORE_MAIN}
    )
  ENDIF()

  SET( SRC_BCI2000_FRAMEWORK
    ${SRC_BCI2000_FRAMEWORK}
    ${PROJECT_SRC_DIR}/shared/modules/CoreModule.cpp
    ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_module.cpp
  )

  utils_extlib_used( QT needqt_ )
  IF( needqt_ )
    SET( SRC_BCI2000_FRAMEWORK
      ${SRC_BCI2000_FRAMEWORK}
      ${PROJECT_SRC_DIR}/shared/modules/CoreModule_Qt.cpp
    )
  ENDIF()    
ENDMACRO()  
