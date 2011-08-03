# -------------------------------------------------
# Project created by QtCreator 2010-01-18T14:45:44
# -------------------------------------------------
include( "../../globals.pri" )
QT += network \
    xml \
    gui \
    sql
TARGET = Updtr
unix:!macx:TARGET=Updtr.com

#CONFIG += console
#CONFIG -= windows
CONFIG -= console
CONFIG += windows

CONFIG -= app_bundle
TEMPLATE = app
HEADERS += UpdateEngine.h \
    ConfigFileTask.h \
    UpdateHistoryUpdateTask.h \
    MacOSXInstallerTask.h \
    UpdateManager.h \
    UpdateProcess.h \
    UpdateTask.h \
    ../../RedBullPlayer/Interfaces/StyleFactory/ITextStyle.h \
    ../../RedBullPlayer/ToolLib/SimpleLogger.h \
    ../../RedBullPlayer/ToolLib/UuidCreator.h \
    ../../RedBullPlayer/Container/Constants.h \
    ../../RedBullPlayer/UpdateServices/PlayerVersion.h \
    ../../RedBullPlayer/UpdateServices/Version.h \
    ../../RedBullPlayer/Package/ArchivData.h \
    ../../RedBullPlayer/Package/ArchivFile.h \
    ../../RedBullPlayer/Package/ArchivBuilder.h \
    ../../RedBullPlayer/Package/ArchivIndex.h \
    ../../RedBullPlayer/Package/ArchivFileReference.h \
    ../../RedBullPlayer/Package/ArchivDataReader.h \
    ../../RedBullPlayer/Package/Package.h \
    ../../RedBullPlayer/Widgets/UpdateProgressWidget.h \
    ../../RedBullPlayer/Widgets/CommandButton.h \
    v0_9_to_1_0/ConfigFileTask_0_9_to_1_0.h \
    DatabaseTask.h \
    Win32InstallerTask.h
SOURCES += updater.cpp \
    UpdateEngine.cpp \
    ConfigFileTask.cpp \
    UpdateHistoryUpdateTask.cpp \
    MacOSXInstallerTask.cpp \
    UpdateManager.cpp \
    UpdateProcess.cpp \
    ../../RedBullPlayer/ToolLib/SimpleLogger.cpp \
    ../../RedBullPlayer/ToolLib/UuidCreator.cpp \
    ../../RedBullPlayer/Container/Constants.cpp \
    ../../RedBullPlayer/UpdateServices/PlayerVersion.cpp \
    ../../RedBullPlayer/UpdateServices/Version.cpp \
    ../../RedBullPlayer/Package/ArchivData.cpp \
    ../../RedBullPlayer/Package/ArchivFile.cpp \
    ../../RedBullPlayer/Package/ArchivBuilder.cpp \
    ../../RedBullPlayer/Package/ArchivIndex.cpp \
    ../../RedBullPlayer/Package/ArchivFileReference.cpp \
    ../../RedBullPlayer/Package/ArchivDataReader.cpp \
    ../../RedBullPlayer/Package/Package.cpp \
    ../../RedBullPlayer/Widgets/UpdateProgressWidget.cpp \
    ../../RedBullPlayer/Widgets/CommandButton.cpp \
    v0_9_to_1_0/ConfigFileTask_0_9_to_1_0.cpp \
    DatabaseTask.cpp \
    Win32InstallerTask.cpp
FORMS += ../../RedBullPlayer/Widgets/UpdateProgressWidget.ui
win32_libs = ../../3rdParty/Common/log4qt/lib/release/log4qt-0.3.dll
unix_libs = ../../3rdParty/Common/log4qt/lib/release/liblog4qt-0.3.so.1.0.0
macx_libs = ../../3rdParty/Common/log4qt/lib/release/liblog4qt-0.3.1.0.0.dylib
include("../../libref.pri")
INCLUDEPATH += ../../3rdParty/Common/log4qt/src/ \
    ../../RedBullPlayer/ToolLib/ \
    ../../RedBullPlayer/Container/ \
    ../../RedBullPlayer/UpdateServices/ \
    ../../RedBullPlayer/Package/ \
    ../../RedBullPlayer/Widgets/
OTHER_FILES += 
RESOURCES += Updater.qrc
