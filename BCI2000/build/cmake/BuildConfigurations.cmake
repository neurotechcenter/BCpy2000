###########################################################################
## $Id: BuildConfigurations.cmake 5563 2016-10-12 13:52:49Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Sets a number of default build options for makefile
## generators.

if( CMAKE_CONFIGURATION_TYPES )
  set( CMAKE_CONFIGURATION_TYPES Release;RelWithDebInfo;Debug CACHE STRING "" FORCE )
  mark_as_advanced( CMAKE_CONFIGURATION_TYPES )
endif()

macro( init_cross_compilation )
  # Required for 32 bit mex files to build on 64-bit-Windows
  # seems not to be set up properly by CMake
  if( CMAKE_SYSTEM_PROCESSOR STREQUAL CMAKE_HOST_SYSTEM_PROCESSOR )

    # no cross compiling set up
    set( x86_64_ x86_64 )
    set( x86_ i686 )

    if( WIN32 ) # names depend on OS
      set( x86_64_ AMD64 )
      set( x86_ x86 )
    elseif( APPLE )
      set( x86_ i386 )
    endif()
    
    # fix obvious mismatches
    if( CMAKE_SIZEOF_VOID_P EQUAL 4 AND CMAKE_SYSTEM_PROCESSOR MATCHES .*64.* )
      set( CMAKE_SYSTEM_PROCESSOR ${x86_} )
    elseif( CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT CMAKE_SYSTEM_PROCESSOR MATCHES .*64.* )
      set( CMAKE_SYSTEM_PROCESSOR ${x86_64_} )
    endif()

  endif()

endmacro()

macro( init_static_linking )
  # Prepare for switching between dynamic vs static linking of runtime libraries.
  set( lang_flags_
     CMAKE_CXX_FLAGS
     CMAKE_C_FLAGS
  )
  set( linker_flags_
     CMAKE_EXE_LINKER_FLAGS
     CMAKE_SHARED_LINKER_FLAGS
     CMAKE_MODULE_LINKER_FLAGS
  )
  set( LANG_FLAG_VARS_ ${lang_flags_} )
  set( LINKER_FLAG_VARS_ ${linker_flags_} )

  foreach( config_type_ ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER "${config_type_}" config_type_ )
    foreach( lang_flag_ ${lang_flags_} )
      list( APPEND LANG_FLAG_VARS_ "${lang_flag_}_${config_type_}" )
    endforeach()
    foreach( linker_flag_ ${linker_flags_} )
      list( APPEND LINKER_FLAG_VARS_ "${linker_flag_}_${config_type_}" )
    endforeach()
  endforeach()

  foreach( flag_name_ ${LANG_FLAG_VARS_} )
    set( ${flag_name_}_DYNAMIC "${${flag_name_}}" )
    compiler_dynamic_to_static_crt_lang_flag( ${flag_name_}_DYNAMIC ${flag_name_}_STATIC )
  endforeach()

  foreach( flag_name_ ${LINKER_FLAG_VARS_} )
    set( ${flag_name_}_DYNAMIC "${${flag_name_}}" )
    compiler_dynamic_to_static_crt_linker_flag( ${flag_name_}_DYNAMIC ${flag_name_}_STATIC )
  endforeach()

endmacro()

macro( set_crt_link_type link_type_ )
    foreach( flag_name_ ${LANG_FLAG_VARS_} ${LINKER_FLAG_VARS_} )
      set( ${flag_name_} "${${flag_name_}_${link_type_}}" )
    endforeach()
    set( crt_link_type ${link_type_} )
endmacro()

IF( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES Clang )
  SET( COMPILER_IS_GCC_COMPATIBLE 1 )
ENDIF()
  
