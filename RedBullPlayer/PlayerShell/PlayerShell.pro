include( "../../globals.pri" )
x-win32:include("../../x-win32-dll.pri")
TEMPLATE = lib
QT += network \
    xml \
    svg \
    sql
TARGET = PlayerShell
SOURCES += Shell.cpp \
    UI/Region.cpp
HEADERS += Shell.h \
    UI/Region.h
FORMS += Shell.ui
win32_libs = $$DESTDIR/Container.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/Widgets.dll \
    $$DESTDIR/ToolLib.dll
unix_libs = $$DESTDIR/libContainer.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libWidgets.so \
    $$DESTDIR/libToolLib.so
macx_libs = $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libWidgets.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/liblog4qt-0.3.1.dylib
include("../../libref.pri")
