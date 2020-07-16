###########################################################################
## $Id: QTOPENGL.cmake 4763 2014-11-25 17:36:47Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Sets up CMAKE variables for including Qt OpenGL in a project
## SETS:
##       LIBS_EXTLIB - Required libraries for OpenGL

# Let CMake know we plan to link against the Qt OPENGL libraries
IF( QT_IS4 )
  SET( QT_USE_QTOPENGL TRUE )
  INCLUDE(${QT_USE_FILE})
  SET( LIBS_EXTLIB
    ${QT_LIBRARIES}
  )

  FIND_PACKAGE( OpenGL REQUIRED )
  IF( OPENGL_FOUND )
    SET( LIBS_EXTLIB
      ${LIBS_EXTLIB}
      ${OPENGL_LIBRARIES}
    )
  ELSE( OPENGL_FOUND )
    MESSAGE( FATAL_ERROR "OpenGL package not found." )
  ENDIF( OPENGL_FOUND )

  # Set success
  SET( EXTLIB_OK TRUE )

ENDIF()
