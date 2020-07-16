###########################################################################
## $Id: CMakeLists.txt 5581 2016-12-02 21:01:12Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for OperatorLibDemo

IF( BUILD_DEMOS )

SET( EXECUTABLE_NAME OperatorLibDemo )
set( DIR_NAME Demos/OperatorLib )

SET( SRC_PROJECT
  main.cpp 
  ${BCI2000_SRC_DIR}/shared/bcistream/BCIStream_tool.cpp
  ${BCI2000_SRC_DIR}/core/Operator/OperatorLib/include/BCI_OperatorLib.h
)

if( WIN32 )
  set( OPERATOR_LIB ${PROJECT_BUILD_ROOT}/prog/OperatorLib.dll )
  list( APPEND SRC_PROJECT COMPANION ${OPERATOR_LIB} )
  set_source_files_properties( ${OPERATOR_LIB} PROPERTIES GENERATED TRUE )
endif()

UTILS_INCLUDE( frameworks/Core )
BCI2000_ADD_TO_INVENTORY( "Operator" ${EXECUTABLE_NAME} )  
BCI2000_ADD_TARGET(
  CONSOLEAPP ${EXECUTABLE_NAME}
  ${SRC_PROJECT}
  LIBRARY OperatorLib
)

ENDIF( BUILD_DEMOS )
