###########################################################################
## $Id: CMakeLists.txt 5421 2016-07-03 18:55:36Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for FeedbackDemo in Qt

bci2000_add_application_module(
  FeedbackDemo
  FeedbackDemoTask.cpp
  SUPPORT /parms/examples/FeedbackDemo*.prm
)


