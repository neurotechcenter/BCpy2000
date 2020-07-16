###########################################################################
## $Id$
## Authors: juergen.mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including the EXTLIB Matlab libs

set( matlabdir_ ${PROJECT_SRC_DIR}/extlib/matlab )
set( SRC_EXTLIB
    ${matlabdir_}/imports/libmex.imports.cpp
    ${matlabdir_}/imports/libmx.imports.cpp
)
set( HDR_EXTLIB
    ${matlabdir_}/mex.h
    ${matlabdir_}/mat.h
)
# use #include "matlab/mex.h" to avoid name clashes
set( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib )
if( WIN32 )
  add_definitions( -DDLL_EXPORT_SYM=__declspec\(dllexport\) )
else()
  add_definitions( -DDLL_EXPORT_SYM=__attribute__\(\(visibility\(\"default\"\)\)\) )
endif()
set( EXTLIB_OK TRUE )

