# -------------------------------------------------
# Project created by QtCreator 2010-04-28T13:34:41
# -------------------------------------------------
TARGET = BCI2000Export
TEMPLATE = app
SOURCES += main.cpp \
    MainWindow.cpp \
    BCIReader.cpp \
    Converters/ASCIIConverter.cpp \
    Converters/BrainVisionConverter.cpp \
    ../../../shared/fileio/dat/BCI2000FileReader.cpp \
    ../../../shared/types/StateVectorSample.cpp \
    ../../../shared/types/StateVector.cpp \
    ../../../shared/types/StateList.cpp \
    ../../../shared/types/State.cpp \
    ../../../shared/types/GenericSignal.cpp \
    ../../../shared/types/SignalType.cpp \
    ../../../shared/types/SignalProperties.cpp \
    ../../../shared/types/PhysicalUnit.cpp \
    ../../../shared/types/ParamList.cpp \
    ../../../shared/types/Param.cpp \
    ../../../shared/types/LabelIndex.cpp \
    ../../../shared/types/Brackets.cpp \
    ../../../shared/types/EncodedString.cpp \
    ../../../shared/types/HierarchicalLabel.cpp \
    ../../../shared/accessors/ParamRef.cpp \
    ../../../shared/bcistream/BCIError.cpp \
    ../../../shared/gui/ExecutableHelp.cpp \
    ../../../shared/gui/AboutBox.cpp \
    ../../../shared/utils/VersionInfo.cpp \
    ../../../shared/utils/Expression/ArithmeticExpression.cpp \
    ../../../shared/utils/Expression/ExpressionParser.cpp
HEADERS += MainWindow.h \
    ../../../shared/fileio/dat/BCI2000FileReader.h
FORMS += 
INCLUDEPATH += \
    Converters \
    ../../../shared \
    ../../../shared/config \
    ../../../shared/accessors \
    ../../../shared/types \
    ../../../shared/gui \
    ../../../shared/bcistream \
    ../../../shared/fileio/dat \
    ../../../shared/utils \
    ../../../shared/utils/Expression
