SET( SIGPROC
  ${BCI2000_SRC_DIR}/shared/modules/signalprocessing
)
INCLUDE_DIRECTORIES( ${SIGPROC} )

BCI2000_ADD_CMDLINE_FILTER( CustomIIRFilter FROM ..
                            INCLUDING MATH
                            EXTRA_SOURCES ${SIGPROC}/IIRFilterBase.cpp
                                          ${SIGPROC}/IIRBandpass.cpp
                            EXTRA_HEADERS ${SIGPROC}/IIRFilterBase.h
                                          ${SIGPROC}/IIRBandpass.h
                           )
BCI2000_ADD_CMDLINE_FILTER( CustomFIRFilter FROM .. )
BCI2000_ADD_CMDLINE_FILTER( HilbertFilter FROM .. )
BCI2000_ADD_CMDLINE_FILTER( DiffFilter FROM .. )
