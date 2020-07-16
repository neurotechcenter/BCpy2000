###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including the udis86 lib

SET( EXTLIB_OK TRUE )
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/udis86/libudis86/udis86.c
  ${PROJECT_SRC_DIR}/extlib/udis86/libudis86/itab.c
  ${PROJECT_SRC_DIR}/extlib/udis86/libudis86/decode.c
  ${PROJECT_SRC_DIR}/extlib/udis86/libudis86/syn.c
  ${PROJECT_SRC_DIR}/extlib/udis86/libudis86/syn-att.c
  ${PROJECT_SRC_DIR}/extlib/udis86/libudis86/syn-intel.c
)
SET( HDR_EXTLIB ${PROJECT_SRC_DIR}/extlib/udis86/udis86.h )
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/udis86 )
