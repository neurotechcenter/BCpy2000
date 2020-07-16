# $Id$
# BCI2000-specific static build of the portaudio library.
# Adapted from the CMakeLists.txt in the portaudio subdirectory.
# The portaudio subdirectory is supposed to contain a copy of
# the latest stable portaudio source code.

SET( PA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/portaudio )

IF(WIN32)
	 SET(DXSDK_FOUND TRUE)
  SET(DXSDK_INCLUDE_DIR ${PROJECT_SRC_DIR}/extlib/dxsdk/include)
  SET(DXSDK_DSOUND_LIBRARY ${PROJECT_SRC_DIR}/extlib/dxsdk/lib/dsound.lib)
  SET(PORTAUDIO_ENABLE_DSOUND ON)
  SET(PORTAUDIO_UNICODE_BUILD OFF) # unicode is broken with PA DSound

  SET(CMAKE_MODULE_PATH ${PA_DIR}/cmake_support)
  # Try to find ASIO SDK (assumes that portaudio and asiosdk folders are side-by-side, see
  # http://portaudio.com/docs/v19-doxydocs/compile_windows.html)
  # To compile BCI2000 with ASIO support, put the ASIO SDK into src/extlib/portaudio/asiosdk.
  FIND_PACKAGE(ASIOSDK QUIET)

  IF(ASIOSDK_FOUND)
    OPTION(PORTAUDIO_ENABLE_ASIO "Enable support for ASIO" ON)
  ENDIF(ASIOSDK_FOUND)
  OPTION(PORTAUDIO_ENABLE_WMME "Enable support for MME" OFF)
  MARK_AS_ADVANCED(PORTAUDIO_ENABLE_WMME)
  OPTION(PORTAUDIO_ENABLE_WASAPI "Enable support for WASAPI" OFF)
  MARK_AS_ADVANCED(PORTAUDIO_ENABLE_WASAPI)
  OPTION(PORTAUDIO_ENABLE_WDMKS "Enable support for WDMKS" OFF)
  MARK_AS_ADVANCED(PORTAUDIO_ENABLE_WDMKS)
  OPTION(PORTAUDIO_USE_WDMKS_DEVICE_INFO "Use WDM/KS API for device info" OFF)
  MARK_AS_ADVANCED(PORTAUDIO_USE_WDMKS_DEVICE_INFO)
  IF(PORTAUDIO_ENABLE_DSOUND)
    OPTION(PORTAUDIO_USE_DIRECTSOUNDFULLDUPLEXCREATE "Use DirectSound full duplex create" ON)
    MARK_AS_ADVANCED(PORTAUDIO_USE_DIRECTSOUNDFULLDUPLEXCREATE)
  ENDIF(PORTAUDIO_ENABLE_DSOUND)
  IF(PORTAUDIO_ENABLE_WDMKS)
    ADD_DEFINITIONS(-DPAWIN_WDMKS_NO_KSGUID_LIB)
  ENDIF()
ENDIF(WIN32)

MACRO(SET_HEADER_OPTION OPTION_NAME OPTION_VALUE)
  IF(${OPTION_NAME})
    SET(${OPTION_VALUE} "1")
  ELSE(${OPTION_NAME})
    SET(${OPTION_VALUE} "0")
  ENDIF(${OPTION_NAME})
ENDMACRO(SET_HEADER_OPTION)

SET_HEADER_OPTION(PORTAUDIO_ENABLE_ASIO   PA_USE_ASIO)
SET_HEADER_OPTION(PORTAUDIO_ENABLE_DSOUND PA_USE_DS)
SET_HEADER_OPTION(PORTAUDIO_ENABLE_WMME   PA_USE_WMME)
SET_HEADER_OPTION(PORTAUDIO_ENABLE_WASAPI PA_USE_WASAPI)
SET_HEADER_OPTION(PORTAUDIO_ENABLE_WDMKS  PA_USE_WDMKS)

# Set variables for DEF file expansion
IF(NOT PORTAUDIO_ENABLE_ASIO)
  SET(DEF_EXCLUDE_ASIO_SYMBOLS ";")
ENDIF(NOT PORTAUDIO_ENABLE_ASIO)

IF(NOT PORTAUDIO_ENABLE_WASAPI)
  SET(DEF_EXCLUDE_WASAPI_SYMBOLS ";")
ENDIF(NOT PORTAUDIO_ENABLE_WASAPI)

