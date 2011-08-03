include( "../../../globals.pri" )
include( "../../../unittest.pri" )
TEMPLATE = app
TARGET = ToolLibTest
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
    $$DESTDIR/FakeLib.dll
unix_libs = $$APPLICATION_DESTDIR/libInterfaces.so \
    $$APPLICATION_DESTDIR/libContainer.so \
    $$APPLICATION_DESTDIR/libPlayerShell.so \
    $$APPLICATION_DESTDIR/libEventAggregator.so \
    $$APPLICATION_DESTDIR/libEvents.so \
    $$APPLICATION_DESTDIR/libWidgets.so \
    $$APPLICATION_DESTDIR/libToolLib.so \
    $$DESTDIR/libFakeLib.so \
    ../../../3rdParty/Common/log4qt/lib/release/liblog4qt-0.3.so.1
macx_libs = $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPlayerShell.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/libFakeLib.1.0.0.dylib \
    $$APPLICATION_DESTDIR/liblog4qt-0.3.1.dylib \
    $$APPLICATION_DESTDIR/libQTVLC.1.0.0.dylib
include("../../../libref.pri")

# include("../../../macbinaries.pri")
HEADERS += ../AutoTest.h \
    PathResolverTest.h \
    filesystemtest.h \
    TVConfigurationTest.h \
    StorageTest.h \
    StorageFactoryTest.h

SOURCES += ../main.cpp \
    filesystemtest.cpp \
    TVConfigurationTest.cpp \
    StorageTest.cpp \
    StorageFactoryTest.cpp \
    PathResolverTest.cpp
INCLUDEPATH += ../../Interfaces/ \
    ../../Container/ \
    ../FakeLib/ \
    ../../ToolLib/ \
    ../../ToolLib/Configuration
OTHER_FILES += testConfig.ini
win32:copy_config.commands = xcopy \
    /F \
    /Y \
    testConfig.ini \
    "\"$$DESTDIR\"" & \
    xcopy \
    /F \
    /Y \
    "\"$$APPLICATION_DESTDIR/ToolLib.dll\"" \
    "\"$$DESTDIR\""
unix:!macx:copy_config.commands = cp \
    -vrf \
    testConfig.ini $$APPLICATION_DESTDIR/libToolLib.so \
    $$DESTDIR/
macx:copy_config.commands = cp \
    -vrf \
    testConfig.ini $$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/
POST_TARGETDEPS += copy_config
QMAKE_EXTRA_TARGETS += copy_config
