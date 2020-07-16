###########################################################################
## $Id: FFT.cmake 5570 2016-10-12 19:27:33Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Sets up CMAKE variables for including the FFT lib
## SETS:
##       SRC_EXTLIB - Required source files for the fft library
##       HDR_EXTLIB - Required header files for the fft library
##       INC_EXTLIB - Include directory for the fft library

# Set success
SET( EXTLIB_OK TRUE )

# Define the source files
SET( SRC_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/fftlib/FFTLibWrap.cpp
  ${PROJECT_SRC_DIR}/extlib/fftlib/fftw3/include/fftw3.imports.cpp
)
# Define the headers
SET( HDR_EXTLIB
  ${PROJECT_SRC_DIR}/extlib/fftlib/FFTLibWrap.h
)

# Define the include directory
SET( INC_EXTLIB ${PROJECT_SRC_DIR}/extlib/fftlib )
if( WIN32 )
  utils_get_extlib( fftw_ fftw 3.3 )
  if( fftw_ )
    list( APPEND SRC_EXTLIB COMPANION ${fftw_}/libfftw3-3.dll=libfftw3-3_${UTILS_ARCH_BITS}.dll )
  else()
    set( EXTLIB_OK FALSE )
  endif()
endif()

