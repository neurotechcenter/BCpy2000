###########################################################################
## $Id$
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: CMake utility functions

### Defines and options
set( ENV{LANG} C ) # avoid translations
if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
  set( UTILS_ARCH_BITS 32 )
elseif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set( UTILS_ARCH_BITS 64 )
else()
  message( FATAL_ERROR "Cannot handle pointer size of ${CMAKE_SIZEOF_VOID_P} bytes" )
endif()

set( UTF8_SHIBBOLETH "ÜŤƑȢ" ) # detect character encoding issues
add_definitions( -DUTF8_SHIBBOLETH=\"${UTF8_SHIBBOLETH}\" )
set( PROJECT_UTILS_DIR ${CMAKE_CURRENT_LIST_DIR} )

### Make sure revision info is up-to-date
option( BUILD_MINIMIZE_REBUILDS "Omit build information that may change frequently" OFF )
set( BUILD_LOCAL_VERSION "0.0" CACHE STRING "Local version" )

set( version_dir_ ${PROJECT_BUILD_DIR}/version )
set( VERSION_SOURCES
  ${version_dir_}/Version.cpp
)
if( WIN32 )
  list( APPEND VERSION_SOURCES ${version_dir_}/Version.rc )
endif()

set( VERSION_CHECK ${version_dir_}/VersionCheck )
file( WRITE ${VERSION_CHECK} "/* Dummy file used to force build information update */" )
set( make_version_info_
  COMMAND ${CMAKE_COMMAND}
  -DPROJECT_NAME=${PROJECT_NAME}
  -DPROJECT_TIMESPAN=${PROJECT_TIMESPAN}
  -DLATEST_RELEASE=${LATEST_RELEASE}
  -DPROJECT_SOURCE_ROOT=${PROJECT_SOURCE_ROOT}
  -DCMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}
  -DVERSION_CHECK=${VERSION_CHECK}
  -DTEMPLATE_DIR=${PROJECT_SOURCE_ROOT}/src/shared/config
  -DBUILD_MINIMIZE_REBUILDS=${BUILD_MINIMIZE_REBUILDS}
  -DBUILD_LOCAL_VERSION=${BUILD_LOCAL_VERSION}
  -P ${CMAKE_CURRENT_LIST_DIR}/MakeVersionInfo.cmake
)
add_custom_target( VersionCheck DEPENDS ${VERSION_CHECK} BYPRODUCTS ${VERSION_SOURCES} ${make_version_info_} )
set_target_properties( VersionCheck PROPERTIES FOLDER Build )
execute_process( ${make_version_info_} )
include( ${version_dir_}/Version.cmake )

# Create project defines
STRING( TOLOWER "${PROJECT_NAME}" domain_ )
SET( PROJECT_DOMAIN "${domain_}.org" CACHE STRING "Domain name of main project" )
SET( PROJECT_SEARCH_ENGINE "google" CACHE STRING "Name of search engine for searching project domain" )
MARK_AS_ADVANCED( PROJECT_DOMAIN PROJECT_SEARCH_ENGINE )
ADD_DEFINITIONS(
  -DPROJECT_NAME="${PROJECT_NAME}"
  -DPROJECT_TIMESPAN="${PROJECT_TIMESPAN}"
  -DPROJECT_DOMAIN="${PROJECT_DOMAIN}"
  -DPROJECT_VERSION="${PROJECT_VERSION}"
  -DWEBSEARCH_DOMAIN="www.${PROJECT_SEARCH_ENGINE}.com"
)

