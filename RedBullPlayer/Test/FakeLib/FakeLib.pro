include( "../../../globals.pri" )
include( "../../../unittest.pri" )
QT += network \
    xml \
    svg \
    sql
TARGET = FakeLib
TEMPLATE = lib
win32_libs = $$APPLICATION_DESTDIR/Interfaces.dll \
    $$APPLICATION_DESTDIR/Container.dll \
    $$APPLICATION_DESTDIR/PlayerShell.dll \
    $$APPLICATION_DESTDIR/EventAggregator.dll \
    $$APPLICATION_DESTDIR/Events.dll \
    $$APPLICATION_DESTDIR/QTVLC.dll \
    $$APPLICATION_DESTDIR/ToolLib.dll \
    ../../../3rdParty/Win32/taglib-1.6.1/bin/libtag.dll \
    $$APPLICATION_DESTDIR/Widgets.dll
unix_libs = $$APPLICATION_DESTDIR/libInterfaces.so \
    $$APPLICATION_DESTDIR/libContainer.so \
    $$APPLICATION_DESTDIR/libPlayerShell.so \
    $$APPLICATION_DESTDIR/libEventAggregator.so \
    $$APPLICATION_DESTDIR/libEvents.so \
    ../../../3rdParty/Common/log4qt/lib/release/liblog4qt-0.3.so.1 \
    $$APPLICATION_DESTDIR/libEvents.so \
    $$APPLICATION_DESTDIR/libQTVLC.so \
    $$APPLICATION_DESTDIR/libToolLib.so \
    ../../../3rdParty/Linux/taglib-1.6.1/lib/libtag.so.1.6.1 \
    $$APPLICATION_DESTDIR/libWidgets.so

macx_libs = $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPlayerShell.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libQTVLC.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib \
    ../../../3rdParty/MacOSX/taglib-1.6.1/lib/libtag.1.6.1.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
    $$APPLICATION_DESTDIR/liblog4qt-0.3.1.dylib


include("../../../libref.pri")
DEFINES += FAKELIB_LIBRARY
SOURCES += 
HEADERS += FileSystemFake.h \
    FakeLib_global.h
INCLUDEPATH += ../../Container/ \
    ../../PlayerShell/
