# -------------------------------------------------
# Project created by QtCreator 2009-07-09T14:52:46
# -------------------------------------------------
TARGET = Operator
TEMPLATE = app

# QMAKE_CFLAGS += -g -pg
# QMAKE_CXXFLAGS += -g -pg
# QMAKE_LFLAGS += -g -pg
SOURCES += main.cpp \
    MainWindow.cpp \
    Preferences.cpp \
    OperatorUtils.cpp \
    ../../../shared/types/ParamList.cpp \
    ../../../shared/types/Param.cpp \
    ../../../shared/types/StateList.cpp \
    ../../../shared/types/State.cpp \
    ../../../shared/types/EncodedString.cpp \
    ../../../shared/types/Brackets.cpp \
    ../../../shared/types/LabelIndex.cpp \
    ../../../shared/types/HierarchicalLabel.cpp \
    ../../../shared/types/StateVector.cpp \
    ../../../shared/types/StateVectorSample.cpp \
    PrefDialog.cpp \
    ../../../shared/utils/ClassName.cpp \
    ConnectionInfo.cpp \
    SysLog.cpp \
    ConfigWindow.cpp \
    ParsedComment.cpp \
    ParamDisplay.cpp \
    ../../../shared/types/Color.cpp \
    EditMatrix.cpp \
    ../../../shared/gui/ExecutableHelp.cpp \
    ShowParameters.cpp \
    ../../../shared/gui/AboutBox.cpp \
    ../../../shared/utils/VersionInfo.cpp \
    ShowStates.cpp \
    ../../../shared/fileio/dat/BCI2000FileReader.cpp \
    ../../../shared/types/SignalProperties.cpp \
    ../../../shared/types/PhysicalUnit.cpp \
    ../../../shared/types/SignalType.cpp \
    ../../../shared/utils/Expression/ArithmeticExpression.cpp \
    ../../../shared/utils/Expression/ExpressionParser.cpp \
    ../../../shared/bcistream/BCIError.cpp \
    ../../../shared/bcistream/BCIError_guiapp.cpp \
    VisDisplay.cpp \
    ../../../extlib/math/FilterDesign.cpp \
    ../../../shared/types/GenericSignal.cpp \
    ../../../shared/gui/SignalDisplay.cpp \
    ../../../shared/types/Label.cpp \
    VisDisplayBase.cpp \
    VisDisplayMemo.cpp \
    VisDisplayGraph.cpp \
    VisDisplayBitmap.cpp \
    ../../../shared/utils/DisplayFilter.cpp \
    ../../../shared/utils/OSThread.cpp \
    ../../../shared/utils/OSError.cpp \
    ../../../shared/utils/OSMutex.cpp \
    ../../../shared/gui/ColorListChooser.cpp
HEADERS += MainWindow.h \
    ../OperatorLib/BCI_OperatorLib.h \
    Preferences.h \
    OperatorUtils.h \
    ../../../shared/types/ParamList.h \
    ../../../shared/types/Param.h \
    ../../../shared/types/StateList.h \
    ../../../shared/types/State.h \
    ../../../shared/types/Brackets.h \
    ../../../shared/types/LabelIndex.h \
    ../../../shared/types/EncodedString.h \
    ../../../shared/types/HierarchicalLabel.h \
    ../../../shared/types/StateVector.h \
    ../../../shared/types/StateVectorSample.h \
    PrefDialog.h \
    ../../../shared/utils/ClassName.h \
    ConnectionInfo.h \
    SysLog.h \
    ConfigWindow.h \
    ParamDisplay.h \
    ParsedComment.h \
    ../../../shared/types/Color.h \
    EditMatrix.h \
    ../../../shared/gui/ExecutableHelp.h \
    ShowParameters.h \
    ../../../shared/gui/AboutBox.h \
    ../../../shared/utils/VersionInfo.h \
    ShowStates.h \
    ../../../shared/fileio/dat/BCI2000FileReader.h \
    ../../../shared/types/SignalProperties.h \
    ../../../shared/types/SignalType.h \
    ../../../shared/types/PhysicalUnit.h \
    ../../../shared/utils/Expression/ArithmeticExpression.h \
    ../../../shared/utils/Expression/ExpressionParser.hpp \
    ../../../shared/bcistream/BCIError.h \
    VisDisplay.h \
    ../../../extlib/math/FilterDesign.h \
    ../../../extlib/math/IIRFilter.h \
    ../../../shared/types/GenericSignal.h \
    ../../../shared/gui/SignalDisplay.h \
    ../../../shared/types/Label.h \
    DisplayFilter.h \
    VisDisplayBase.h \
    VisDisplayGraph.h \
    VisDisplayBitmap.h \
    VisDisplayMemo.h \
    MetaTypes.h \
    ../../../shared/utils/DisplayFilter.h \
    ../../../shared/utils/OSThread.h \
    ../../../shared/utils/OSError.h \
    ../../../shared/utils/OSMutex.h
FORMS += MainWindow.ui \
    PrefDialog.ui \
    ConnectionInfo.ui \
    ConfigWindow.ui \
    EditMatrix.ui \
    ShowParameters.ui \
    ShowStates.ui
win32:LIBS += ../../../../prog/libOperatorLib.dll.a
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
    ../../../extlib/math \
    ../OperatorLib