# Determine host and user name
IF( NOT CONFIG_BUILD_USER )
  set( site_ )
  IF( CMAKE_HOST_WIN32 )
    EXECUTE_PROCESS( COMMAND net config workstation RESULT_VARIABLE result_ OUTPUT_VARIABLE output_ )
    IF( result_ EQUAL 0 )
      SET( pat_ ".*\nFull Computer name.([^\n]+).*" )
      IF( output_ MATCHES ${pat_} )
        STRING( REGEX REPLACE  ${pat_} "\\1" site_ ${output_} )
        STRING( STRIP "${site_}" site_ )
      ENDIF()
    ENDIF()
  ELSE()
    execute_process( COMMAND hostname -f RESULT_VARIABLE result_ OUTPUT_VARIABLE site_ )
    if( result_ EQUAL 0 )
      string( STRIP "${site_}" site_ )
    else()
      site_name( site_ )
    endif()
  ENDIF()
  SET( user_ "$ENV{USER}" )
  IF( user_ STREQUAL "" )
    SET( user_ "$ENV{USERNAME}" )
  ENDIF()
  IF( user_ STREQUAL "" )
    SET( user_ "unknown" )
  ENDIF()
  SET( CONFIG_BUILD_USER "${user_}@${site_}" CACHE STRING "Build user ID, may be used to track down the origin of executables" )
  UNSET( site_ CACHE )
ENDIF()
ADD_DEFINITIONS(
  -DBUILD_USER="${CONFIG_BUILD_USER}"
)

set( PROJECT_ICON_TEMPLATE "" CACHE STRING "Icon template in PNG format" )
set( PROJECT_ICON_STYLE "" CACHE STRING "Icon style" )
mark_as_advanced( PROJECT_ICON_TEMPLATE PROJECT_ICON_STYLE )
set( MAKE_ICON ${CMAKE_CURRENT_LIST_DIR}/MakeIcon.cmake )
set( UTILS_ICON_TARGETS "" CACHE INTERNAL "" )

macro( utils_set_appicon icon_ )
 if(WIN32)
  if( PROJECT_ICON_TEMPLATE AND NOT APPICON_DEFINED )
    set( APPICON_DEFINED TRUE )
    set( APPICON_SOURCES )

    get_filename_component( iconame_ ${icon_} NAME )
    get_filename_component( icodir_ "${PROJECT_BUILD_DIR}/icons" ABSOLUTE )
    set( pngicon_ "${icodir_}/${iconame_}256.png" )
    set( make_pngicon_ 
          -DBUILDUTILS_DIR=${BUILDUTILS_DIR}
          -DPROJECT_ICON_TEMPLATE=${PROJECT_ICON_TEMPLATE}
          -DSTYLE=${PROJECT_ICON_STYLE}
          -DICON=${icon_}
          -DOUTPUT=${pngicon_}
          -P ${MAKE_ICON}
    )
    execute_process(
      COMMAND ${CMAKE_COMMAND} -DPREFLIGHT=TRUE ${make_pngicon_}
      RESULT_VARIABLE err_ OUTPUT_VARIABLE info_ ERROR_VARIABLE errinfo_
    )
    if( err_ )
      if( NOT info_ )
        set( info_ "${errinfo_}" )
      endif()
      message( "Could not set application icon:\n${info_}" )
    else()
      add_custom_command(
        OUTPUT ${pngicon_}
        WORKING_DIRECTORY ${icodir_}
        DEPENDS transform_png combine_png ${MAKE_ICON}
        COMMAND ${CMAKE_COMMAND} ${make_pngicon_}
      )
      if( NOT WIN32 )
        utils_add_resource( AppIcon256 ${pngicon_} )
      else()
        set( rcfile_ ${CMAKE_CURRENT_BINARY_DIR}/appicon.rc )
        set( icofile_ "${icodir_}/${iconame_}.ico" )
        file( WRITE ${rcfile_} "#pragma code_page(65001)\n1 ICON \"${icofile_}\"\n" )
        set( makeico_
          WORKING_DIRECTORY ${icodir_}
          DEPENDS make_win32ico transform_png png2bmp ${pngicon_}
        )
        set( pngfiles_ )
        set( bmpfiles_ )
        foreach( size_ 16 32 48 )
          list( APPEND makeico_
            COMMAND
            ${BUILDUTILS_DIR}/transform_png --width=${size_} --height=${size_}
            "${pngicon_}" "${iconame_}${size_}.png"
            COMMAND
            ${BUILDUTILS_DIR}/png2bmp 
            "${iconame_}${size_}.png" > "${iconame_}${size_}.bmp"
          )
          list( APPEND pngfiles_ "${iconame_}${size_}.png" )
          list( APPEND bmpfiles_ "${iconame_}${size_}.bmp" )
        endforeach()
        list( APPEND makeico_
            COMMAND ${BUILDUTILS_DIR}/make_win32ico ${bmpfiles_} ${pngicon_} > ${icofile_}
            COMMAND ${CMAKE_COMMAND} -E remove ${pngfiles_} ${bmpfiles_}
        )
        set( icotarget_ icon-${iconame_} )
        list( FIND UTILS_ICON_TARGETS ${icotarget_} idx_ )
        if( idx_ LESS 0 )
          add_custom_target( ${icotarget_} ${makeico_} )
          set_target_properties( ${icotarget_} PROPERTIES FOLDER "Build/Icons" )
          set( UTILS_ICON_TARGETS ${UTILS_ICON_TARGETS} ${icotarget_} CACHE INTERNAL "" )
        endif()
        set_source_files_properties( ${icofile_} PROPERTIES GENERATED TRUE )
        list( APPEND APPICON_SOURCES
          ${rcfile_}
          ${icofile_}
          DEPENDS ${icotarget_}
        )
      endif()
      source_group( "Generated\\AppIcon" FILES ${APPICON_SOURCES} )
    endif()
  endif()
 endif(WIN32)
