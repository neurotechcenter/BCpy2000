###########################################################################
## $Id: SetupExtlibDependencies.cmake 3541 2011-09-13 15:33:18Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Macro that simplifies BCI2000 CMake files

# Helper macros not going elsewhere
MACRO( QT_WRAP_CPP )
  if( QT_AVAILABLE )
    QT5_WRAP_CPP( ${ARGN} )
  endif()
ENDMACRO()

MACRO( QT_WRAP_UI )
  IF( QT_AVAILABLE )
    QT5_WRAP_UI( ${ARGN} )
  ENDIF()
ENDMACRO()

MACRO( QT_ADD_RESOURCES )
  IF( QT_AVAILABLE )
    QT5_ADD_RESOURCES( ${ARGN} )
  ENDIF()
ENDMACRO()

# Counting targets
SET( ENV{TARGET_NUMBER} 0 )
FUNCTION( NEXT_TARGET outTargetNumber )
  MATH( EXPR target "$ENV{TARGET_NUMBER} + 1" )
  SET( ENV{TARGET_NUMBER} ${target} )
  SET( ${outTargetNumber} ${target} PARENT_SCOPE )
ENDFUNCTION()
  
# Debugging targets
SET( DebugCMake_Target 0 CACHE STRING "Target number to be debugged, 0 for none" )
SET( DebugCMake_DumpProperties
  "DIRECTORY\;COMPILE_DEFINITIONS;CMAKE\;VARIABLES"
  CACHE STRING "Properties to display when debugging"
)
MARK_AS_ADVANCED( DebugCMake_Target DebugCMake_DumpProperties )

MACRO( dump_debug_info_ )
  MESSAGE( "======= Dumping debug info for target #${target}: ${kind_} ${name_} =======" )
  FOREACH( _prop ${DebugCMake_DumpProperties} )
    MESSAGE( "======= ${_prop} =======" )
    LIST( GET _prop 0 _type )
    LIST( GET _prop 1 _name )
    IF( _type STREQUAL CMAKE )
      GET_CMAKE_PROPERTY( _vars ${_name} )
    ELSE()
      GET_PROPERTY( _vars ${_type} PROPERTY ${_name} )
    ENDIF()
    FOREACH( _var ${_vars}) 
      IF( DEFINED ${_var} )
        MESSAGE( "${_var}==${${_var}}" )
      ELSE()
        MESSAGE( "${_var}" )
      ENDIF()
    ENDFOREACH()
  ENDFOREACH()
  MESSAGE( "======= End of debug info for ${name_} =======" )
ENDMACRO()

