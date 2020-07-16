
SET( GAUGESOURCES  
  ${GAUGE_DIR}/Gauge.cpp
  ${GAUGE_DIR}/Scale.cpp
  ${GAUGE_DIR}/GaugeGallery/AnalogGauge.cpp
  ${GAUGE_DIR}/GaugeGallery/BarGauge.cpp
  ${GAUGE_DIR}/GaugeGallery/BarPlotGauge.cpp
  ${GAUGE_DIR}/GaugeGallery/HistoryGauge.cpp
  ${GAUGE_DIR}/GaugeGallery/ScrollingHistoryGauge.cpp
  ${GAUGE_DIR}/GaugeGallery/XYGauge.cpp
)

SET( GAUGEHEADERS
  ${GAUGE_DIR}/Gauge.h
  ${GAUGE_DIR}/Scale.h
  ${GAUGE_DIR}/GaugeGallery/AnalogGauge.h
  ${GAUGE_DIR}/GaugeGallery/BarGauge.h
  ${GAUGE_DIR}/GaugeGallery/BarPlotGauge.h
  ${GAUGE_DIR}/GaugeGallery/HistoryGauge.h
  ${GAUGE_DIR}/GaugeGallery/ScrollingHistoryGauge.h
  ${GAUGE_DIR}/GaugeGallery/XYGauge.h
)

SET( SRC_PROJECT
  ${SRC_PROJECT}
  ${BCI2000_SRC_DIR}/private/shared/utils/Quantiles.cpp
#  ${GAUGE_DIR}/GaugeApp.cpp
  ${GAUGESOURCES}
)
SET( HDR_PROJECT
  ${HDR_PROJECT}
  ${BCI2000_SRC_DIR}/private/shared/utils/Quantiles.h
#  ${GAUGE_DIR}/GaugeApp.h
  ${GAUGEHEADERS}
)

INCLUDE_DIRECTORIES(
  ${BCI2000_SRC_DIR}/private/shared/utils
  ${GAUGE_DIR}
  ${GAUGE_DIR}/GaugeGallery
)

# Set the Project Source Groups
SOURCE_GROUP( Source\\Gauge_Framework FILES ${GAUGESOURCES} )
SOURCE_GROUP( Headers\\Gauge_Framework FILES ${GAUGEHEADERS} )