IF(PORTAUDIO_USE_WDMKS_DEVICE_INFO)
  ADD_DEFINITIONS(-DPAWIN_USE_WDMKS_DEVICE_INFO)
ENDIF(PORTAUDIO_USE_WDMKS_DEVICE_INFO)

IF(PORTAUDIO_USE_DIRECTSOUNDFULLDUPLEXCREATE)
  ADD_DEFINITIONS(-DPAWIN_USE_DIRECTSOUNDFULLDUPLEXCREATE)
ENDIF(PORTAUDIO_USE_DIRECTSOUNDFULLDUPLEXCREATE)

#######################################
IF(WIN32)
  INCLUDE_DIRECTORIES(${PA_DIR}/src/os/win)
ELSE()
  IF(APPLE)
    INCLUDE_DIRECTORIES(${PA_DIR}/src/os/mac_os)
  ENDIF()
  INCLUDE_DIRECTORIES(${PA_DIR}/src/os/unix)
ENDIF()

IF(PORTAUDIO_ENABLE_ASIO)
  INCLUDE_DIRECTORIES(${ASIOSDK_ROOT_DIR}/common)
  INCLUDE_DIRECTORIES(${ASIOSDK_ROOT_DIR}/host)
  INCLUDE_DIRECTORIES(${ASIOSDK_ROOT_DIR}/host/pc)

  SET(PA_ASIO_SOURCES 
    ${PA_DIR}/src/hostapi/asio/pa_asio.cpp
  )  

  SET(PA_ASIOSDK_SOURCES
    ${ASIOSDK_ROOT_DIR}/common/asio.cpp
    ${ASIOSDK_ROOT_DIR}/host/pc/asiolist.cpp
    ${ASIOSDK_ROOT_DIR}/host/asiodrivers.cpp
  )

  SOURCE_GROUP("hostapi\\ASIO" FILES
    ${PA_ASIO_SOURCES}
  )

  SOURCE_GROUP("hostapi\\ASIO\\ASIOSDK" FILES
    ${PA_ASIOSDK_SOURCES}
  )
ENDIF(PORTAUDIO_ENABLE_ASIO)

