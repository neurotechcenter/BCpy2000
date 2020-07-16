# -------------------------------------------------
# Project created by QtCreator 2010-06-01T16:02:36
# -------------------------------------------------
TARGET = BCI2000Launcher
TEMPLATE = app
SOURCES += main.cpp \
    MainWindow.cpp \
    ../../../shared/gui/AboutBox.cpp \
    ../../../shared/gui/ExecutableHelp.cpp \
    ../../../shared/utils/VersionInfo.cpp \
    ../../../shared/utils/OSThread.cpp \
    ../../../shared/utils/OSError.cpp \
    ../../../shared/bcistream/BCIError.cpp \
    ../../../shared/bcistream/BCIError_guiapp.cpp \
    ../../../shared/types/EncodedString.cpp
HEADERS += MainWindow.h
FORMS += MainWindow.ui
INCLUDEPATH += ../../../shared/config \
    ../../../shared/gui \
    ../../../shared/utils \
    ../../../shared/bcistream \
    ../../../shared/types \
    ../../../shared
DESTDIR = ../../../../prog