endmacro()

# Conditionally emit a status message
OPTION( CONFIG_VERBOSE "Set to OFF to suppress status information during configuration" ON )
FUNCTION( UTILS_CONFIG_STATUS )
  IF( CONFIG_VERBOSE )
    MESSAGE( STATUS ${ARGV} )
  ENDIF()
ENDFUNCTION()

function( utils_warn )
  message( "WARNING:\t" ${ARGV} )
endfunction()

FUNCTION( UTILS_FATAL_ERROR )
  IF( CONFIG_VERBOSE )
    MESSAGE( ${ARGV} "\n" )
  ENDIF()
  MESSAGE( FATAL_ERROR ${ARGV} "\n" )
ENDFUNCTION()

# Deal with CMake policy warnings
SET( PROJECT_CMAKE_POLICIES_OLD
)
FOREACH( policy_ ${PROJECT_CMAKE_POLICIES_OLD} )
  IF( POLICY CMP${policy_} )
    UTILS_CONFIG_STATUS( "Silencing CMP${policy_} policy warning" )
    CMAKE_POLICY( SET CMP${policy_} OLD )
  ENDIF()
ENDFOREACH()

SET( PROJECT_CMAKE_POLICIES_NEW
  0020 # don't link to qtmain.lib automatically
  0022 # INTERFACE_LINK_LIBRARIES
  0038 # don't allow targets to link to themselves
  0040 # target must exist
  0042 # MACOSX_RPATH enabled by default
  0043 # COMPILE_DEFINITIONS_<CONFIG>
)
FOREACH( policy_ ${PROJECT_CMAKE_POLICIES_NEW} )
  IF( POLICY CMP${policy_} )
    UTILS_CONFIG_STATUS( "Silencing CMP${policy_} policy warning" )
    CMAKE_POLICY( SET CMP${policy_} NEW )
  ENDIF()
ENDFOREACH()

### Functions

# Include a file from the utils directory
MACRO( UTILS_INCLUDE file_ )
  INCLUDE( ${PROJECT_UTILS_DIR}/${file_}.cmake )
ENDMACRO()

