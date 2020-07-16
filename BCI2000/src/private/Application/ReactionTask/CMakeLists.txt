###########################################################################
## $Id:  $
## Authors:
## Description: Build information for the ReactionTask application module
##              
##              
###########################################################################

BCI2000_ADD_APPLICATION_MODULE( ReactionTask
  ReactionTaskFilter.cpp
  ${BCI2000_SRC_DIR}/contrib/Extensions/StimBoxFilter/lib/gSTIMbox.imports.cpp
)

