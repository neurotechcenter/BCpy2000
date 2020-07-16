###########################################################################
## $Id$

add_subdirectory( Test )

set( DIR_NAME "Build/Tools/AppBundler" )
add_subdirectory( BundleStub )
set_output_directory( ${BUILDUTILS_DIR} )
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/BundleStub.bin
  COMMAND "$<TARGET_FILE:BundleStub>" "./BundleStub.bin"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS BundleStub
)

utils_add_resource( BundleStub ${CMAKE_CURRENT_BINARY_DIR}/BundleStub.bin )
bci2000_include( "MINIZ" )
utils_include( frameworks/Core )
if( WIN32 )
  add_definitions( -DMINIZ_NO_STDIO )
  bci2000_add_target(
    CONSOLEAPP NOBUNDLE NOVERSION AppBundler
    AppBundler.cpp
    Bundle.h
    MinizArchive.cpp
    Utils.cpp
  )
endif()
