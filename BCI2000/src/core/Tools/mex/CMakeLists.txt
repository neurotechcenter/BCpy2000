# $Id$

set( DIR_NAME "${DIR_NAME}mex/" )
set_crt_link_type( STATIC )

bci2000_add_mexfile( convert_bciprm convert_bciprm.cpp )
bci2000_add_mexfile( load_bcidat load_bcidat.cpp )
bci2000_add_mexfile( save_bcidat
  save_bcidat.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/dat/BCI2000OutputFormat.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/EDFHeader.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/EDFOutputBase.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/EDFOutputFormat.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/GDF.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/edf_gdf/GDFOutputFormat.cpp
)
# workaround for not being able to create DLLs with bundles (yet)
foreach( flag_var CMAKE_CXX_FLAGS;CMAKE_C_FLAGS )
  string( REGEX REPLACE ".openmp" "" ${flag_var} "${${flag_var}}" )
endforeach()
bci2000_add_mexfile( mem
  mem.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ARFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ARSpectrum.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/Spectrum.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/WindowingFilter.cpp
  ${PROJECT_SRC_DIR}/extlib/math/MEMPredictor.h
)

