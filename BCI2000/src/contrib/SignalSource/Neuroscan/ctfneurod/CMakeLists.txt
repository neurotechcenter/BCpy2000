# CMakeLists.txt for building the NeuroSrv Neuroscan protocol server demo
# executable on any platform.
# Designed to be used outside BCI2000 without change.

ADD_EXECUTABLE(
  demoneurod
  demoneurod.cc
  NeuroSrv.cc
  NeuroSrv.h
  SockStream.cc
  SockStream.h
  EDFHeader.cc
  EDFHeader.h
  GDF.h
  NeuroscanProtocol.h
)

IF( WIN32 )
  TARGET_LINK_LIBRARIES( demoneurod ws2_32 )
ENDIF( WIN32 )

IF( DIR_NAME )
  SET_PROPERTY( TARGET demoneurod PROPERTY FOLDER ${DIR_NAME} )
ENDIF( DIR_NAME )
