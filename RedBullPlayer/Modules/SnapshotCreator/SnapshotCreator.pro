include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = SnapshotCreatorModule
TEMPLATE = lib
CONFIG += plugin
HEADERS += SnapshotCreatorModule.h \
    SnapshotRequestHandler.h \
    RequestMetaData.h
SOURCES += SnapshotCreatorModule.cpp \
    SnapshotRequestHandler.cpp \
    RequestMetaData.cpp
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/ToolLib.dll \
    $$DESTDIR/QTVLC.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libToolLib.so \
    $$DESTDIR/libQTVLC.so
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
