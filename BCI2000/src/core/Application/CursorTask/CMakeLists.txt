###########################################################################
## $Id: CMakeLists.txt 5421 2016-07-03 18:55:36Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for CursorTask in Qt

bci2000_add_application_module(
  CursorTask
  CursorFeedbackTask.cpp
  FeedbackScene2D.cpp
  FeedbackScene3D.cpp
  FeedbackScene.h
  SUPPORT /prog/images/*
  SUPPORT /parms/examples/CursorTask*.prm
)
