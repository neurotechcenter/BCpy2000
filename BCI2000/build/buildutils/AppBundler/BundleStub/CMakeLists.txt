###########################################################################
## $Id$

if( WIN32 )

  option( BUNDLE_STUB_DEBUG "Build bundle stub in debug configuration" OFF )
  mark_as_advanced( BUNDLE_STUB_DEBUG )
  set( config_types_ )
  foreach( config_type_ ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER "${config_type_}" config_type_ )
    list( APPEND config_types_ ${config_type_} )
  endforeach()
  foreach( config_type_ ${config_types_} )
    set( CMAKE_C_FLAGS_${config_type_} "" )
    set( CMAKE_CXX_FLAGS_${config_type_} "" )
    set( CMAKE_EXE_LINKER_FLAGS_${config_type_} "" )
  endforeach()

  bci2000_include( "udis86_nosyn" )
  bci2000_include( "miniz" )
  add_definitions( -DMINIZ_NO_TIME -DMINIZ_NO_STDIO )
  if( MSVC )
    set( CMAKE_EXE_LINKER_FLAGS "/ENTRY:InitialEntryPoint /FIXED:NO /INCREMENTAL:NO /MANIFEST:NO" )
    if( BUNDLE_STUB_DEBUG )
      set( CMAKE_CXX_FLAGS "/GS- /Zi /MTd /Od /Oy- /D_DEBUG /D_NO_CRT_STDIO_INLINE /Zc:threadSafeInit-,sizedDealloc-,implicitNoexcept-" )
      set( CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL:NO" )
      set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /debug /NODEFAULTLIB ntdll.lib" )
    else()
      set( CMAKE_CXX_FLAGS "/GS- /MT /DNDEBUG /D_NO_CRT_STDIO_INLINE /Zc:threadSafeInit-,sizedDealloc-,implicitNoexcept-" )
      set( CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2" )
      set( CMAKE_CXX_FLAGS_MINSIZEREL "/O1" )
      set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Zi /O2 /Ob1" )
      set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" )
      set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB ntdll.lib /OPT:REF" )
    endif()
    set( CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS} )
    foreach( config_type_ ${config_types_} )
      set( CMAKE_C_FLAGS_${config_type_} "${CMAKE_CXX_FLAGS_${config_type_}}" )
    endforeach()
  endif()

  bci2000_add_target(
    WINAPP NOBUNDLE NOVERSION BundleStub
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    Main.cpp
    Stub.cpp
    ../Bundle.h
    ../Utils.cpp
    ../MinizArchive.cpp
    ${PROJECT_SRC_DIR}/shared/utils/Lib/PEHeader.cpp
    MiniCRT.cpp
    DllImport.cpp
    ZipFileSystem.cpp
    NtHookFs.cpp
    PELoader.cpp
    FunctionHook.cpp
#    ExeDetach.cpp
    NtDllImports.cpp
  )
endif()
