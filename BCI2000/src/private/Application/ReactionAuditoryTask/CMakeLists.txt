###########################################################################
## Authors:
## Description: Build information for the ReactionAuditoryTask application module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( 3DAPI )

bci2000_add_application_module(
  ReactionAuditoryTask
  ReactionAuditoryTask.cpp
)
