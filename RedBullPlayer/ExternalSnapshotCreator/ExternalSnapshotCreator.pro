# -------------------------------------------------
# Project created by QtCreator 2010-01-08T10:25:37
# -------------------------------------------------
include( "../../globals.pri" )
TEMPLATE = app
QT += network \
    xml \
    svg \
    sql
TARGET = ExternalSnapshotCreator
CONFIG += console
CONFIG -= app_bundle
SOURCES += main.cpp \
    InputParser.cpp \
    SnapshotEngine.cpp \
    SnapshotCreationData.cpp \
    ArgumentParser.cpp \
    CommandProcessor.cpp
HEADERS += InputParser.h \
    SnapshotEngine.h \
    SnapshotCreationData.h \
    ArgumentParser.h \
    CommandProcessor.h

# INCLUDEPATH += ../../3rdParty/Common/log4qt/src/
LOG4QT_LIBDIR = ../../3rdParty/Common/log4qt/lib/release
win32_libs = $$DESTDIR/QTVLC.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/Interfaces.dll \
    $$DESTDIR/ToolLib.dll \
    $$LOG4QT_LIBDIR/log4qt-0.3.dll
unix_libs = $$DESTDIR/libQTVLC.so \
    ../../3rdParty/src/vlc-1.0.x-BuildDir/Linux_runtime/libvlc.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libToolLib.so \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.so.1.0.0
macx_libs = $$DESTDIR/libQTVLC.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.1.0.0.dylib
include("../../libref.pri")
macx:generate_library_list.commands = echo \
    $$TARGET \
    >> \
    $$DESTDIR/../Setup/DYNLIBS
macx:POST_TARGETDEPS += generate_library_list
macx:QMAKE_EXTRA_TARGETS += generate_library_list
