###########################################################################
## $Id: CMakeLists.txt 5480 2016-07-27 23:56:28Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000Automation

BCI2000_INCLUDE( COM )

utils_set_appicon( puzzlepiece )
BCI2000_ADD_TARGET(
  WINDLL BCI2000Automation

  BCI2000Automation.cpp
  BCI2000Automation_i.c
  BCI2000Automation.rc
  CBCI2000Remote.cpp
  Resource.h
  ${BCI2000_SRC_DIR}/shared/utils/Lib/sockstream.cpp
  ${BCI2000_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Remote.cpp  
  ${BCI2000_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Connection.cpp
  
  InstallAutomation.cmd
)
