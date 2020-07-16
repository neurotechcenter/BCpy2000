###########################################################################
## $Id: SigProcModule.cmake -1   $
## Authors: griffin.milsap@gmail.com, juergen.mellinger@uni-tuebingen.de

SET( SRC_BCI2000_FRAMEWORK
  ${SRC_BCI2000_FRAMEWORK}
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/SpatialFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/IIRFilterBase.cpp 
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/IIRBandpass.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ThreadedFilter.cpp  
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/WindowingFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/Spectrum.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ARSpectrum.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/FFTSpectrum.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/SpectralEstimator.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ARFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/P3TemporalFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/LinearClassifier.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/LPFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ExpressionFilter.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/Normalizer.cpp
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/FrequencyEstimator.cpp

  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/AverageDisplay.cpp 
#  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ComplexDemodulator.cpp 
#  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/ConditionalIntegrator.cpp 
  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/RandomFilter.cpp 
#  ${PROJECT_SRC_DIR}/shared/modules/signalprocessing/StateTransform.cpp 

  ${PROJECT_SRC_DIR}/shared/utils/Multithreading.cpp 
)
