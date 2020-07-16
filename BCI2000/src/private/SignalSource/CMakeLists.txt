###########################################################################
## $Id: CMakeLists.txt 5848 2019-01-03 16:46:40Z madamek $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}SignalSource/" )

# ADD_SUBDIRECTORY( PrivateSourceModule )
#ADD_SUBDIRECTORY( g.USBamp_BufferedADC )

ADD_SUBDIRECTORY( NihonKohdenSource )
ADD_SUBDIRECTORY( NatusNeuroworks )
add_subdirectory( Cortec )