IF( COMPILER_IS_GCC_COMPATIBLE )
  # Set a default build type for single-configuration
  # CMake generators if no build type is set.
  IF( NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE RELEASE )
  ENDIF( NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE )

  macro( compiler_dynamic_to_static_crt_lang_flag dynamic_ static_ )
    set( ${static_} "${${dynamic_}}" )
  endmacro()

  if( APPLE )
    macro( compiler_dynamic_to_static_crt_linker_flag dynamic_ static_ )
      set( ${static_} "${${dynamic_}}" )
    endmacro()
  else()
    macro( compiler_dynamic_to_static_crt_linker_flag dynamic_ static_ )
      set( ${static_} "${${dynamic_}} -static-libgcc -static-libstdc++" )
    endmacro()
  endif()

  macro( apply_build_configuration_compiler_specific )
    # Select flags.
    SET( cflags_
      "-include \"${PROJECT_SRC_DIR}/shared/config/gccprefix.h\""
    )
    IF( USE_SSE2 )
      LIST( APPEND cflags_ -msse2 )
    ENDIF()
    IF( USE_OPENMP )
      LIST( APPEND cflags_ -openmp )
    ENDIF()

    IF( NOT WIN32 )
      SET( cflags_
        ${cflags_}
        -fPIC
        -fvisibility=hidden
      )
    ENDIF()
  
    SET( cflags_
      ${cflags_}
      -Wstrict-aliasing
      -Wno-endif-labels
      -Wno-multichar
    )

    FOREACH( flag_ ${cflags_} )
      SET( CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} ${flag_}"
      )
      SET( CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} ${flag_}"
      )
    ENDFOREACH()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set( CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g" )

    IF( WIN32 )
      SET( lflags_
        -Wl,--no-undefined
        -Wl,--enable-stdcall-fixup
      )
    ELSEIF( APPLE )
      SET( lflags
        -dead_strip
      )
    ELSE()
      SET( lflags_
        -Wl,--no-undefined
      )
    ENDIF()

    FOREACH( var_ EXE;SHARED;MODULE )
      FOREACH( flag_ ${lflags_} )
        SET( CMAKE_${var_}_LINKER_FLAGS
          "${CMAKE_${var_}_LINKER_FLAGS} ${flag_}"
        )
      ENDFOREACH()
    ENDFOREACH()

  endmacro( apply_build_configuration_compiler_specific )

ENDIF( COMPILER_IS_GCC_COMPATIBLE )

# MSVC specific flags
IF( MSVC )

  SET_PROPERTY( GLOBAL PROPERTY USE_FOLDERS ${BUILD_USE_SOLUTION_FOLDERS} )
  
  macro( apply_build_configuration_compiler_specific )
  
    SET( CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} /EHsc"
    )
    IF( USE_SSE2 )
      SET( CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} /arch:SSE2"
      )
      SET( CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} /arch:SSE2"
      )
    ENDIF()

    IF( USE_OPENMP )
      SET( CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} /openmp"
      )
      SET( CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} /openmp"
      )
    ENDIF()

    SET( CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} /D_SECURE_SCL=0"
    )
    ADD_DEFINITIONS(
      /DNOMINMAX
      /D_CRT_SECURE_NO_WARNINGS
      /D_CRT_SECURE_NO_DEPRECATE
      /D_CRT_NONSTDC_NO_WARNINGS
      /D_SCL_SECURE_NO_WARNINGS
      /J # use unsigned chars -- must be enabled
      /W1
      /wd4250 # https://connect.microsoft.com/VisualStudio/feedback/details/733720/inheriting-from-std-fstream-produces-c4250-warning
      /wd4290 # ignored exception specification -- ignore while VS2012 is still supported
      /wd4297 # noexcept function throws -- ignore while VS2012 is still supported
      /wd4355 # 'this' used in base member initializer list -- plain nonsense
      /wd4800 # forcing value to bool 'true' or 'false' (performance warning) -- plain nonsense
      /wd4838 # "narrowing cast required" -- impractical unless you want to clutter everything with static_cast<>
    )
    INCLUDE_DIRECTORIES(
      ${PROJECT_SRC_DIR}/shared/compat
    )
    if( MSVC_VERSION LESS 1800 ) # VS2012 still requires this, unsure about VS2013
      include_directories(${PROJECT_SRC_DIR}/shared/compat/C99Inttypes)
    endif()
    if( MSVC_VERSION GREATER 1899 ) # VS 2015 workaround
      add_definitions( /DTINY_DEBUG_LOCKS=0 )
    endif()

    set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /OPT:REF" )
    
  endmacro( apply_build_configuration_compiler_specific )
  
  macro( compiler_dynamic_to_static_crt_lang_flag dynamic_ static_ )
      if( ${static_} MATCHES ".*REL.*" )
        set( ${static_} "${${dynamic_}} /MT" )
      else()
        set( ${static_} "${${dynamic_}} /MTd" )
      endif()
  endmacro()

  macro( compiler_dynamic_to_static_crt_linker_flag dynamic_ static_ )
    string( REGEX REPLACE "/INCREMENTAL[:YESNO]*" "" temp_ "${${dynamic_}}" )
    set( ${dynamic_} "${temp_} /INCREMENTAL:NO" )
    set( ${static_} "${${dynamic_}} /NODEFAULTLIB:msvcrt /NODEFAULTLIB:msvcrtd /NODEFAULTLIB:msvcprt /NODEFAULTLIB:msvcprtd" )
  endmacro()

ENDIF( MSVC )

add_definitions( -D_USE_MATH_DEFINES )

set( debug_flags_ "-DBCIDEBUG=1 -DDEBUG_BUILD=1 -DTINY_DEBUG=1" )
set( release_flags_ "-DNDEBUG=1" )
foreach( config_type_ ${CMAKE_CONFIGURATION_TYPES} )
  string( TOUPPER "${config_type_}" config_type_ )
  if( config_type_ MATCHES .*REL.* )
    set( flags_ ${release_flags_} )
  else()
    set( flags_ ${debug_flags_} )
  endif()
  set( CMAKE_C_FLAGS_${config_type_} "${CMAKE_C_FLAGS_${config_type_}} ${flags_}" )
  set( CMAKE_CXX_FLAGS_${config_type_} "${CMAKE_CXX_FLAGS_${config_type_}} ${flags_}" )
endforeach()

init_cross_compilation()
apply_build_configuration_compiler_specific()
init_static_linking()
set_crt_link_type( STATIC )

add_definitions( -DCOMPILER_NAME="${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}-${CMAKE_SYSTEM_PROCESSOR}" )