# Parse arguments into provided variable names
MACRO( UTILS_PARSE_ARGS args_ )

  SET( firstargs_ ${args_} )
  LIST( LENGTH firstargs_ pos_ )
  IF( ${ARGC} LESS pos_ ) 
    MESSAGE( FATAL_ERROR "Too few arguments: args_ = \"${args_}\", ARGN = \"${ARGN}\"" )
  ENDIF()
  MATH( EXPR pos_ "${pos_}-1" )
  LIST( GET firstargs_ ${pos_} lastarg_ )
  LIST( REMOVE_AT firstargs_ ${pos_} )

  SET( remargs_ ${ARGN} )
  FOREACH( arg_ ${firstargs_} )
    LIST( GET remargs_ 0 ${arg_} )
    LIST( REMOVE_AT remargs_ 0 )
  ENDFOREACH()
  SET( ${lastarg_} )
  FOREACH( arg_ ${remargs_} )
    LIST( APPEND ${lastarg_} ${arg_} )
  ENDFOREACH()

ENDMACRO()

function( utils_extract_named_elements ioList inName outElements )

  set( list_ ${${ioList}} )
  set( found_ )
  list( FIND list_ "${inName}" idx )
  while( NOT idx EQUAL -1 )
    list( REMOVE_AT list_ ${idx} )
    list( GET list_ ${idx} next_ )
    list( APPEND found_ "${next_}" )
    list( REMOVE_AT list_ ${idx} )
    list( FIND list_ "${inName}" idx )
  endwhile()
  set( ${ioList} ${list_} PARENT_SCOPE )
  set( ${outElements} ${found_} PARENT_SCOPE )
  
endfunction()

function( utils_extract_flag ioList inFlag outFound )

  set( list_ ${${ioList}} )
  set( found_ FALSE )
  list( FIND list_ "${inFlag}" idx )
  if( NOT idx EQUAL -1 )
    set( found_ TRUE )
    list( REMOVE_AT list_ ${idx} )
  endif()
  set( ${ioList} ${list_} PARENT_SCOPE )
  set( ${outFound} ${found_} PARENT_SCOPE )
  
endfunction()

# Move directories into a separate list
function( utils_extract_directories ioList outDirectories )

  foreach( file_ ${${ioList}} )
    get_filename_component( abs_ ${file_} ABSOLUTE )
    if( IS_DIRECTORY "${abs_}" )
      list( APPEND dirs_ "${abs_}" )
    else()
      list( APPEND files_ "${abs_}" )
    endif()
  endforeach()
  if( DEFINED files_ )
    list( REMOVE_DUPLICATES files_ )
    set( ${ioList} ${files_} PARENT_SCOPE )
  endif()
  if( DEFINED dirs_ )
    list( REMOVE_DUPLICATES dirs_ )
  endif()
  set( ${outDirectories} ${dirs_} PARENT_SCOPE )

endfunction()

# Automatically add header files
FUNCTION( UTILS_AUTOHEADERS ioList )

  FOREACH( file_ ${${ioList}} )
    IF( file_ MATCHES .*\\.\(c|cpp|cc\)$ )
      STRING( REGEX REPLACE \(c|cpp|cc\)$ h header_ "${file_}" )
      IF( EXISTS ${header_} OR EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${header_} )
        LIST( APPEND files_ "${header_}" )
      ENDIF()
    ENDIF()
    LIST( APPEND files_ "${file_}" )
  ENDFOREACH()
  IF( DEFINED files_ )
    LIST( REMOVE_DUPLICATES files_ )
    SET( ${ioList} ${files_} PARENT_SCOPE )
  ENDIF()
  
ENDFUNCTION()

