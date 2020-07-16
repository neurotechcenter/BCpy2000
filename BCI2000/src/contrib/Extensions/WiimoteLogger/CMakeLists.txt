###########################################################################
## $Id: CMakeLists.txt 5342 2016-04-29 14:43:51Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for the MSAutoConnectWiimote program

IF( ${BUILD_WIIMOTELOGGER} )

  # It is likely possible to compile this code on at least MinGW in addition
  # to MSVC, but doing so would be a bit more of a task than its worth.  It
  # comes down to a matter of symbol decoration in MS-compiled libraries.
  # If you're interested in building this with MinGW, see here:
  # http://www.daniweb.com/software-development/cpp/threads/365665
  IF( MSVC )

    # Set the executable name
    SET( NAME MSAutoConnectWiimote )

    # Set the project specific sources
    SET( SRC_PROJECT
      MSAutoConnectWiimote.cpp
    )
    SET( HDR_PROJECT
    )

    INCLUDE_DIRECTORIES(
    )
    
    ADD_DEFINITIONS( /wd4995 )

    SET_OUTPUT_DIRECTORY( ${PROJECT_BUILD_ROOT}/tools/ )
    BCI2000_ADD_TARGET( INFO "Wiimote Tool" EXECUTABLE ${NAME} ${SRC_PROJECT} ${HDR_PROJECT} )
    
    # Set the project build folder
    SET_PROPERTY( TARGET ${NAME} PROPERTY FOLDER "${DIR_NAME}" )

  ENDIF( MSVC )

ENDIF( ${BUILD_WIIMOTELOGGER} )
