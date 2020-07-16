###########################################################################
## $Id: CMakeLists.txt 5826 2018-11-23 14:14:08Z mellinger $
## Authors: Adam Wilson, juergen.mellinger@neurotechcenter.org
## Description: Build information for BCI2000CertAnalysis

# Set the executable name
SET( NAME BCI2000CertAnalysis)

set(ENGINE AnalysisEngine)
set(ENGINE_OBJ ${ENGINE}/Objects)
set(ENGINE_STAT ${ENGINE}/Statistics)

# Set the project specific sources
SET( SOURCES 
  main.cpp
  AnalysisGui.cpp
  Analyzer.cpp
  ResultsTreeModel.cpp
  Gui/ScriptEditor.cpp
  Gui/CollapsibleGroup.cpp
  ../TaskType.cpp
  ../Property.cpp
  ${ENGINE}/AnalysisEngine.cpp
  ${ENGINE_STAT}/BasicStatistics.cpp
  ${ENGINE_STAT}/Histogram.cpp
  ${ENGINE_STAT}/Separability.cpp
  ${ENGINE}/Dataplot.cpp
  ${ENGINE_OBJ}/AnalysisDataObject.cpp
  ${ENGINE_OBJ}/SampleDataObject.cpp
  ${ENGINE_OBJ}/TimeSeriesDataObject.cpp
  ${ENGINE_OBJ}/SampleClock.cpp
  ${ENGINE_OBJ}/Timestamp.cpp
  ${ENGINE_OBJ}/ThresholdAnalysis.cpp
  ${ENGINE_OBJ}/InputData.cpp
  ${ENGINE_OBJ}/ComputedTimeSeries.cpp
  ${ENGINE_OBJ}/Event.cpp
  ${ENGINE_OBJ}/Interval.cpp
  ${ENGINE_OBJ}/Statistics.cpp
  ${ENGINE_OBJ}/FrameStatistics.cpp
  ${ENGINE_OBJ}/Group.cpp
  ${PROJECT_SRC_DIR}/shared/utils/QtSignalConnector.h
)
if(WIN32)
  list(APPEND SOURCES LIBRARY shlwapi)
endif()
SET( DIR_NAME Core/Tools/Certification )
utils_add_resource(InitScript Scripts/Init.bciscript)
utils_add_resource(DefaultInputScript Scripts/DefaultInput.bciscript)
utils_add_resource(LatencyAnalysisScript Scripts/LatencyAnalysis.bciscript)
utils_add_resource(LatencyRequirements Scripts/LatencyRequirements.bcireq)
utils_add_resource(VideoFrameAnalysisScript Scripts/VideoFrameAnalysis.bciscript)
utils_add_resource(VideoFrameRequirements Scripts/VideoFrameRequirements.bcireq)
BCI2000_ADD_TOOLS_GUIAPP(
  ${NAME}
  ${SOURCES}
  OUTPUT_DIRECTORY ${PROJECT_BUILD_ROOT}/tools/BCI2000Certification
)