# Automatically handle Qt preprocessing
FUNCTION( UTILS_AUTOMOC ioList outNeedqt )

  SET( generated )
  FOREACH( file_ ${${ioList}} )
    IF( file_ MATCHES .*\\.h$ )
      FILE( STRINGS ${file_} qobjects REGEX [^/]*Q_OBJECT.* )
      LIST( LENGTH qobjects qobjects )
      IF( qobjects GREATER 0 )
        QT_WRAP_CPP( generated ${file_} )
      ENDIF()
    ELSEIF( file_ MATCHES .*\\.ui$ )
      QT_WRAP_UI( generated ${file_} )
    ELSEIF( file_ MATCHES .*.\\qrc$ )
      QT_ADD_RESOURCES( generated ${file_} )
    ENDIF()
  ENDFOREACH()
  set( nonascii_regex_ ".*[^\\x00-\\x7F].*" )
  if( WIN32 )
    if( CMAKE_CURRENT_SOURCE_DIR MATCHES ${nonascii_regex_}
        OR CMAKE_CURRENT_BINARY_DIR MATCHES ${nonascii_regex_} )
      # Source directory contains non-ascii characters. This is a problem because
      # qt_wrap_cpp() writes moc parameter files in utf-8 encoding, but
      # moc will then use QString::fromLocal8Bit() to read the same data.
      # As a workaround, we replace filenames in that file with their 8.3 forms,
      # which consist of ascii characters.
      file( GLOB_RECURSE mocfiles_ ${CMAKE_CURRENT_BINARY_DIR}/moc_*_parameters )
      set( failed_ )
      foreach( file_ IN LISTS mocfiles_ )
        file( STRINGS ${file_} content_ ENCODING UTF-8 )
        if( content_ MATCHES ".*${UTF8_SHIBBOLETH}.*" )
          execute_process(
            COMMAND cmd /d /q /c fixup_mocparams.cmd ${file_}
            WORKING_DIRECTORY ${PROJECT_UTILS_DIR}/../buildutils
            RESULT_VARIABLE result_
          )
          if( NOT result_ EQUAL 0 )
            set( failed_ TRUE )
          endif()
        endif()
      endforeach()
      if( failed_ )
        message( FATAL_ERROR
          "Failed to fix up moc file(s) in ${CMAKE_CURRENT_BINARY_DIR}.\n"
          "Please make sure that both source and build directory contain"
          " only ascii characters in their names: A-Z a-z 0-9 _ are ok, \n"
          " dot and space character may be present as well."
        )
      endif()
    endif()
  endif( WIN32 )

  SET( needqt FALSE )
  IF( generated )
    SOURCE_GROUP( Generated\\Qt FILES ${generated} )
    SET( ${ioList} ${${ioList}} ${generated} PARENT_SCOPE )
    SET( needqt TRUE )
  ELSEIF( ${QT_QTCORE_INCLUDE_DIR} )
    UTILS_IS_INCLUDED( ${QT_QTCORE_INCLUDE_DIR} needqt )
  ENDIF()
  SET( ${outNeedqt} ${needqt} PARENT_SCOPE )
  
ENDFUNCTION()

# Construct a reasonable compiler id
function( utils_compiler_id outId )

  string(REPLACE "." ";" version_ "${CMAKE_CXX_COMPILER_VERSION}")
  list(GET version_ 0 major_)
  list(GET version_ 1 minor_)
  if(NOT minor_)
    set(minor_ "0")
  endif()
  string(TOLOWER "${CMAKE_CXX_COMPILER_ID}-${major_}.${minor_}" id_)
  set(${outId} ${id_} PARENT_SCOPE)

endfunction()

