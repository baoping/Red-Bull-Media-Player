include( "../../../globals.pri" )
include( "../../../unittest.pri" )
TEMPLATE = app
TARGET = UpdateInstallerTest
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
    $$APPLICATION_DESTDIR/Package.dll \
    $$APPLICATION_DESTDIR/UpdateServices.dll \
    $$APPLICATION_DESTDIR/Widgets.dll
unix_libs = $$APPLICATION_DESTDIR/libInterfaces.so \
    $$APPLICATION_DESTDIR/libContainer.so \
    $$APPLICATION_DESTDIR/libPlayerShell.so \
    $$APPLICATION_DESTDIR/libEventAggregator.so \
    $$APPLICATION_DESTDIR/libEvents.so \
    $$APPLICATION_DESTDIR/libWidgets.so \
    $$APPLICATION_DESTDIR/libPackage.so \
    $$APPLICATION_DESTDIR/libToolLib.so \
    $$APPLICATION_DESTDIR/libUpdateServices.so \
    ../../../3rdParty/Common/log4qt/lib/release/liblog4qt-0.3.so.1
macx_libs = $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPlayerShell.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPackage.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libUpdateServices.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib
include("../../../libref.pri")
HEADERS += ../AutoTest.h \
SOURCES += ../main.cpp \
INCLUDEPATH += ../../Container/ \
    ../../PlayerShell/ \
    ../../Modules/UpdateInstaller/ \
    ../FakeLib/ \
    ../../UpdateServices/

