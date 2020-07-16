###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including LibAV in a project

if(MSVC)
  set(USE_DYNAMIC_LIBAV_DEFAULT OFF)
elseif(WIN32 OR APPLE)
  set(USE_DYNAMIC_LIBAV_DEFAULT ON)
else()
  set(USE_DYNAMIC_LIBAV_DEFAULT OFF)
endif()
option( USE_DYNAMIC_LIBAV "Dynamically link to libav libraries" ${USE_DYNAMIC_LIBAV_DEFAULT} )
add_definitions( -D__STDC_CONSTANT_MACROS )
set( INC_EXTLIB "${PROJECT_SRC_DIR}/extlib/libav/include" )
set( LIBS_EXTLIB )
set( HDR_EXTLIB )
set( SRC_EXTLIB )

if( USE_DYNAMIC_LIBAV )

  file( GLOB libavfiles_ "${INC_EXTLIB}/../imports/*" )
  list( APPEND SRC_EXTLIB ${libavfiles_} )
  if( WIN32 )
    utils_get_extlib( libav_ libav 11.7 )
    if( libav_ )
      file( GLOB libavfiles_ "${libav_}/usr/bin/*.dll" )
      foreach( _R0 ${libavfiles_} )
        list( APPEND SRC_EXTLIB COMPANION ${_R0} )
      endforeach()
      set( EXTLIB_OK TRUE )
    endif()
  endif()

else()

  # TODO: needs cleanup/unification with dynamically linked version
  if( WIN32 )
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      set( arch_ win32-amd64 )
    else()
      set( arch_ win32-x86 )
    endif()
  endif()
  file( GLOB libs_ RELATIVE "${INC_EXTLIB}" "${INC_EXTLIB}/*" )
  foreach( lib_ ${libs_} )
    if( IS_DIRECTORY "${INC_EXTLIB}/${lib_}" )
      list( APPEND LIBS_EXTLIB ${lib_} )
      if( NOT TARGET ${lib_} )
        utils_import_lib( "${INC_EXTLIB}/../${arch_}/static/${lib_}" ${lib_} )
      endif()
    endif()
  endforeach()
  set( EXTLIB_OK TRUE )

endif()


