###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including the udis86 lib

SET( EXTLIB_OK TRUE )
add_definitions( -DUDIS86_NOSYN )
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/udis86/libudis86/udis86.c
  ${PROJECT_SRC_DIR}/extlib/udis86/libudis86/itab.c
  ${PROJECT_SRC_DIR}/extlib/udis86/libudis86/decode.c
)
SET( HDR_EXTLIB ${PROJECT_SRC_DIR}/extlib/udis86/udis86.h )
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/udis86 )

