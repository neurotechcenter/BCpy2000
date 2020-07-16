###########################################################################
## $Id$
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
## Description: Defines a static library containing a basic BCI2000 framework

set( NAME BCI2000FrameworkCore_StaticCrt )
set_crt_link_type( STATIC )
utils_include( frameworks/Core )
utils_include( frameworks/CoreLib/Files )
bci2000_add_target( INFO "Framework library" STATIC_LIBRARY ${NAME} ${SRC_LIBCORE} )
if( FAILED )
  message( ERROR "Could not satisfy dependencies of the core library." )
else( FAILED )
  target_link_libraries( ${NAME} ${DEPLIBS} )
endif( FAILED )
