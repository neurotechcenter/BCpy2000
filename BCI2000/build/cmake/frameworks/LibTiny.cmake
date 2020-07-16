###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Usage header for LibTiny library

INCLUDE_DIRECTORIES(
  ${PROJECT_SRC_DIR}/shared/utils/Lib
)
IF( WIN32 )
  SET( LIBS ${LIBS} ws2_32 avrt )
ENDIF()
if( crt_link_type STREQUAL STATIC )
  list( APPEND BCI2000_STATIC_LIBS LibTiny_StaticCrt )
  list( REMOVE_ITEM BCI2000_STATIC_LIBS LibTiny_DynamicCrt )
elseif( crt_link_type STREQUAL DYNAMIC )
  list( APPEND BCI2000_STATIC_LIBS LibTiny_DynamicCrt )
  list( REMOVE_ITEM BCI2000_STATIC_LIBS LibTiny_StaticCrt )
else()
  message( FATAL_ERROR "Unexpected crt_link_type value: ${crt_link_type}" )
endif()
ADD_DEFINITIONS( -DHAVE_LIB_TINY )
