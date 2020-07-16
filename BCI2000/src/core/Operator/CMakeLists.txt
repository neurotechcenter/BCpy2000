###########################################################################
## $Id: CMakeLists.txt 5369 2016-05-20 14:57:42Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000 and CMake

# Set this directory name
SET( DIR_NAME "${DIR_NAME}Operator/" )

# Recurse down into all project subdirectories
ADD_SUBDIRECTORY( OperatorLib )
ADD_SUBDIRECTORY( OperatorQt )
ADD_SUBDIRECTORY( BCI2000Remote )
ADD_SUBDIRECTORY( BCI2000Shell )
ADD_SUBDIRECTORY( MessageBox )

