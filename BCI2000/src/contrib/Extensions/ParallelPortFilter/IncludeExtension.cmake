###########################################################################
## $Id: IncludeExtension.cmake 4536 2013-08-05 14:30:13Z mellinger $
## Authors: kaleb.goering@gmail.com, juergen.mellinger@uni-tuebingen.de

if( NOT WIN32 )
  message( "**** ParallelPort Extension not available on this platform" )
  return()
endif()

# Add the ParallelPortLogger to all source modules
list( APPEND BCI2000_SIGSRC_FILES
   ${BCI2000_EXTENSION_DIR}/ParallelPortLogger.cpp
   ${BCI2000_EXTENSION_DIR}/ParallelPort.cpp
)
list( APPEND BCI2000_SIGSRC_LIBS
  Cfgmgr32
)

# Add the ParallelPortFilter to all application modules
list( APPEND BCI2000_APP_FILES
   ${BCI2000_EXTENSION_DIR}/ParallelPortFilter.cpp
   ${BCI2000_EXTENSION_DIR}/ParallelPort.cpp
)
list( APPEND BCI2000_APP_LIBS
  Cfgmgr32
)
