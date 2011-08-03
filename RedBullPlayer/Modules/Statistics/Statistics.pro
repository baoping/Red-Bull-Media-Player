include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = StatisticsModule
TEMPLATE = lib
CONFIG += plugin
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/QTVLC.dll \
    $$DESTDIR/ToolLib.dll \
    $$DESTDIR/Widgets.dll \
    $$DESTDIR/UpdateServices.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libUpdateServices.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libQTVLC.so \
    $$DESTDIR/libToolLib.so \
    $$DESTDIR/libWidgets.so
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/libUpdateServices.1.0.0.dylib \
    $$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
HEADERS += StatisticsModule.h \
    SenderQueue.h \
    UrlGeneratorFactory.h \
    PlayerStartedUrlGenerator.h \
    PodCastViewStatisticUrlGenerator.h
SOURCES += StatisticsModule.cpp \
    SenderQueue.cpp \
    UrlGeneratorFactory.cpp \
    PlayerStartedUrlGenerator.cpp \
    PodCastViewStatisticUrlGenerator.cpp
