###########################################################################
## $Id:  $
## Authors: kaleb.goering@gmail.com
## Description: Build information for the gNautilusSource source module
###########################################################################
if( NOT WIN32 )
  message( "gNautilusSource does not support this OS, will not build" )
else()

  SET( EXECUTABLE_NAME gNautilusSource )

  SET( FILES_PROJECT
    gNautilusADC.cpp
    gNautilusDevice.cpp
    lib/GDSClientAPI.imports.cpp
  )

  IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET( FILES_PROJECT
      ${FILES_PROJECT}
      lib/coff64/gAPI.dll
      lib/coff64/GDSServer.dll
      lib/coff64/GDSClientAPI.dll
      lib/coff64/Networking.dll
    )
  ELSE()
    SET( FILES_PROJECT
      ${FILES_PROJECT}
      lib/coff/gAPI.dll
      lib/coff/GDSServer.dll
      lib/coff/GDSClientAPI.dll
      lib/coff/Networking.dll
    )
  ENDIF()

  BCI2000_ADD_SIGNAL_SOURCE_MODULE( 
    "${EXECUTABLE_NAME}" 
    "${FILES_PROJECT}" 
    COMPANION lib/config/GDSAmplifierFeatures.h5=config/GDSAmplifierFeatures.h5
  )

endif( NOT WIN32 )
