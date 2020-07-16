###########################################################################
## $Id: CMakeLists.txt 5473 2016-07-23 01:02:25Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for P300Classifier

BCI2000_INCLUDE( ALGLIB )
BCI2000_ADD_TOOLS_GUIAPP( 
  P300Classifier
  main.cpp 
  CARfilter.cpp
  CDataFile.cpp
  Check_Consistency_Files.cpp
  CmdLine.cpp
  configdialog.cpp
  configdialog.qrc
  downsampling.cpp
  filter.cpp
  GetP3Responses.cpp
  GetScore.cpp
  in_out_variable.cpp
  load_BCIdat.cpp
  nrerror.cpp
  P300_Classify.cpp
  pages.cpp
  ReadIniParameters.cpp
  solve_linear_equation.cpp
  StimulusPresentation_Classify.cpp
  SWLDA.cpp
  WritePRM.cpp
)
