###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including the COM library into
##   a project.
## SETS:
##       SRC_EXTLIB - Required source files
##       HDR_EXTLIB - Required header files
##       INC_EXTLIB - Include directory
##       LIBDIR_EXTLIB - Library directory
##       LIBS_EXTLIB - required libraries

IF( WIN32 )

# Define the source files
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/com/ComModule.def
  ${PROJECT_SRC_DIR}/extlib/com/ComModule.cpp
  ${PROJECT_SRC_DIR}/extlib/com/ComClassFactory.cpp
  ${PROJECT_SRC_DIR}/extlib/com/ComRegistrar.cpp
  ${PROJECT_SRC_DIR}/extlib/com/ComStrings.cpp
)

# Define the headers
SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/com/ComModule.h
  ${PROJECT_SRC_DIR}/extlib/com/ComClassFactory.h
  ${PROJECT_SRC_DIR}/extlib/com/ComCoClass.h
  ${PROJECT_SRC_DIR}/extlib/com/ComRegistrar.h
  ${PROJECT_SRC_DIR}/extlib/com/ComStrings.h
  ${PROJECT_SRC_DIR}/extlib/com/ComPtr.h
)

# Define the include directory
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/com )

# Define libraries
SET( LIBS_EXTLIB
  shlwapi
)

# Set success
SET( EXTLIB_OK TRUE )

ELSE( WIN32 )

  utils_warn( "COM is only supported on windows." )
  SET( EXTLIB_OK FALSE )

ENDIF( WIN32 )
