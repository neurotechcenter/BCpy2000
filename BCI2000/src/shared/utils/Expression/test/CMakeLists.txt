###########################################################################
## $Id: CMakeLists.txt 5342 2016-04-29 14:43:51Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for Calculator

IF( BUILD_TESTS )

SET( DIR_NAME Tests/Expressions )
BCI2000_ADD_TOOLS_CMDLINE( 
  Calculator
  Calculator.cpp
  FALSE
  OUTPUT_DIRECTORY ${PROJECT_BUILD_ROOT}/Tests
)

ENDIF( BUILD_TESTS )
