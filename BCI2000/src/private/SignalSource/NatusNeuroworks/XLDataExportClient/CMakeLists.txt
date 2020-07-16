###########################################################################
## $Id: CMakeLists.txt 5480 2016-07-27 23:56:28Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000Automation

BCI2000_INCLUDE( COM )

utils_set_appicon( puzzlepiece )
BCI2000_ADD_TARGET(
  WINDLL XLDataExportClient

  XLDataExportClient.cpp
  XLDataExportClient_i.c
  XLDataExportClient.rc
  XLDataExportReceiver.cpp
  Resource.h

  InstallXLDataExportClient.cmd
)
