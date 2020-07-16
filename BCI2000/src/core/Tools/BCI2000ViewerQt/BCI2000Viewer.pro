# -------------------------------------------------
# Project created by QtCreator 2010-01-26T16:26:41
# -------------------------------------------------
TARGET = BCI2000Viewer
TEMPLATE = app
SOURCES += main.cpp \
    BCI2000Viewer.cpp \
    ../../../shared/types/GenericSignal.cpp \
    ../../../shared/fileio/dat/BCI2000FileReader.cpp \
    ../../../shared/gui/SignalDisplay.cpp \
    ../../../shared/types/Color.cpp \
    ../../../shared/types/SignalType.cpp \
    ../../../shared/types/SignalProperties.cpp \
    ../../../shared/types/ParamList.cpp \
    ../../../shared/types/LabelIndex.cpp \
    ../../../shared/types/StateVectorSample.cpp \
    ../../../shared/types/StateVector.cpp \
    ../../../shared/types/StateList.cpp \
    ../../../shared/utils/OSMutex.cpp \
    ../../../shared/types/PhysicalUnit.cpp \
    ../../../shared/types/Param.cpp \
    ../../../shared/types/Brackets.cpp \
    ../../../shared/types/EncodedString.cpp \
    ../../../shared/types/State.cpp \
    ../../../shared/utils/Expression/ArithmeticExpression.cpp \
    ../../../shared/utils/Expression/ExpressionParser.cpp \
    ../../../shared/bcistream/BCIError.cpp \
    ../../../shared/bcistream/BCIError_guiapp.cpp \
    ../../../shared/types/HierarchicalLabel.cpp \
    ../../../shared/accessors/ParamRef.cpp \
    ../../../shared/gui/ExecutableHelp.cpp \
    ../../../shared/gui/AboutBox.cpp \
    ../../../shared/utils/VersionInfo.cpp \
    ../../../shared/gui/ColorListChooser.cpp \
    ../../../shared/utils/DisplayFilter.cpp \
    ../../../extlib/math/FilterDesign.cpp \
    SignalWidget.cpp
HEADERS += BCI2000Viewer.h \
    SignalWidget.h
FORMS += BCI2000Viewer.ui
INCLUDEPATH += ../../../shared/config \
    ../../../shared \
    ../../../shared/utils \
    ../../../shared/utils/Expression \
    ../../../shared/types \
    ../../../shared/modules \
    ../../../shared/bcistream \
    ../../../shared/accessors \
    ../../../shared/fileio \
    ../../../shared/fileio/dat \
    ../../../shared/gui \
    ../../../extlib/math
