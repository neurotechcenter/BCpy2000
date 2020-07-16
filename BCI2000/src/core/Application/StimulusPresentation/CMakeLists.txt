###########################################################################
## $Id: CMakeLists.txt 5421 2016-07-03 18:55:36Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for StimulusPresentation in Qt

bci2000_add_application_module(
  StimulusPresentation
  StimulusPresentationTask.cpp
  SUPPORT /prog/images/*
  SUPPORT /prog/sounds/*
  SUPPORT /prog/video/*
  SUPPORT /parms/examples/P3Speller*.prm
)