# Work around IDE/CMake bugs
FUNCTION( UTILS_FIXUP_FILES listname_ )

  IF( MSVC ) # With the VS2010 generator, there are problems 
             # with non-compiled files appearing in multiple projects.
             # As a workaround, we construct globally unique paths for such files.
    GET_FILENAME_COMPONENT( proj_dir_ "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE )
    GET_FILENAME_COMPONENT( proj_dir_name_ "${CMAKE_CURRENT_SOURCE_DIR}" NAME )
    SET( files_ )
    FOREACH( file_ ${${listname_}} )
      IF( NOT file_ MATCHES  "\\.(h|c|cpp)$" )
        GET_FILENAME_COMPONENT( file_ "${file_}" ABSOLUTE )
        FILE( RELATIVE_PATH rpath_ "${proj_dir_}" "${file_}" )
        FILE( TO_CMAKE_PATH "${rpath_}" rpath_ )
        SET( file_ "../${proj_dir_name_}/${rpath_}" )
      ENDIF()
      LIST( APPEND files_ "${file_}" )
    ENDFOREACH()      
    SET( ${listname_} ${files_} PARENT_SCOPE )
  ENDIF()

ENDFUNCTION()

# Check if a preprocessor define exists in the current directory
FUNCTION( UTILS_IS_DEFINITION inDef outResult )

  GET_DIRECTORY_PROPERTY( defs COMPILE_DEFINITIONS )  
  SET( ${outResult} FALSE PARENT_SCOPE )
  UTILS_REGEX_ESCAPE( inDef )
  FOREACH( def ${defs} )
    IF( def MATCHES "^${inDef}$" OR def MATCHES "^${inDef}=.*" )
      SET( ${outResult} TRUE PARENT_SCOPE )
      RETURN()
    ENDIF()
  ENDFOREACH()

ENDFUNCTION()

# Check whether a path is being included
FUNCTION( UTILS_IS_INCLUDED inPath outResult )

  SET( ${outResult} FALSE PARENT_SCOPE )
  IF( "${inPath}" STREQUAL "" )
    RETURN()
  ENDIF()
  
  GET_FILENAME_COMPONENT( path "${inPath}" ABSOLUTE )
  GET_DIRECTORY_PROPERTY( dirs INCLUDE_DIRECTORIES )
  FOREACH( dir ${dirs} )
    GET_FILENAME_COMPONENT( dir "${dir}" ABSOLUTE )
    IF( dir STREQUAL path )
      SET( ${outResult} TRUE PARENT_SCOPE )
      RETURN()
    ENDIF()
  ENDFOREACH()

ENDFUNCTION()

# Add a flag to an existing target
FUNCTION( UTILS_ADD_FLAG name_ flag_ )
  GET_PROPERTY( flags_ TARGET ${name_} PROPERTY COMPILE_FLAGS )
  SET_PROPERTY( TARGET ${name_} PROPERTY COMPILE_FLAGS "${flags_} ${flag_}" )
ENDFUNCTION()

# Escape special regex metacharacters with a backslash
FUNCTION( UTILS_REGEX_ESCAPE ioVar )
  STRING( REGEX REPLACE "([$^.[|*+?()]|])" "\\\\\\1" result "${${ioVar}}" )
  SET( ${ioVar} ${result} PARENT_SCOPE )
ENDFUNCTION()

# Conditionally add a subdirectory
FUNCTION( UTILS_MATCH_SUBDIR inExpr inDir )
  STRING( TOLOWER "${CMAKE_CURRENT_SOURCE_DIR}/${inDir}//" dir )
  FOREACH( expr ${${inExpr}} )
    STRING( TOLOWER ${expr} expr )
    IF( "${dir}" MATCHES ".*/${expr}/.*" )
      ADD_SUBDIRECTORY( ${inDir} )
      RETURN()
    ENDIF()
  ENDFOREACH()
ENDFUNCTION()


# Get an external file from a local cache, or from the net.
FUNCTION( UTILS_GET_EXTERNAL inFile ioTempVar )

  MESSAGE( STATUS "Looking for ${inFile} ..." )
  SET( cachedfile "${PROJECT_BUILD_DIR}/download_cache/${inFile}" )
  SET( can_connect FALSE )
  IF( EXISTS "${cachedfile}" )
    UTILS_CONFIG_STATUS( "... found locally at ${cachedfile}" )
  ELSE()
    SET( uribase "${PROJECT_DOMAIN}/externals" )
    SET( urlbase "http://${uribase}" )
    UTILS_CONFIG_STATUS( "Trying http ..." )
    FILE( DOWNLOAD "${urlbase}" "${cachedfile}" TIMEOUT 3 STATUS result )
    IF( NOT result EQUAL 0 )
      SET( urlbase "https://${uribase}" )
      UTILS_CONFIG_STATUS( "Trying https ..." )
      FILE( DOWNLOAD "${urlbase}" "${cachedfile}" TIMEOUT 3 STATUS result )
    ENDIF()
    SET( url "${urlbase}/${inFile}" )
    IF( result EQUAL 0 )
      SET( can_connect TRUE )
      FILE( DOWNLOAD "${url}" "${cachedfile}" TIMEOUT 900 INACTIVITY_TIMEOUT 900 STATUS result SHOW_PROGRESS )
    ELSE()
      UTILS_CONFIG_STATUS( "Trying svn ..." )
      EXECUTE_PROCESS( COMMAND svn list "${urlbase}" TIMEOUT 60 RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET  )
      IF( result EQUAL 0 )
        SET( can_connect TRUE )
        EXECUTE_PROCESS( COMMAND svn list "${url}" TIMEOUT 60 RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET  )
        IF( result EQUAL 0 )
          FILE( WRITE "${cachedfile}" "" ) # ensure parent directory
          UTILS_CONFIG_STATUS( "... found at ${url}, downloading ..." )
          EXECUTE_PROCESS( COMMAND svn cat "${url}" TIMEOUT 900 RESULT_VARIABLE result OUTPUT_FILE "${cachedfile}" ERROR_QUIET )
        ENDIF()
      ENDIF()
    ENDIF()
    IF( NOT result EQUAL 0 )
      FILE( REMOVE "${cachedfile}" )
      IF( can_connect )
        UTILS_CONFIG_STATUS( "... not found." )
      ELSE()
        UTILS_CONFIG_STATUS( "... connection failed." )
      ENDIF()
    ENDIF()
  ENDIF()

  IF( EXISTS "${cachedfile}" )
    EXECUTE_PROCESS( COMMAND "${CMAKE_COMMAND}" -E copy "${cachedfile}" "${${ioTempVar}}" RESULT_VARIABLE result )
    if( NOT result EQUAL 0 ) # race condition in CMake?
      execute_process( COMMAND "${CMAKE_COMMAND}" -E sleep 1 )
      execute_process( COMMAND "${CMAKE_COMMAND}" -E copy "${cachedfile}" "${${ioTempVar}}" RESULT_VARIABLE result )
    endif()
  ELSEIF( can_connect )
    SET( ${ioTempVar} NOTFOUND PARENT_SCOPE )
  ELSE()
    SET( ${ioTempVar} CANTCONNECT-NOTFOUND PARENT_SCOPE )
  ENDIF()
  
ENDFUNCTION()

# Get an external library
function( utils_get_extlib outPath inName inVersion )

set( arch_ ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR} )
set( path_ "${PROJECT_BUILD_DIR}/extlib/${inName}-${inVersion}" )
set( outpath_ ${path_} )
set( lib_ "${inName}-${inVersion}-${arch_}" )
if( NOT EXISTS ${path_}/.extracted )
  set( libtemp_ "${path_}/${lib_}.tmp" )
  utils_get_external( ${inName}/${lib_} libtemp_ )
  if(NOT libtemp_)
    utils_compiler_id( cid_ )
    set( libtemp_ "${path_}/${lib_}.tmp" )
    utils_get_external( ${inName}/${lib_}-${cid_} libtemp_ )
  endif()
  if( libtemp_ )
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E tar x ${libtemp_}
      WORKING_DIRECTORY ${path_}
      RESULT_VARIABLE err_
    )
    execute_process( COMMAND ${CMAKE_COMMAND} -E remove ${libtemp_} )
    if( NOT err_ )
      execute_process( COMMAND ${CMAKE_COMMAND} -E touch ${path_}/.extracted )
    endif()
  endif()
