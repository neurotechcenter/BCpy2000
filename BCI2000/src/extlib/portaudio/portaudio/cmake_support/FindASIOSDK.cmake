# $Id: $
#
# - Try to find the ASIO SDK
# Once done this will define
#
#  ASIOSDK_FOUND - system has ASIO SDK
#  ASIOSDK_ROOT_DIR - path to the ASIO SDK base directory
#  ASIOSDK_INCLUDE_DIR - the ASIO SDK include directory

IF(WIN32)
ELSE(WIN32)
  MESSAGE(FATAL_ERROR "FindASIOSDK.cmake: Unsupported platform ${CMAKE_SYSTEM_NAME}" )
ENDIF(WIN32)

FILE(GLOB results "${PA_DIR}/../asi*")
FOREACH(f ${results})
  IF(IS_DIRECTORY ${f})
    SET(ASIOSDK_PATH_HINT ${ASIOSDK_PATH_HINT} ${f})
  ENDIF()
ENDFOREACH()

FIND_PATH(ASIOSDK_ROOT_DIR
  common/asio.h
  HINTS
    ${ASIOSDK_PATH_HINT}
)

FIND_PATH(ASIOSDK_INCLUDE_DIR
  asio.h
  PATHS
    ${ASIOSDK_ROOT_DIR}/common 
)  

# handle the QUIETLY and REQUIRED arguments and set ASIOSDK_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASIOSDK DEFAULT_MSG ASIOSDK_ROOT_DIR ASIOSDK_INCLUDE_DIR)

MARK_AS_ADVANCED(
    ASIOSDK_ROOT_DIR ASIOSDK_INCLUDE_DIR
)
