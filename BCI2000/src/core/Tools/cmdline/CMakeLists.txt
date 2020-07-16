###########################################################################
## $Id: CMakeLists.txt 5778 2018-06-11 14:02:47Z mellinger $
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de
## Description: Build information for the command line tools in cmake

SET( DIR_NAME "${DIR_NAME}cmdline/" )

# Format conversion tools from the cmdline directory
set_crt_link_type( STATIC )

BCI2000_ADD_CMDLINE_CONVERTER( bci_dat2stream )
BCI2000_ADD_CMDLINE_CONVERTER( bci_stream2asc )
BCI2000_ADD_CMDLINE_CONVERTER( bci_decimate   )
BCI2000_ADD_CMDLINE_CONVERTER( bci_prm2stream )
BCI2000_ADD_CMDLINE_CONVERTER( bci_stream2mat )
BCI2000_ADD_CMDLINE_CONVERTER( bci_stream2prm )
BCI2000_ADD_CMDLINE_CONVERTER( bci_stream2table )


# Filters residing in other directories.

SET( SIGPROC_DIR ${PROJECT_SOURCE_ROOT}/src/shared/modules/signalprocessing )

BCI2000_ADD_CMDLINE_FILTER( ARFilter              FROM ${SIGPROC_DIR} INCLUDING "MATH"
                            EXTRA_SOURCES ${SIGPROC_DIR}/ARSpectrum.cpp
                                          ${SIGPROC_DIR}/WindowingFilter.cpp
                                          ${SIGPROC_DIR}/Spectrum.cpp )
BCI2000_ADD_CMDLINE_FILTER( SpectralEstimator     FROM ${SIGPROC_DIR} INCLUDING "MATH" "FFT"
                            EXTRA_SOURCES ${SIGPROC_DIR}/ARSpectrum.cpp
                                          ${SIGPROC_DIR}/FFTSpectrum.cpp
                                          ${SIGPROC_DIR}/WindowingFilter.cpp
                                          ${SIGPROC_DIR}/Spectrum.cpp )
BCI2000_ADD_CMDLINE_FILTER( ComplexDemodulator    FROM ${SIGPROC_DIR} )
BCI2000_ADD_CMDLINE_FILTER( ConditionalIntegrator FROM ${SIGPROC_DIR} )
BCI2000_ADD_CMDLINE_FILTER( ExpressionFilter      FROM ${SIGPROC_DIR} )
BCI2000_ADD_CMDLINE_FILTER( LinearClassifier      FROM ${SIGPROC_DIR} )
BCI2000_ADD_CMDLINE_FILTER( LPFilter              FROM ${SIGPROC_DIR} )
BCI2000_ADD_CMDLINE_FILTER( Normalizer            FROM ${SIGPROC_DIR} )
BCI2000_ADD_CMDLINE_FILTER( P3TemporalFilter      FROM ${SIGPROC_DIR} )
BCI2000_ADD_CMDLINE_FILTER( SpatialFilter         FROM ${SIGPROC_DIR} )
BCI2000_ADD_CMDLINE_FILTER( StateTransform        FROM ${SIGPROC_DIR} )
BCI2000_ADD_CMDLINE_FILTER( TransmissionFilter    FROM ${PROJECT_SOURCE_ROOT}/src/shared/modules/signalsource )
BCI2000_ADD_CMDLINE_FILTER( FFTFilter             FROM ${SIGPROC_DIR} INCLUDING "FFT" )
BCI2000_ADD_CMDLINE_FILTER( IIRBandpass           FROM ${SIGPROC_DIR} INCLUDING "MATH"
                            EXTRA_SOURCES ${SIGPROC_DIR}/IIRFilterBase.cpp )


# The MatlabFilter must be listed last, else all filter executables will depend on libeng and libmx.
ADD_DEFINITIONS( -DDISABLE_BCITEST )
BCI2000_ADD_CMDLINE_FILTER( MatlabFilter          FROM ${PROJECT_SOURCE_ROOT}/src/core/SignalProcessing/Matlab INCLUDING "MATLAB_ENGINE"
                            EXTRA_SOURCES ${PROJECT_SOURCE_ROOT}/src/core/SignalProcessing/Matlab/MatlabWrapper.cpp
                            EXTRA_HEADERS ${PROJECT_SOURCE_ROOT}/src/core/SignalProcessing/Matlab/MatlabWrapper.h   )
