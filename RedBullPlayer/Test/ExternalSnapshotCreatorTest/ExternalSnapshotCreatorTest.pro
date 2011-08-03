include( "../../../globals.pri" )
include( "../../../unittest.pri" )
TEMPLATE = app
TARGET = ExternalSnapshotCreatorTest
CONFIG -= app_bundle
QT += testlib \
    network \
    xml \
    svg \
    sql
win32_libs = $$APPLICATION_DESTDIR/Interfaces.dll \
    $$APPLICATION_DESTDIR/Container.dll \
    $$APPLICATION_DESTDIR/PlayerShell.dll \
    $$APPLICATION_DESTDIR/EventAggregator.dll \
    $$APPLICATION_DESTDIR/Events.dll \
    $$APPLICATION_DESTDIR/ToolLib.dll \
    $$APPLICATION_DESTDIR/Widgets.dll \
    $$DESTDIR/FakeLib.dll \
    $$APPLICATION_DESTDIR/QTVLC.dll

unix_libs = $$APPLICATION_DESTDIR/libInterfaces.so \
    $$APPLICATION_DESTDIR/libContainer.so \
    $$APPLICATION_DESTDIR/libPlayerShell.so \
    $$APPLICATION_DESTDIR/libEventAggregator.so \
    $$APPLICATION_DESTDIR/libEvents.so \
    $$APPLICATION_DESTDIR/libWidgets.so \
    $$APPLICATION_DESTDIR/libToolLib.so \
    $$DESTDIR/libFakeLib.so \
    $$APPLICATION_DESTDIR/libQTVLC.so \
	../../../3rdParty/src/vlc-1.0.x-BuildDir/Linux_runtime/libvlc.so \
    ../../../3rdParty/Common/log4qt/lib/release/liblog4qt-0.3.so.1

macx_libs = $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPlayerShell.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/libFakeLib.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libQTVLC.1.0.0.dylib

include("../../../libref.pri")
HEADERS += ../AutoTest.h \
    ../../ExternalSnapshotCreator/ArgumentParser.h \
    ../../ExternalSnapshotCreator/SnapshotCreationData.h \
    ../../ExternalSnapshotCreator/CommandProcessor.h \
    ../../ExternalSnapshotCreator/SnapshotEngine.h \
    ArgumentParserTest.h \
    CommandProcessorTest.h
SOURCES += ../main.cpp \
    ArgumentParserTest.cpp \
    ../../ExternalSnapshotCreator/ArgumentParser.cpp \
    ../../ExternalSnapshotCreator/SnapshotCreationData.cpp \
    ../../ExternalSnapshotCreator/CommandProcessor.cpp \
    ../../ExternalSnapshotCreator/SnapshotEngine.cpp \
    CommandProcessorTest.cpp
INCLUDEPATH += ../../Interfaces/ \
    ../../Container/ \
    ../FakeLib/ \
    ../../ToolLib/ \
    ../../ToolLib/Configuration \
    ../../ExternalSnapshotCreator/ \
    OTHER_FILES \
    +=
