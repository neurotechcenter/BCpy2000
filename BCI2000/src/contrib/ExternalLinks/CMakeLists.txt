###########################################################################
## $Id: CMakeLists.txt 4536 2013-08-05 14:30:13Z mellinger $
## Authors: griffin.milsap@gmail.com
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}ExternalLinks/" )

# Recurse down into all project subdirectories
ADD_SUBDIRECTORY( BCI2000Automation )
ADD_SUBDIRECTORY( BCI2000Command )
ADD_SUBDIRECTORY( BCI2000PresentationLink )
ADD_SUBDIRECTORY( BCI2000PythonBindings )
ADD_SUBDIRECTORY( PyffApplication )