# Add any kind of target
FUNCTION( BCI2000_ADD_TARGET )
  set( sources_ ${ARGV} )
  utils_extract_named_elements( sources_ INFO info_ )
  utils_extract_flag( sources_ NOBUNDLE nobundle_ )
  utils_extract_flag( sources_ NOVERSION noversion_ )
  UTILS_PARSE_ARGS( "kind_;name_;sources_" ${sources_} )
  if( kind_ STREQUAL STATIC_LIBRARY )
    set( noversion_ TRUE )
  endif()
  if( info_ MATCHES ".*${name_}.*" )
    set( target_description_ "${info_}" )
  elseif( info_ )
    set( target_description_ "${name_} ${info_}" )
  else()
    set( target_description_ "${name_}" )
  endif()
  if( NOT target_description_ MATCHES ".*${PROJECT_NAME}.*" )
    set( target_description_ "${PROJECT_NAME} ${target_description_}" )
  endif()
  set( bundleable TRUE )
  if( nobundle_ )
    set( bundleable FALSE )
  endif()
  set( use_version_ TRUE )
  if( noversion_ )
    set( use_version_ FALSE )
  endif()

  IF( kind_ STREQUAL WINDLL OR kind_ STREQUAL WINAPP )
    IF( NOT WIN32 )
      SET( FAILED TRUE PARENT_SCOPE )
      RETURN()
    ENDIF()
  ENDIF()
  IF( kind_ STREQUAL WINDLL AND CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( name_ "${name_}64" )
  ENDIF()

  NEXT_TARGET( target )
  SET( debug OFF )
  IF( DebugCMake_Target EQUAL target OR DebugCMake_Target STREQUAL ${name_} )
    SET( debug ON )
  ENDIF()

  if( kind_ STREQUAL QTAPP )
    set( needqt_ TRUE )
  endif()
  IF( NOT needqt_ )
    UTILS_IS_DEFINITION( USE_QT needqt_ )
  ENDIF()
  if(needqt_)
    BCI2000_INCLUDE(QT)
  endif()

  SET( failed_ FALSE )
  BCI2000_SETUP_EXTLIB_DEPENDENCIES( SRC_BCI2000_FRAMEWORK HDR_BCI2000_FRAMEWORK LIBS failed_ )
  IF( NOT failed_ )

    utils_setup_resources( ${name_} SRC_BCI2000_FRAMEWORK )
    SET( sources_ ${sources_} ${SRC_BCI2000_FRAMEWORK} ${HDR_BCI2000_FRAMEWORK} )
    list( APPEND sources_ ${APPICON_SOURCES} )

    BCI2000_AUTODEPEND( sources_ LIBS )

    set(inherited_bundle_)
    foreach( lib_ ${BCI2000_STATIC_LIBS} )
      if(NOT lib_ STREQUAL name_)
        list( APPEND inherited_bundle_ ${${lib_}_BUNDLED_FILES} )
      endif()
    endforeach()
    utils_extract_named_elements( sources_ DEPENDS depends_ )
    utils_extract_named_elements( sources_ LIBRARY libraries_ )
    utils_extract_named_elements( sources_ COMPANION companions )
    utils_extract_named_elements( sources_ SUPPORT support_ )
    utils_extract_named_elements( sources_ OUTPUT_DIRECTORY outputDir )
    utils_extract_directories( sources_ directories_ )
    include_directories( ${directories_} )
    UTILS_AUTOHEADERS( sources_ )
    BCI2000_AUTOSOURCEGROUPS( sources_ companions2_ )
    list( APPEND companions ${companions2_} )
    UTILS_AUTOMOC( sources_ needqt_ )
    BCI2000_AUTOINCLUDE( sources_ )

  ENDIF()

  IF( DEFINED info_ )
    SET( info_ "${info_}, " )
  ENDIF()
  if( failed_ )
    message( "Target #${target}: ${name_} (${info_}${kind_}) not added" )
  endif()

  IF( NOT failed_ )
    IF( NOT DEFINED outputDir )
      SET( outputDir ${PROJECT_OUTPUT_DIR} )
    ENDIF()
    
    set( build_config_ )
    foreach( var_ ${BUILD_OPTIONS} )
      set( build_config_ "${build_config_} ${var_}:${${var_}}" )
    endforeach()
    IF( needqt_ )
      IF( kind_ STREQUAL GUIAPP OR kind_ STREQUAL EXECUTABLE )
        SET( kind_ QTAPP )
      ENDIF()
      SET( build_config_ "${build_config_} Qt:${QT_VERSION}:${QT_QMAKE_EXECUTABLE}" )
      SOURCE_GROUP( "Source Files\\BCI2000 Framework\\extlib\\qt" FILES ${QT_SOURCES} )
      SET( sources_ ${sources_} ${QT_SOURCES} )
    ENDIF()
    add_definitions( -DBUILD_CONFIG=\"${build_config_}\" )

    IF( WIN32 )
      SET( winmain_ ${PROJECT_SRC_DIR}/shared/compat/WinMain.cpp )
      SOURCE_GROUP( "Source Files\\BCI2000 Framework\\shared\\compat" FILES ${winmain_} )
    ELSE()
      SET( winmain_ )
    ENDIF()

    if( use_version_ )
      set( version_sources_ )
      utils_is_definition( HAVE_CORELIB use_version_ )
      if( use_version_ )
        set( version_sources_ ${VERSION_SOURCES} )
      else()
        if( sources_ MATCHES ".*\\.rc.*" )
          foreach( source_ ${VERSION_SOURCES} )
            if( source_ MATCHES ".*\\.rc" )
              list( APPEND version_sources_ ${source_} )
            endif()
          endforeach()
        endif()
        if( sources_ MATCHES ".*VersionInfo\\.cpp.*" )
          foreach( source_ ${VERSION_SOURCES} )
            if( source_ MATCHES ".*\\.cpp" )
              list( APPEND version_sources_ ${source_} )
            endif()
          endforeach()
        endif()
      endif()
      if( version_sources_ )
        set( use_version_ TRUE )
      endif()
      if( use_version_ )
        list( APPEND sources_ ${version_sources_} )
        source_group( "Generated\\Version" FILES ${version_sources_} )
      endif()
    endif()
    set( target_is_dylib_ 0 )
    UTILS_CONFIG_STATUS( "Target #${target}: ${name_} (${info_}${kind_})" )

    # Now, add the target
    IF( debug )
      dump_debug_info_()
    ENDIF()
    
    MACRO( ADD_GUI_EXECUTABLE )
      ADD_EXECUTABLE( ${name_} WIN32 ${sources_} )
#      IF( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4 )
#        SET_PROPERTY( TARGET ${name_} APPEND PROPERTY
#          LINK_FLAGS "/SUBSYSTEM:WINDOWS,5.01" )
#      ENDIF()
    ENDMACRO()
    MACRO( ADD_CONSOLE_EXECUTABLE )
      ADD_EXECUTABLE( ${name_} ${sources_} )
#      IF( MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4 )
#       SET_PROPERTY( TARGET ${name_} APPEND PROPERTY
#        LINK_FLAGS "/SUBSYSTEM:CONSOLE,5.01" )
#      ENDIF()
    ENDMACRO()

    IF( kind_ STREQUAL QTAPP )
      SET( sources_ ${sources_} ${winmain_} )
      ADD_GUI_EXECUTABLE()
      TARGET_LINK_LIBRARIES( ${name_} ${QT_LIBRARIES} )

    ELSEIF( kind_ STREQUAL GUIAPP )
      SET( sources_ ${sources_} ${winmain_} )
      ADD_GUI_EXECUTABLE()

    ELSEIF( kind_ STREQUAL WINAPP )
      ADD_GUI_EXECUTABLE()

    ELSEIF( kind_ STREQUAL CONSOLEAPP )
      ADD_CONSOLE_EXECUTABLE( ${name_} ${sources_} )

    ELSEIF( kind_ STREQUAL EXECUTABLE )
      SET( sources_ ${sources_} ${winmain_} )
      ADD_GUI_EXECUTABLE()
      
    ELSEIF( kind_ STREQUAL STATIC_LIBRARY )
      SET( bundleable FALSE )
      ADD_LIBRARY( ${name_} STATIC ${sources_} )
      SET( outputDir )

    ELSEIF( kind_ STREQUAL MODULE OR kind_ STREQUAL WINDLL )
      set( target_is_dylib_ 1 )
      SET( bundleable FALSE )
      ADD_LIBRARY( ${name_} MODULE ${sources_} )
      SET_PROPERTY( TARGET "${name_}" PROPERTY PREFIX "" )

    ELSEIF( kind_ STREQUAL DLL )
      set( target_is_dylib_ 1 )
      SET( bundleable FALSE )
      ADD_LIBRARY( ${name_} SHARED ${sources_} )
      SET_PROPERTY( TARGET "${name_}" PROPERTY PREFIX "" )
      IF( MINGW )
        SET_PROPERTY( TARGET ${name_} APPEND PROPERTY
          LINK_FLAGS "-Wl,--add-stdcall-alias"
        )
      ENDIF()
      IF( APPLE )
        SET_PROPERTY( TARGET ${name_} APPEND PROPERTY
          LINK_FLAGS "-install_name @executable_path/${name_}.dylib"
        )
      ENDIF()

    ELSE()
      MESSAGE( SEND_ERROR "Unknown target kind: ${kind_}" )
      SET( failed_ TRUE )

    ENDIF()

  ENDIF()

  IF( NOT failed_ )
    utils_setup_resources_post(${name_})
    if( depends_ )
      add_dependencies( ${name_} ${depends_} )
    endif()
    if( use_version_ )
      target_compile_definitions( ${name_} PRIVATE
        -DTARGET_NAME=\"${name_}\"
        -DTARGET_FILE_NAME=\"$<TARGET_FILE_NAME:${name_}>\"
        -DTARGET_IS_DYLIB=${target_is_dylib_}
        -DTARGET_DESCRIPTION="${target_description_}"
      )
      add_dependencies( ${name_} VersionCheck )
      add_custom_command(
        TARGET ${name_}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E touch ${VERSION_CHECK}
      )
    endif()
    target_link_libraries(${name_} ${LIBS} ${libraries_})
    if(NOT kind_ STREQUAL STATIC_LIBRARY)
      # CMAKE policy 0038: don't allow targets to link to themselves
      target_link_libraries(${name_} ${BCI2000_STATIC_LIBS})
    endif()

    SET_PROPERTY( TARGET ${name_} PROPERTY FOLDER ${DIR_NAME} )
    # CMAKE policy 0020: don't link to qtmain.lib automatically
    set_property(TARGET ${name_} PROPERTY Qt5_NO_LINK_QTMAIN ON)

    if( needqt_ )
      target_link_libraries( ${name_} ${QT_LIBRARIES} )
      if( WIN32 AND kind_ STREQUAL QTAPP AND USE_EXTERNAL_QT )
        set( dll_ "platforms/qwindows$<$<CONFIG:Debug>:d>.dll" )
        set( companion_ "${QT_INSTALL_PLUGINS}/${dll_}=qtplugins/${dll_}" )
        list( APPEND companions ${companion_} )
        foreach( module_ IN ITEMS ${QT5_MODULES} Core )
          set( dll_ "Qt5${module_}$<$<CONFIG:Debug>:d>.dll" )
          set( companion_ "${QT_INSTALL_LIBEXECS}/${dll_}" )
          list( APPEND companions ${companion_} )
        endforeach()
      endif()
    endif()

    SET( bundle )
    FOREACH( companion_ ${companions} )
      IF( companion_ MATCHES "\\.bat$" )
        list( APPEND support_ "${companion_}=>/batch" )
      ELSE()
        # in case of .*=.* syntax, this will handle the left side properly
        GET_FILENAME_COMPONENT( companion_ ${companion_} ABSOLUTE )
        file( TO_NATIVE_PATH "${companion_}" companion_ )
        SET( bundle ${bundle} ${companion_} )
      ENDIF()
    ENDFOREACH()
    set(bundle ${bundle} ${inherited_bundle_})
    if( bundle )
      list( REMOVE_DUPLICATES bundle )
    endif()

    foreach( file_ ${support_} )
      if( NOT file_ MATCHES ".*=>.*" )
        get_filename_component( dir_ ${file_} DIRECTORY )
        set( file_ "${file_}=>${dir_}" )
      endif()
      string( REPLACE "=>" ";" file_ ${file_} )
      list( GET file_ 0 from_ )
      list( GET file_ 1 to_ )
      if( from_ MATCHES "^/.*" )
        set( from_ "${PROJECT_SOURCE_ROOT}${from_}" )
      endif()
      if( to_ MATCHES "^/.*" )
        set( to_ "${PROJECT_BUILD_ROOT}${to_}" )
      elseif( NOT IS_ABSOLUTE to_ )
        set( to_ "${outputDir}/${to_}" )
      endif()
      file( GLOB from_ ${from_} )
      set( args_
        TARGET ${name_}
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${to_}"
      )
      foreach( file_ ${from_} )
        set( args_
          ${args_}
          COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${file_}" "${to_}"
        )
      endforeach()
      add_custom_command( ${args_} )
    endforeach()

    if( NOT outputDir )
      set( ${name_}_BUNDLED_FILES ${bundle} CACHE INTERNAL ${name_}_BUNDLED_FILES )
    else()
      IF( bundleable AND WIN32 )
        set( deps_ )
        set( copy_as_ )
        FOREACH( companion_ ${bundle} )
          if( companion_ MATCHES ".*=.*" )
            string( REPLACE "=" ";" companion_ ${companion_} )
            list( GET companion_ 0 from_ )
            list( GET companion_ 1 to_ )
            # cmd shell treats = character as argument separator
            list( APPEND copy_as_ --copy-as ${from_} ${to_} )
          else()
            list( APPEND deps_ ${companion_} )
          endif()
        ENDFOREACH()
        get_property(link_dirs_ DIRECTORY PROPERTY LINK_DIRECTORIES)
        set(redist_dirs)
        foreach(dir_ ${link_dirs_})
          list( APPEND redist_dirs "--add-redist-dir" ${dir_} )
        endforeach()
        add_dependencies( ${name_} list_dependencies )
        set( deploy_app_ ${BUILDUTILS_DIR}/deploy_app )
        if( USE_APPLICATION_BUNDLES )
          add_dependencies( ${name_} AppBundler )
          set( bundlearg_ "--bundle" )
          set( info_ "Create application bundle" )
        else()
          add_dependencies( ${name_} AppCopier )
          set( bundlearg_ "" )
          set( info_ "Deploying executable" )
        endif()
        ADD_CUSTOM_COMMAND(
            TARGET ${name_}
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${outputDir}"
            COMMAND
            "${deploy_app_}" "${outputDir}/${name_}" "$<TARGET_FILE:${name_}>" ${bundlearg_} ${redist_dirs} ${deps_} ${copy_as_}
            || "${CMAKE_COMMAND}" -E remove "$<TARGET_FILE:${name_}>" 
            && "${CMAKE_COMMAND}" -E touch_nocreate .notexist
            COMMENT ${info_}
            VERBATIM
        )
      ELSE() # simply copy executable to output directory
        ADD_CUSTOM_COMMAND(
	    TARGET ${name_}
	    POST_BUILD
	    COMMAND "${CMAKE_COMMAND}" -E make_directory "${outputDir}"
	    COMMAND
	    "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:${name_}>" "${outputDir}"
	    || "${CMAKE_COMMAND}" -E remove "$<TARGET_FILE:${name_}>" 
	    && "${CMAKE_COMMAND}" -E touch_nocreate .notexist
	    COMMENT "Copy to destination"
        )
        FOREACH( companion_ ${bundle} )
          if( companion_ MATCHES ".*=.*" )
            string( REPLACE "=" ";" companion_ ${companion_} )
            list( GET companion_ 0 from_ )
            list( GET companion_ 1 to_ )
          else()
            set( from_ ${companion_} )
            set( to_ "" )
          endif()
          ADD_CUSTOM_COMMAND(
            TARGET ${name_}
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${outputDir}"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${from_}" "${outputDir}/${to_}"
          )
        ENDFOREACH()
      ENDIF()
    endif()
  ENDIF()
  
  SET( FAILED "${failed_}" PARENT_SCOPE )

ENDFUNCTION()

# Automatically define source groups for framework files
FUNCTION( BCI2000_AUTOSOURCEGROUPS ioList outCompanions )

  GET_FILENAME_COMPONENT( proj_dir_ "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE )
  file( TO_CMAKE_PATH "${proj_dir_}" proj_dir_ )
  GET_FILENAME_COMPONENT( frame_dir_ "${PROJECT_SRC_DIR}" ABSOLUTE )
  file( TO_CMAKE_PATH "${frame_dir_}" frame_dir_ )
  SET( list_ ${${ioList}} )
  SET( companions )

  UTILS_FIXUP_FILES( list_ )

  FOREACH( file_ ${list_} )
  
    GET_FILENAME_COMPONENT( absfile_ ${file_} ABSOLUTE )
    file( TO_CMAKE_PATH "${absfile_}" absfile_ )
    STRING( FIND "${absfile_}" "${proj_dir_}" projdirpos_ )
    SET( section_ "" )
    IF( RESOURCE_FILES )
      LIST( FIND RESOURCE_FILES "${absfile_}" res_ )
      IF( NOT res_ LESS 0 )
        SET( section_ Resources )
      ENDIF()
    ENDIF()

    IF( "${section_}" STREQUAL "" )
      SET( section_ "Source Files" )
      IF( file_ MATCHES ".*\\.(h|hpp)$" )
        SET( section_ "Header Files" )
      ELSEIF( file_ MATCHES ".*\\.(res|rc|rsrc|ui|qrc|def)$" )
        SET( section_ Resources )
      ELSEIF( file_ MATCHES ".*\\.(bat|dll|so|cmd|exe|txt|htm|html|rtf|pdf|tex|m)$" )
        LIST( APPEND companions ${file_} )
      ENDIF()
    ENDIF()

    SET( subsection "" )
    get_filename_component( abspath_ ${absfile_} PATH )
    utils_get_shortest_ext( ext_ ${file_} )
    IF( NOT projdirpos_ LESS 0 )
      # Inside current source directory
      FILE( RELATIVE_PATH rpath_ "${proj_dir_}" "${abspath_}" )
      FILE( TO_CMAKE_PATH "${rpath_}" rpath_ )
    ELSE()
      FILE( RELATIVE_PATH rpath_ "${frame_dir_}" "${abspath_}" )
      FILE( TO_CMAKE_PATH ${rpath_} rpath_ )
      IF( rpath_ MATCHES "^(shared|core)/" )
        SET( subsection "BCI2000 Framework" )
      ELSEIF( rpath_ MATCHES "^extlib/" )
        IF( rpath_ MATCHES "^extlib/(3DAPI|com|fftlib|math)(/|$)" )
          SET( subsection "BCI2000 Framework" )
          STRING( REPLACE "extlib/" "" rpath_ "${rpath_}" )
        ELSE()
          SET( subsection "External Libraries" )
          STRING( REPLACE "extlib/" "" rpath_ "${rpath_}" )
        ENDIF()
      ELSEIF( rpath_ MATCHES "^contrib/" )
        SET( subsection "BCI2000 Contrib" )
        FILE( RELATIVE_PATH rpath_ "${frame_dir_}/contrib" "${abspath_}" )
        FILE( TO_CMAKE_PATH "${rpath_}" rpath_ )
      ENDIF()
    ENDIF()
    IF( NOT "${rpath_}" STREQUAL "" )
      SET( rpath__ "/${rpath_}" )
      STRING( REPLACE "/.." "" rpath_ "${rpath__}" )
    ENDIF()
    SET( group_ "${section_}/${subsection}${rpath_}" )
    string( REPLACE "/" "\\" group_ "${group_}" )
    source_group( ${group_} FILES ${absfile_} )

  ENDFOREACH()

  foreach( companion_ ${companions} )
    list( REMOVE_ITEM list_ ${companion_} )
  endforeach()
  SET( ${ioList} ${list_} PARENT_SCOPE )
  SET( ${outCompanions} ${companions} PARENT_SCOPE )
  
ENDFUNCTION()

# Add dependencies of individual source files
FUNCTION( BCI2000_AUTODEPEND  listname_ libsname_ )

  SET( newlibs_ )
  SET( newfiles_ )

  FOREACH( file_ ${${listname_}} )
    IF( WIN32 AND file_ MATCHES "/sockstream\\.cpp$" )
      LIST( APPEND newlibs_ ws2_32 )
    ENDIF()
  ENDFOREACH()
  SET( ${listname_} ${${listname_}} ${newfiles_} PARENT_SCOPE )
  SET( ${libsname_} ${${libsname_}} ${newlibs_} PARENT_SCOPE )

ENDFUNCTION()

# Automatically add proper include directories (to work reliably, must operate on absolute paths)
FUNCTION( BCI2000_AUTOINCLUDE listname_ )

  SET( paths_
    ${PROJECT_SRC_DIR}/shared
    ${PROJECT_SRC_DIR}/shared/config
    ${PROJECT_SRC_DIR}/shared/bcistream
  )
  FOREACH( file_ ${${listname_}} )
    IF( file_ MATCHES ".*\\.\(h|c|cpp\)$" )
      GET_FILENAME_COMPONENT( path_ ${file_} PATH )
      SET( paths_ ${paths_} ${path_} )
    ENDIF()
  ENDFOREACH()
  IF( paths_ )
    LIST( REMOVE_DUPLICATES paths_ )
    INCLUDE_DIRECTORIES( ${paths_} )
  ENDIF()

ENDFUNCTION()
