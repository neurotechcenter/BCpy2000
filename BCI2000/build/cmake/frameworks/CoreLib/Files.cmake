###########################################################################
## $Id: BasicFramework.cmake 3482 2011-08-23 17:15:02Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: List of files in a basic BCI2000 framework

SET( SRC_LIBCORE

  ${PROJECT_SRC_DIR}/shared/config/VersionInfo.cpp
  ${PROJECT_SRC_DIR}/shared/utils/LogFile.cpp
  ${PROJECT_SRC_DIR}/shared/utils/RandomGenerator.cpp
  ${PROJECT_SRC_DIR}/shared/utils/MeasurementUnits.cpp
  ${PROJECT_SRC_DIR}/shared/utils/PrecisionTime.cpp
  ${PROJECT_SRC_DIR}/shared/utils/IndexList.cpp
  ${PROJECT_SRC_DIR}/shared/utils/EventQueue.cpp
  ${PROJECT_SRC_DIR}/shared/utils/FPExceptMask.h
  ${PROJECT_SRC_DIR}/shared/utils/Resource.h
  ${PROJECT_SRC_DIR}/shared/utils/AsyncIODevice.cpp
  ${PROJECT_SRC_DIR}/shared/utils/SimpleStatistics.h

  ${PROJECT_SRC_DIR}/shared/utils/Expression/ArithmeticExpression.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Expression/Expression.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Expression/ExpressionNodes.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Expression/ExpressionParser.cpp
  ${PROJECT_SRC_DIR}/shared/utils/Expression/ExpressionParser.hpp

  ${PROJECT_SRC_DIR}/shared/filters/GenericFilter.cpp
  ${PROJECT_SRC_DIR}/shared/filters/ChoiceCombination.cpp
  ${PROJECT_SRC_DIR}/shared/filters/FilterCombination.cpp
  ${PROJECT_SRC_DIR}/shared/filters/StandaloneFilters.cpp
  ${PROJECT_SRC_DIR}/shared/filters/IdentityFilter.h
  ${PROJECT_SRC_DIR}/shared/filters/SubchainFilter.h
  ${PROJECT_SRC_DIR}/shared/filters/SignalStream.h

  ${PROJECT_SRC_DIR}/shared/modules/MessageChannel.cpp
  ${PROJECT_SRC_DIR}/shared/modules/CoreConnection.cpp

  ${PROJECT_SRC_DIR}/shared/types/BitmapImage.cpp
  ${PROJECT_SRC_DIR}/shared/types/Blob.cpp
  ${PROJECT_SRC_DIR}/shared/types/Tree.cpp
  ${PROJECT_SRC_DIR}/shared/types/Brackets.cpp
  ${PROJECT_SRC_DIR}/shared/types/CfgID.cpp
  ${PROJECT_SRC_DIR}/shared/types/Color.cpp
  ${PROJECT_SRC_DIR}/shared/types/EncodedString.cpp
  ${PROJECT_SRC_DIR}/shared/types/GenericSignal.cpp
  ${PROJECT_SRC_DIR}/shared/types/GenericVisualization.cpp
  ${PROJECT_SRC_DIR}/shared/types/HierarchicalLabel.cpp
  ${PROJECT_SRC_DIR}/shared/types/Label.cpp
  ${PROJECT_SRC_DIR}/shared/types/LabelIndex.cpp
  ${PROJECT_SRC_DIR}/shared/types/Param.cpp
  ${PROJECT_SRC_DIR}/shared/types/ParamList.cpp
  ${PROJECT_SRC_DIR}/shared/types/ParamListView.cpp
  ${PROJECT_SRC_DIR}/shared/types/PhysicalUnit.cpp
  ${PROJECT_SRC_DIR}/shared/types/SignalProperties.cpp
  ${PROJECT_SRC_DIR}/shared/types/SignalType.cpp
  ${PROJECT_SRC_DIR}/shared/types/State.cpp
  ${PROJECT_SRC_DIR}/shared/types/StateList.cpp
  ${PROJECT_SRC_DIR}/shared/types/StateVector.cpp
  ${PROJECT_SRC_DIR}/shared/types/Status.cpp
  ${PROJECT_SRC_DIR}/shared/types/SysCommand.cpp
  ${PROJECT_SRC_DIR}/shared/types/VisID.cpp
  ${PROJECT_SRC_DIR}/shared/types/ProtocolVersion.h  

  ${PROJECT_SRC_DIR}/shared/accessors/BCIEvent.cpp
  ${PROJECT_SRC_DIR}/shared/accessors/Environment.cpp
  ${PROJECT_SRC_DIR}/shared/accessors/ParamRef.cpp
  ${PROJECT_SRC_DIR}/shared/accessors/StateRef.cpp

  ${PROJECT_SRC_DIR}/shared/bcistream/BCIStream.cpp
  ${PROJECT_SRC_DIR}/shared/bcistream/BCIException.h

  ${PROJECT_SRC_DIR}/shared/fileio/RunManager.cpp
  ${PROJECT_SRC_DIR}/shared/fileio/dat/BCI2000FileReader.cpp
  ${PROJECT_SRC_DIR}/extlib/math/FastConv.h
)

IF( WIN32 )
  SET( SRC_LIBCORE
    ${SRC_LIBCORE}
    ${PROJECT_SRC_DIR}/shared/utils/SerialStream.cpp
  )
ENDIF()
