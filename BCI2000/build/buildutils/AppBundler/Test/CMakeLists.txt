###########################################################################
## $Id$

set( DIR_NAME "Build/Tools/AppBundler" )

if( WIN32 )
  bci2000_add_target(
    CONSOLEAPP NOBUNDLE NOVERSION BundleTest
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    BundleTest.cpp
  )
  add_custom_command( TARGET BundleTest POST_BUILD
    COMMAND "$<TARGET_FILE:BundleTest>" --create 1048576 "${CMAKE_CURRENT_BINARY_DIR}/testdata.bin"
    COMMAND "$<TARGET_FILE:AppBundler>" "${BUILDUTILS_DIR}/BundleTest"
      "$<TARGET_FILE:BundleTest>" "${CMAKE_CURRENT_BINARY_DIR}/testdata.bin=test/test.bin"
    COMMAND "${BUILDUTILS_DIR}/BundleTest" --test 10000 - C:/current.app/test/test.bin
    DEPENDS AppBundler
  )
  add_dependencies( BundleTest AppBundler )
endif()
