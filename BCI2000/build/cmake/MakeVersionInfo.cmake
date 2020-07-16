###########################################################################
## $Id$
## Authors: juergen.mellinger@neurotechcenter.org
## Description: CMake script determining whether SVN revision has changed

find_package( Subversion QUIET )
if( Subversion_FOUND )

  Subversion_WC_INFO( ${PROJECT_SOURCE_ROOT} svn )
  set( PROJECT_REVISION ${svn_WC_LAST_CHANGED_REV} )
  set( PROJECT_DATE ${svn_WC_LAST_CHANGED_DATE} )
  set( PROJECT_VERSION ${LATEST_RELEASE}.${PROJECT_REVISION} )
  # try to get PROJECT_DATE as UTC in ISO 8601 format
  execute_process(
    COMMAND ${Subversion_SVN_EXECUTABLE} info --xml ${PROJECT_SOURCE_ROOT}
    OUTPUT_VARIABLE info_
  )
  if( info_ MATCHES ".*<date>([^>.]*)(\\.[0-9]*)?Z</date>.*" )
    set( PROJECT_DATE "${CMAKE_MATCH_1}Z" )
  endif()
  # check for local modifications
  execute_process(
    COMMAND ${Subversion_SVN_EXECUTABLE} status --quiet ${PROJECT_SOURCE_ROOT}
    OUTPUT_VARIABLE modifications_
  )
  string( REGEX MATCHALL "\n" modifications_ "${modifications_}" )
  list( LENGTH modifications_ modifications_ )
  if( modifications_ )
    if( BUILD_MINIMIZE_REBUILDS )
      set( PROJECT_REVISION "${PROJECT_REVISION}.?" )
      set( PROJECT_VERSION "${PROJECT_VERSION}.-1" )
    else()
      set( PROJECT_REVISION "${PROJECT_REVISION}.${modifications_}" )
      set( PROJECT_VERSION "${PROJECT_VERSION}.${modifications_}" )
    endif()
  endif()
  
else()

  message( "SVN command line tools not found, cannot include revision info." )
  set( PROJECT_REVISION "N/A" )
  set( PROJECT_DATE "N/A" )
  set( PROJECT_VERSION ${LATEST_RELEASE} )
endif()

set( BUILD_VERSION ${PROJECT_VERSION} )
if( NOT BUILD_LOCAL_VERSION VERSION_EQUAL 0 )
  set( BUILD_VERSION "${BUILD_VERSION}:${BUILD_LOCAL_VERSION}" )
endif()

macro( parse_version version_var_name_ )
  string( REPLACE "." ";" version_ "${${version_var_name_}}" )
  set( comps_ MAJOR MINOR PATCH TWEAK )
  foreach( comp_ ${comps_} )
    set( ${version_var_name_}_${comp_} 0 )
  endforeach()
  set( idx_ 0 )
  foreach( field_ ${version_} )
    list( GET comps_ ${idx_} comp_ )
    set( value_ ${field_} )
    string( REGEX REPLACE "[^0-9]" "" value_ ${value_} )
    set( ${version_var_name_}_${comp_} ${value_} )
    math( EXPR idx_ "${idx_}+1" )
  endforeach()
endmacro()
parse_version( PROJECT_VERSION )
parse_version( BUILD_LOCAL_VERSION )

if( BUILD_MINIMIZE_REBUILDS )
  string( TIMESTAMP NOW "%Y-%m-%dZ" UTC )
  message( STATUS "BUILD_DATE set to ${NOW}" )
else()
  string( TIMESTAMP NOW UTC )
  message(
    "BUILD_DATE set to ${NOW} "
    "(enable BUILD_MINIMIZE_REBUILDS to reduce number of rebuilds)"
  )
endif()

get_filename_component( version_dir_ ${VERSION_CHECK} DIRECTORY )
file( GLOB version_sources_ ${TEMPLATE_DIR}/Version.*.in )
foreach( file_ ${version_sources_} )
  get_filename_component( name_ ${file_} NAME )
  string( REPLACE ".in" "" name_ ${name_} )
  set( configured_ ${version_dir_}/${name_} )
  configure_file( ${file_} ${configured_} @ONLY ESCAPE_QUOTES )
endforeach()