endif()
if( EXISTS ${path_}/.extracted )
  file( GLOB contents_ LIST_DIRECTORIES false RELATIVE ${path_} "${path_}/*" )
  list( LENGTH contents_ countWithoutDirs_ )
  file( GLOB contents_ LIST_DIRECTORIES true RELATIVE ${path_} "${path_}/*" )
  list( LENGTH contents_ countWithDirs_ )
  if( ${countWithoutDirs_} EQUAL 1 AND ${countWithDirs_} EQUAL 2 )
    list( REMOVE_ITEM contents_ ".extracted" )
    set( outpath_ "${path_}/${contents_}" )
  endif()
  set( ${outPath} ${outpath_} PARENT_SCOPE )
else()
  set( ${outPath} NOTFOUND PARENT_SCOPE )
endif()

endfunction()


# Create an imported library target.
FUNCTION( UTILS_IMPORT_LIB inFile inTarget )

  if(NOT TARGET ${inTarget})
    add_library( "${inTarget}" MODULE IMPORTED )
    set_property( TARGET "${inTarget}" APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE )
    set_target_properties( "${inTarget}" PROPERTIES IMPORTED_LOCATION_RELEASE "${inFile}.lib" )
    set_property( TARGET "${inTarget}" APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG )
    set_target_properties( "${inTarget}" PROPERTIES IMPORTED_LOCATION_DEBUG "${inFile}d.lib" )
  endif()

