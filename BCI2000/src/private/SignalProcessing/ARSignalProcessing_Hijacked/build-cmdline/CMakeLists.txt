###########################################################################
## $Id: CMakeLists.txt 3505 2011-09-01 19:16:10Z jhill $
## Authors: jezhill@gmail.com
## Description: 

BCI2000_ADD_CMDLINE_FILTER( AudioEscapeFilter FROM .. )
BCI2000_ADD_CMDLINE_FILTER( HijackFilter FROM ..
                            EXTRA_SOURCES ../Gaussian.cpp
                            EXTRA_HEADERS ../Gaussian.h
                          )
BCI2000_ADD_CMDLINE_FILTER( FilterFilter  FROM ..
                            EXTRA_SOURCES ../CustomIIR.cpp
                            EXTRA_HEADERS ../CustomIIR.h
                            INCLUDING "MATH"
                          )
BCI2000_ADD_CMDLINE_FILTER( LogFilter FROM .. )
BCI2000_ADD_CMDLINE_FILTER( PairwiseCouplingFilter FROM .. )
