###########################################################################
## $Id: CMakeLists.txt 5398 2016-06-15 19:16:30Z moheimanian $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}Application/" )

# Add this to the CMAKELists.txt in this directory
ADD_SUBDIRECTORY( Games )
ADD_SUBDIRECTORY( AttentionTask )
ADD_SUBDIRECTORY( CursorTaskWithGauges )
ADD_SUBDIRECTORY( GaugesOnly )
ADD_SUBDIRECTORY( ReactionTask )
ADD_SUBDIRECTORY( NeurofeedbackTask )
ADD_SUBDIRECTORY( ReactionAuditoryCalibrationTask )
add_subdirectory( ReactionAuditoryTask )