ENDFUNCTION()

# get_filename_component( ... EXT ) gives us the longest extension
function( utils_get_shortest_ext outExt inFile )

  string( REGEX MATCH "\\.[^.]*$" ext_ "${inFile}" )
  set( ${outExt} ${ext_} PARENT_SCOPE )

endfunction()

# construct a path name that is safe to use in source files
function( utils_safe_path outVar inPath )

  # may be obsolete once we switched to VS2015
  set( safe_ ${inPath} )
  if( MSVC )
    execute_process(
      COMMAND cmd /q /d /c for %a in ("${inPath}") do echo %~sdpa%~nxa
      OUTPUT_VARIABLE safe_
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    file( TO_CMAKE_PATH "${safe_}" safe_ )
  endif()
  set( ${outVar} ${safe_} PARENT_SCOPE )

endfunction()

# recursively scan for files whose names match a pattern
function(utils_scan_for_files outResult inRoot inPattern)
  set(files)
  _utils_scan_for_files(${inRoot} ${inPattern})
  set(${outResult} ${files} PARENT_SCOPE)
endfunction()

macro(_utils_scan_for_files inRoot inPattern)
  file(GLOB children RELATIVE ${inRoot} ${inRoot}/*)
  foreach(child ${children})
    if(IS_DIRECTORY ${inRoot}/${child})
      _utils_scan_for_files(${inRoot}/${child} ${inPattern})
    elseif(child MATCHES ${inPattern})
      set(files ${files} ${inRoot}/${child})
    endif()
  endforeach()
endmacro()

# sort a list of paths by file names
function(utils_sort_by_filename outResult inFiles)
  set(sortfiles)
  foreach(file ${inFiles})
    get_filename_component(dir ${file} DIRECTORY)
    get_filename_component(name ${file} NAME)
    list(APPEND sortfiles "${name}&&&${dir}")
  endforeach()
  list(SORT sortfiles)
  set(files)
  foreach(file ${sortfiles})
    string(REPLACE "&&&" ";" file "${file}")
    list(GET file 0 name)
    list(GET file 1 dir)
    list(APPEND files ${dir}/${name})
  endforeach()
  set(${outResult} ${files} PARENT_SCOPE)
endfunction()

