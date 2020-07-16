###########################################################################
## $Id: IncludeExtension.cmake 5828 2018-11-28 16:45:06Z abelsten $
## Authors: jezhill@gmail.com

if( MSVC )

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set( OPENCV_FFMPEG opencv_ffmpeg400_64 )
  set( OPENCV_ARCH x64 )
else()
  set( OPENCV_FFMPEG opencv_ffmpeg400 )
  set( OPENCV_ARCH x32 )
endif()

set( OPENCV_LIBDIR 
  ${PROJECT_SRC_DIR}/extlib/opencv/lib/msvc/${OPENCV_ARCH}/$<CONFIG>
)

list( APPEND BCI2000_SIGSRC_FILES
   ${PROJECT_SRC_DIR}/extlib/opencv/include
   ${BCI2000_EXTENSION_DIR}/WebcamLogger.cpp
   ${BCI2000_EXTENSION_DIR}/WebcamThread.cpp
)

list( APPEND BCI2000_SIGSRC_LIBS 
  ${OPENCV_LIBDIR}/opencv_core400$<$<CONFIG:Debug>:d>.lib
  ${OPENCV_LIBDIR}/opencv_highgui400$<$<CONFIG:Debug>:d>.lib
  ${OPENCV_LIBDIR}/opencv_imgcodecs400$<$<CONFIG:Debug>:d>.lib
  ${OPENCV_LIBDIR}/opencv_imgproc400$<$<CONFIG:Debug>:d>.lib
  ${OPENCV_LIBDIR}/opencv_video400$<$<CONFIG:Debug>:d>.lib
  ${OPENCV_LIBDIR}/opencv_videoio400$<$<CONFIG:Debug>:d>.lib
)

list( APPEND BCI2000_SIGSRC_FILES 
  ${OPENCV_LIBDIR}/${OPENCV_FFMPEG}.dll
  ${OPENCV_LIBDIR}/opencv_core400$<$<CONFIG:Debug>:d>.dll
  ${OPENCV_LIBDIR}/opencv_highgui400$<$<CONFIG:Debug>:d>.dll
  ${OPENCV_LIBDIR}/opencv_imgcodecs400$<$<CONFIG:Debug>:d>.dll
  ${OPENCV_LIBDIR}/opencv_imgproc400$<$<CONFIG:Debug>:d>.dll
  ${OPENCV_LIBDIR}/opencv_video400$<$<CONFIG:Debug>:d>.dll
  ${OPENCV_LIBDIR}/opencv_videoio400$<$<CONFIG:Debug>:d>.dll
)

else( MSVC )

  utils_warn( "WebcamLogger: OpenCV libraries are only present for MSVC on windows." )

endif( MSVC )



