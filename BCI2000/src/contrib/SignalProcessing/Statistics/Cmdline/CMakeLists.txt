###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors:
## Description: Build information for StatisticsFilter cmdline tool
##              
##              
###########################################################################

BCI2000_ADD_CMDLINE_FILTER(
  StatisticsFilter FROM ${BCI2000_SRC_DIR}/shared/modules/signalprocessing/statistics
  EXTRA_SOURCES ${SRC_STAT}
  EXTRA_HEADERS ${HDR_STAT}
)
