###########################################################################
## $Id: VAMP.cmake 4447 2013-05-22 14:26:10Z mellinger $
## Author: juergen.mellinger@uni-tuebingen.de
## Description: Find and include Qt
##   No support for pre-compiled static Qt versions any more.
##   Set Qt5_DIR to the desired Qt installation's lib/cmake/Qt5 
##   subdirectory before configuring project, e.g. using cmake-gui.

SET( QT5_MODULES
  Gui
  Widgets
)

FUNCTION( GET_QT_VERSION outVer )

  SET( ${outVer} NOTFOUND PARENT_SCOPE )
  IF( EXISTS ${QT_QMAKE_EXECUTABLE} )
    EXECUTE_PROCESS(
      COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_VERSION
      OUTPUT_VARIABLE ver
      RESULT_VARIABLE result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    IF( result EQUAL 0 )
      SET( ${outVer} ${ver} PARENT_SCOPE )
    ENDIF()
  ENDIF()

ENDFUNCTION()

set(QT_AVAILABLE FALSE)
FIND_PACKAGE(Qt5 COMPONENTS ${QT5_MODULES})
if(Qt5_DIR) # defined by find_package on success
  set(QT_AVAILABLE TRUE)
  if(WIN32)
    find_program( QT_QMAKE_EXECUTABLE qmake NO_DEFAULT_PATH PATHS ${Qt5_DIR}/../../../bin )
  else()
    find_program( QT_QMAKE_EXECUTABLE qmake )
  endif()
  if( NOT QT_QMAKE_EXECUTABLE )
    set(QT_AVAILABLE FALSE)
    message(
    "NOTE: Qt5 could not be found, but may be available on your machine.\n"
    "Set Qt5_DIR to the desired Qt installation's lib/cmake/Qt5 subdirectory "
    "if you have Qt installed."
    )
  endif()
endif()

if(QT_AVAILABLE)
  # report version
  GET_QT_VERSION( qtver_ )
  GET_FILENAME_COMPONENT( qtdir_ ${QT_QMAKE_EXECUTABLE} PATH )
  SET( QT_VERSION ${qtver_} )
  UTILS_CONFIG_STATUS( "Using Qt ${qtver_} (${qtdir_})" )

  set( USE_DYNAMIC_CRT TRUE CACHE BOOL "Forced by USE_EXTERNAL_QT" FORCE )

  if(QT_QMAKE_EXECUTABLE)
    foreach( var_ IN ITEMS
        QT_INSTALL_LIBS
        QT_INSTALL_LIBEXECS
        QT_INSTALL_PLUGINS
        QT_INSTALL_HEADERS )
      execute_process(
          COMMAND ${QT_QMAKE_EXECUTABLE} -query ${var_}
          OUTPUT_VARIABLE ${var_}
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    endforeach()
    set(QT_INCLUDES ${QT_INSTALL_HEADERS} ${QT_INSTALL_HEADERS}/QtCore)
    foreach(mod_ ${QT5_MODULES})
        list(APPEND QT_INCLUDES ${QT_INSTALL_HEADERS}/Qt${mod_})
    endforeach()
  endif()


  SET( QT_LIBRARIES "" )
  SET( QT5_PLUGINS "" )
  FOREACH( module_ ${QT5_MODULES} )
	SET( QT_LIBRARIES ${QT_LIBRARIES} "Qt5::${module_}" )
  ENDFOREACH()
  list( APPEND QT_SOURCES ${PROJECT_SRC_DIR}/shared/utils/QtMessageHandler.cpp )
  IF( WIN32 )
    SET( QT_LIBRARIES
      ${QT_LIBRARIES}
        imm32
        opengl32
        glu32
        ws2_32
        winmm
      )
  ENDIF()
  set( EXTLIB_OK TRUE )
  add_definitions(-DUSE_QT)
  set( INC_EXTLIB ${QT_INCLUDES} )
  set( SRC_EXTLIB ${QT_SOURCES} )
  if(WIN32)
    set( dll "platforms/qwindows$<$<CONFIG:Debug>:d>.dll" )
    list( APPEND SRC_EXTLIB COMPANION "${QT_INSTALL_PLUGINS}/${dll}=qtplugins/${dll}" )
    foreach( module IN ITEMS ${QT5_MODULES} Core )
      set( dll "Qt5${module}$<$<CONFIG:Debug>:d>.dll" )
      list( APPEND SRC_EXTLIB COMPANION "${QT_INSTALL_LIBEXECS}/${dll}" )
    endforeach()
if(0)
    # must explicitly list dependencies of qwindows.dll
    foreach( depdll IN ITEMS libEGL libGLESv2 )
      set( dll "${depdll}$<$<CONFIG:Debug>:d>.dll" )
      list( APPEND SRC_EXTLIB COMPANION "${QT_INSTALL_LIBEXECS}/${dll}" )
    endforeach()
endif()
    # older versions of Qt depend on libicu dlls
    foreach( depdll IN ITEMS icuin51 icuuc51 icudt51 )
      set( dll "${depdll}.dll" )
      if(EXISTS "${QT_INSTALL_LIBEXECS}/${dll}")
        list( APPEND SRC_EXTLIB COMPANION "${QT_INSTALL_LIBEXECS}/${dll}" )
      endif()
    endforeach()
  endif()

  foreach(item Core ${QT5_MODULES})
    list(APPEND LIBS_EXTLIB Qt5${item})
  endforeach()
  set( LIBS_EXTLIB ${QT_LIBRARIES} )
  set( LIBDIR_EXTLIB ${QT_INSTALL_LIBEXECS} )
else()
  set( EXTLIB_OK FALSE )
endif()







