###########################################################################
## $Id: SignalSourceMacro.cmake 5750 2018-05-07 14:40:07Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Contains a macro for creating a signal source module

macro( bci2000_add_signal_source_module )
  utils_parse_args( "NAME;SOURCES" ${ARGV} )

  utils_include( frameworks/SigSrcModule )
  add_definitions( -DMODTYPE=1 )
  utils_set_appicon( signal-screen )
  bci2000_add_core_main( ${NAME} ${SOURCES} )
  bci2000_add_target(
    INFO "Signal source module"
    EXECUTABLE ${NAME}
    ${SOURCES}
  )
  if( NOT FAILED )
    bci2000_add_to_inventory( SignalSource ${NAME} )
  endif()

endmacro()