IF(PORTAUDIO_ENABLE_DSOUND)
  INCLUDE_DIRECTORIES(${DXSDK_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${PA_DIR}/src/os/win)

  SET(PA_DS_INCLUDES
    ${PA_DIR}/src/hostapi/dsound/pa_win_ds_dynlink.h
  )

  SET(PA_DS_SOURCES
    ${PA_DIR}/src/hostapi/dsound/pa_win_ds.c
    ${PA_DIR}/src/hostapi/dsound/pa_win_ds_dynlink.c
  )

  SOURCE_GROUP("hostapi\\dsound" FILES
    ${PA_DS_INCLUDES}
    ${PA_DS_SOURCES}
  )
ENDIF(PORTAUDIO_ENABLE_DSOUND)

IF(PORTAUDIO_ENABLE_WMME)
  SET(PA_WMME_SOURCES
    ${PA_DIR}/src/hostapi/wmme/pa_win_wmme.c
  )

  SOURCE_GROUP("hostapi\\wmme" FILES
    ${PA_WMME_SOURCES}
  )
ENDIF(PORTAUDIO_ENABLE_WMME)

IF(PORTAUDIO_ENABLE_WASAPI)
  SET(PA_WASAPI_SOURCES
    ${PA_DIR}/src/hostapi/wasapi/pa_win_wasapi.c
  )

  SOURCE_GROUP("hostapi\\wasapi" FILES
    ${PA_WASAPI_SOURCES}
  )
  ENDIF(PORTAUDIO_ENABLE_WASAPI)

  IF(PORTAUDIO_ENABLE_WDMKS)
  SET(PA_WDMKS_SOURCES
    ${PA_DIR}/src/hostapi/wdmks/pa_win_wdmks.c
  )

  SOURCE_GROUP("hostapi\\wdmks" FILES
    ${PA_WDMKS_SOURCES}
  )
ENDIF(PORTAUDIO_ENABLE_WDMKS)

#######################################
SET(PA_INCLUDES 
  ${PA_DIR}/include/portaudio.h
)
IF(WIN32)
  SET(PA_INCLUDES
    ${PA_INCLUDES}
    ${PA_DIR}/include/pa_asio.h
    ${PA_DIR}/include/pa_win_ds.h
    ${PA_DIR}/include/pa_win_wasapi.h
    ${PA_DIR}/include/pa_win_wmme.h
    ${PA_DIR}/include/pa_win_wdmks.h
  )
ELSEIF(APPLE)
  SET(PA_INCLUDES
    ${PA_INCLUDES}
    ${PA_DIR}/include/pa_mac_core.h
  )
ELSE()
  SET(PA_INCLUDES
    ${PA_INCLUDES}
    ${PA_DIR}/include/pa_linux_alsa.h
  )
ENDIF()

SOURCE_GROUP("include" FILES
  ${PA_INCLUDES}
)

SET(PA_COMMON_INCLUDES
  ${PA_DIR}/src/common/pa_allocation.h
  ${PA_DIR}/src/common/pa_converters.h
  ${PA_DIR}/src/common/pa_cpuload.h
  ${PA_DIR}/src/common/pa_debugprint.h
  ${PA_DIR}/src/common/pa_dither.h
  ${PA_DIR}/src/common/pa_endianness.h
  ${PA_DIR}/src/common/pa_hostapi.h
  ${PA_DIR}/src/common/pa_memorybarrier.h
  ${PA_DIR}/src/common/pa_process.h
  ${PA_DIR}/src/common/pa_ringbuffer.h
  ${PA_DIR}/src/common/pa_stream.h
  ${PA_DIR}/src/common/pa_trace.h
  ${PA_DIR}/src/common/pa_types.h
  ${PA_DIR}/src/common/pa_util.h
)

SET(PA_COMMON_SOURCES
  ${PA_DIR}/src/common/pa_allocation.c
  ${PA_DIR}/src/common/pa_converters.c
  ${PA_DIR}/src/common/pa_cpuload.c
  ${PA_DIR}/src/common/pa_debugprint.c
  ${PA_DIR}/src/common/pa_dither.c
  ${PA_DIR}/src/common/pa_front.c
  ${PA_DIR}/src/common/pa_process.c
  ${PA_DIR}/src/common/pa_ringbuffer.c
  ${PA_DIR}/src/common/pa_stream.c
  ${PA_DIR}/src/common/pa_trace.c
)

SOURCE_GROUP("common" FILES
  ${PA_COMMON_INCLUDES}
  ${PA_COMMON_SOURCES}
)

ADD_DEFINITIONS(-DUSE_C99_LRINTF)

SOURCE_GROUP("cmake_generated" FILES
  ${CMAKE_CURRENT_BINARY_DIR}/portaudio_cmake.def
  ${CMAKE_CURRENT_BINARY_DIR}/options_cmake.h
)

IF(WIN32)
  SET(PA_PLATFORM_SOURCES 
    ${PA_DIR}/src/os/win/pa_win_hostapis.c
    ${PA_DIR}/src/os/win/pa_win_util.c
    ${PA_DIR}/src/os/win/pa_win_waveformat.c
    ${PA_DIR}/src/os/win/pa_win_wdmks_utils.c
    ${PA_DIR}/src/os/win/pa_win_coinitialize.c
  )

  SOURCE_GROUP("os\\win" FILES
    ${PA_PLATFORM_SOURCES}
  )
ELSE()
  SET(PA_PLATFORM_SOURCES 
    ${PA_DIR}/src/os/unix/pa_unix_hostapis.c
    ${PA_DIR}/src/os/unix/pa_unix_util.c
  )
  SOURCE_GROUP("os\\unix" FILES
    ${PA_PLATFORM_SOURCES}
  )
ENDIF(WIN32)

INCLUDE_DIRECTORIES( ${PA_DIR}/include )
INCLUDE_DIRECTORIES( ${PA_DIR}/src/common )

IF(MINGW)
  IF(GCC_VERSION VERSION_LESS 4.7)
    INCLUDE_DIRECTORIES( ${PA_DIR}/src/hostapi/wasapi/mingw-include )
    ADD_DEFINITIONS( -D_WAVEFORMATEXTENSIBLE_ )
  ELSE()
    INCLUDE_DIRECTORIES( ${PA_DIR}/../mingw-include )
  ENDIF()
ENDIF()

IF(MSVC)
  ADD_DEFINITIONS(
    /D_CRT_SECURE_NO_WARNINGS
    /W0
  )
ELSE()
  ADD_DEFINITIONS(
    -w
  )
ENDIF()

ADD_DEFINITIONS(-DPORTAUDIO_CMAKE_GENERATED)
INCLUDE_DIRECTORIES( ${PA_DIR} )

SET(SOURCES_LESS_ASIO_SDK
  ${PA_COMMON_SOURCES}
  ${PA_ASIO_SOURCES}
  ${PA_DS_SOURCES}
  ${PA_WMME_SOURCES}
  ${PA_WASAPI_SOURCES}
  ${PA_WDMKS_SOURCES}
  ${PA_SKELETON_SOURCES}
  ${PA_PLATFORM_SOURCES}
)

IF(PORTAUDIO_UNICODE_BUILD)
  SET_SOURCE_FILES_PROPERTIES(
    ${SOURCES_LESS_ASIO_SDK}
    PROPERTIES
    COMPILE_DEFINITIONS "UNICODE;_UNICODE"
  )
ENDIF(PORTAUDIO_UNICODE_BUILD)

ADD_LIBRARY(PortAudio STATIC
  ${PA_INCLUDES}
  ${PA_COMMON_INCLUDES}
  ${SOURCES_LESS_ASIO_SDK}
  ${PA_ASIOSDK_SOURCES}
  ${CMAKE_CURRENT_BINARY_DIR}/options_cmake.h
)
SET_PROPERTY( TARGET PortAudio PROPERTY FOLDER "${DIR_NAME}" )

# Configure the exports file according to settings
SET(GENERATED_MESSAGE "CMake generated file, do NOT edit! Use CMake-GUI to change configuration instead.")
# Configure header for options (PA_USE_xxx)
CONFIGURE_FILE( ${PA_DIR}/cmake_support/options_cmake.h.in ${CMAKE_CURRENT_BINARY_DIR}/options_cmake.h @ONLY )

IF(WIN32)
  TARGET_LINK_LIBRARIES(PortAudio winmm)

  # If we use DirectSound, we need this for the library to be found (if not in VS project settings)
  IF(PORTAUDIO_ENABLE_DSOUND AND DXSDK_FOUND)
    TARGET_LINK_LIBRARIES(PortAudio ${DXSDK_DSOUND_LIBRARY})
  ENDIF(PORTAUDIO_ENABLE_DSOUND AND DXSDK_FOUND)

  # If we use WDM/KS we need setupapi.lib
  IF(PORTAUDIO_ENABLE_WDMKS)
    TARGET_LINK_LIBRARIES(PortAudio setupapi)
  ENDIF(PORTAUDIO_ENABLE_WDMKS)
ENDIF(WIN32)

IF( PORTAUDIO_BUILD_TESTS )
  # Tests from portaudio/tests
  SET( tests_
    patest1
    patest_buffer
    patest_callbackstop
    patest_clip
    patest_converters
    patest_dither
    patest_dsound_surround.0
    patest_hang
    patest_in_overflow
    patest_jack_wasapi
    patest_latency.0
    patest_leftright
    patest_longsine.0
    patest_many
    patest_maxsines
    patest_mono
    patest_multi_sine
    patest_out_underflow
    patest_prime
    patest_read_record
    patest_ringmix
    patest_sine8
    patest_sine_channelmaps
    patest_sine_formats
    patest_sine_srate
    patest_sine_time
    patest_start_stop
    patest_stop
    patest_stop_playout
    patest_suggested_vs_streaminfo_latency.0
    patest_sync
    patest_timing
    patest_toomanysines
    patest_two_rates
    patest_underflow
    patest_wire
    patest_wmme_find_best_latency_params.0
    patest_wmme_low_level_latency_params
    patest_write_stop
    pa_minlat
  )
  FOREACH( test_ ${tests_} )
    STRING( FIND ${test_} ".0" noAutorun_ )
    STRING( REPLACE ".0" "" test_ ${test_} )
    ADD_EXECUTABLE( ${test_} ${PA_DIR}/test/${test_}.c )
    TARGET_LINK_LIBRARIES( ${test_} PortAudio )
    SET_PROPERTY( TARGET ${test_} PROPERTY FOLDER Tests/PortAudio )
    IF( NOT noAutorun_ )
      ADD_TEST( ${test_} ${test_} )
      SET_PROPERTY( TEST ${test_} PROPERTY TIMEOUT 120 )
    ENDIF()
  ENDFOREACH() 
ENDIF()
