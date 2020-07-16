###########################################################################
## $Id$
## Authors: juergen.mellinger@neurotechcenter.org
## Description: Sets up CMAKE variables for including the EXTLIB Matlab libs

set( matlabdir_ ${PROJECT_SRC_DIR}/extlib/matlab )
set( SRC_EXTLIB
    ${matlabdir_}/imports/libeng.imports.cpp
    ${matlabdir_}/imports/libmx.imports.cpp
)
set( HDR_EXTLIB
    ${matlabdir_}/engine.h
    ${matlabdir_}/mat.h
)
# use #include "matlab/engine.h" to avoid name clashes
set( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib )
set( EXTLIB_OK TRUE )

