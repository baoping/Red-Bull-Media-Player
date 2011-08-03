include( "../../../globals.pri" )
QT += network \
    xml \
    svg \
    sql
TARGET = MediaWatcherModule
TEMPLATE = lib
CONFIG += plugin
HEADERS += MediaWatcherModule.h \
    MediaWatcher.h \
    MediaWatcherExclusionListHandler.h \
    MediaWatcherEventBridge.h
SOURCES += MediaWatcherModule.cpp \
    MediaWatcher.cpp \
    MediaWatcherExclusionListHandler.cpp \
    MediaWatcherEventBridge.cpp
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/UpdateServices.dll \
    $$DESTDIR/ToolLib.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libUpdateServices.so \
    $$DESTDIR/libToolLib.so
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libUpdateServices.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
