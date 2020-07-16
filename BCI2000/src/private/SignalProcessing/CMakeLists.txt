#############################################################################
## $Id: CMakeLists.txt 5800 2018-09-13 19:07:28Z abelsten $
## Authors: jezhill@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}SignalProcessing/" )

ADD_SUBDIRECTORY( ARSignalProcessing_Hijacked )
ADD_SUBDIRECTORY( NeuroFeedbackSignalProcessing )

ADD_SUBDIRECTORY( IIRFilterBankSignalProcessing )
ADD_SUBDIRECTORY( StimResponseSignalProcessing )
ADD_SUBDIRECTORY( ReferenceViewFilter )
ADD_SUBDIRECTORY( CP2130 )
ADD_SUBDIRECTORY( TBSIStimulator )
