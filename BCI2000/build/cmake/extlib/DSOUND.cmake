###########################################################################
## $Id: DSOUND.cmake 5371 2016-05-20 16:07:58Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Sets up CMAKE variables for including DirectSound in a project
## SETS:
##       SRC_EXTLIB - Required source files for DirectSound
##       HDR_EXTLIB - Required header files for DirectSound
##       INC_EXTLIB - Include directory for DirectSound
##       LIBDIR_EXTLIB - Directory where the DSound library resides
##       LIBS_EXTLIB - Required libraries for DirectSound

IF( WIN32 )
IF( NOT BORLAND )

# Define the source files
SET( SRC_EXTLIB )

# Define the headers
SET( HDR_EXTLIB ${PROJECT_SRC_DIR}/extlib/dxsdk/include/dsound.h )

# Define the include directory
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/dxsdk/include )

# Set where the libraries can be found
SET( LIBDIR_EXTLIB ${PROJECT_SRC_DIR}/extlib/dxsdk/lib )

# Set the name of the library to link against within LIBDIR
SET( LIBS_EXTLIB dsound.lib )

ENDIF( NOT BORLAND )

# Set success
SET( EXTLIB_OK TRUE )

ELSE( WIN32 )

utils_warn(
  "DirectSound required (currently) for speech synthesis.\n"
  "Cannot build audio support for this application."
)

# Set success
SET( EXTLIB_OK FALSE )

ENDIF( WIN32 )
