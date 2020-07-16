###########################################################################
## $Id: CMakeLists.txt 5421 2016-07-03 18:55:36Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for P3Speller in Qt

BCI2000_ADD_APPLICATION_MODULE( 
  P3Speller
  P3SpellerTask.cpp
  SUPPORT /prog/images/*
  SUPPORT /parms/examples/P3Speller*.prm
)
