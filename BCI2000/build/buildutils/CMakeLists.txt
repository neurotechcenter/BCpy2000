###########################################################################
## $Id: CMakeLists.txt 5836 2018-12-04 15:15:31Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for testing and build utilities

CONFIGURE_FILE( buildenv.h.in ${BUILDUTILS_DIR}/buildenv.h )

SET( DIR_NAME "${DIR_NAME}Build/BuildUtils/" )
SET( USE_PRECOMPILED_HEADERS OFF )

ADD_SUBDIRECTORY( BootstrapCustomProjects )
ADD_SUBDIRECTORY( DylibTool )
ADD_SUBDIRECTORY( SchedulingTool )
SET_OUTPUT_DIRECTORY( ${BUILDUTILS_DIR} )
if( USE_APPLICATION_BUNDLES )
  ADD_SUBDIRECTORY( AppBundler )
else()
  add_subdirectory( AppCopier )
endif()
configure_file( deploy_app.bat "${BUILDUTILS_DIR}/deploy_app.bat" @ONLY )

SET_OUTPUT_DIRECTORY( ${BUILDUTILS_DIR}/tests )
set_crt_link_type( STATIC )
BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE NOVERSION bci_datadiff bci_datadiff.cpp )

SET_OUTPUT_DIRECTORY( ${BUILDUTILS_DIR} )
# extract_registry
INCLUDE_DIRECTORIES(
  ${PROJECT_SRC_DIR}/shared/config
)
BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE NOVERSION extract_registry extract_registry.cpp )

# create_resource
UTILS_INCLUDE( frameworks/LibTiny )
BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE NOVERSION create_resource
  create_resource.cpp
  ${PROJECT_SRC_DIR}/shared/types/Blob.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Resource.h
)

# list_dependencies
BCI2000_ADD_TARGET( CONSOLEAPP NOBUNDLE NOVERSION list_dependencies
  list_dependencies.cpp
  ${BUILDUTILS_DIR}/buildenv.h
)

if( WIN32 )
  bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION make_win32ico
    make_win32ico.cpp
    ${PROJECT_SRC_DIR}/shared/utils/BinaryDataIO/Win32BmpFormat.cpp
  )
  utils_include( frameworks/LibTiny )
  bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION png2bmp
    png2bmp.cpp
    ${PROJECT_SRC_DIR}/shared/utils/BinaryDataIO/Win32BmpFormat.cpp
    ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.cpp
  )
endif()
bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION combine_png
  combine_png.cpp
  ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.cpp
)
utils_include( frameworks/Core )
bci2000_add_target( CONSOLEAPP NOBUNDLE NOVERSION transform_png
  transform_png.cpp
  ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream_tool.cpp
  ${PROJECT_SRC_DIR}/extlib/lodepng/lodepng.cpp
)

