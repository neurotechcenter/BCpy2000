###########################################################################
## $Id: CMakeLists.txt 5826 2018-11-23 14:14:08Z mellinger $
## Authors: Adam Wilson
## Description: Build information for BCI2000CertificationTester

# Set the executable name
SET( NAME BCI2000CertificationTester )

# Set the project specific sources
SET( SOURCES 
  main.cpp
  CertLauncherGui.cpp
  CertLauncher.cpp
  ../TaskType.cpp
  ../Property.cpp
  SUPPORT /tools/BCI2000Certification/*
  SUPPORT /tools/BCI2000Certification/audio/*
  SUPPORT /tools/BCI2000Certification/batch/*
  SUPPORT /tools/BCI2000Certification/images/*
  SUPPORT /tools/BCI2000Certification/video/*
  SUPPORT /tools/BCI2000Certification/parms/*
)


INCLUDE_DIRECTORIES( . ../ )

SET( DIR_NAME Core/Tools/Certification )
BCI2000_USE_INVENTORY( SOURCES )
BCI2000_ADD_TOOLS_GUIAPP(
  ${NAME}
  ${SOURCES}
  OUTPUT_DIRECTORY ${PROJECT_BUILD_ROOT}/tools/BCI2000Certification
)
