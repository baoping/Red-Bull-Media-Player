include( "../../../globals.pri" )
include( "../../../unittest.pri" )
TEMPLATE = app
TARGET = PlayerUpdateTest
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
    $$APPLICATION_DESTDIR/Package.dll \
    $$APPLICATION_DESTDIR/UpdateServices.dll \
    ../../../3rdParty/Win32/taglib-1.6.1/bin/libtag.dll
unix_libs = $$APPLICATION_DESTDIR/libInterfaces.so \
    $$APPLICATION_DESTDIR/libContainer.so \
    $$APPLICATION_DESTDIR/libPlayerShell.so \
    $$APPLICATION_DESTDIR/libEventAggregator.so \
    $$APPLICATION_DESTDIR/libEvents.so \
    $$APPLICATION_DESTDIR/libWidgets.so \
    $$APPLICATION_DESTDIR/libToolLib.so \
    $$APPLICATION_DESTDIR/libPackage.so \
    $$APPLICATION_DESTDIR/libUpdateServices.so \
    ../../../3rdParty/Common/log4qt/lib/release/liblog4qt-0.3.so.1 \
    ../../../3rdParty/Linux/taglib-1.6.1/lib/libtag.so.1.6.1 \
    ../../../3rdParty/src/vlc-1.1.x-BuildDir/Linux_runtime/libvlc.so
macx_libs = $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPlayerShell.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libUpdateServices.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPackage.1.0.0.dylib \
    ../../../3rdParty/MacOSX/taglib-1.6.1/lib/libtag.1.6.1.dylib  \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
    $$APPLICATION_DESTDIR/liblog4qt-0.3.1.dylib

contains( CONFIG, ppc ):macx_libs = $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPlayerShell.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libUpdateServices.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPackage.1.0.0.dylib \
    ../../../3rdParty/MacOSX/ppc/taglib-1.6.1/lib/libtag.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
    $$APPLICATION_DESTDIR/liblog4qt-0.3.1.dylib

include("../../../libref.pri")
HEADERS += ../AutoTest.h \
    ../../Modules/PlayerUpdateChecker/UpdateServiceUrlGenerator.h \
    ../../../Setup/Updater/ConfigFileTask.h \
    ../../../Setup/Updater/UpdateManager.h \
    ../../../Setup/Updater/UpdateProcess.h \
    ../../../Setup/Updater/UpdateTask.h \
    UpdateProcessTest.h \
    UpdateManagerTest.h \
    PlayerUpdateUrlGeneratorTest.h \
    VersionTest.h \
    PlayerVersionTest.h
SOURCES += ../main.cpp \
    ../../Modules/PlayerUpdateChecker/UpdateServiceUrlGenerator.cpp \
    ../../../Setup/Updater/ConfigFileTask.cpp \
    ../../../Setup/Updater/UpdateManager.cpp \
    ../../../Setup/Updater/UpdateProcess.cpp \
    PlayerUpdateUrlGeneratorTest.cpp \
    VersionTest.cpp \
    PlayerVersionTest.cpp \
    UpdateProcessTest.cpp \
    UpdateManagerTest.cpp
INCLUDEPATH += ../../Modules/PlayerUpdateChecker/ \
    ../../Interfaces/MediaLibrary/ \
    ../../Modules/MediaLibrary/ \
    ../../Events/ \
    ../../Interfaces/ \
    ../../Container/ \
    ../../PlayerShell/ \
    ../../ToolLib/ \
    ../../UpdateServices/ \
    ../../../Setup/Updater/ \
    ../../Package/
macx:INCLUDEPATH += ../../../3rdParty/MacOSX/taglib-1.6.1/include/taglib
win32:INCLUDEPATH += ../../../3rdParty/Win32/taglib-1.6.1/include/taglib
unix:!macx:INCLUDEPATH += ../../../3rdParty/Linux/taglib-1.6.1/include/taglib
OTHER_FILES += testVersion.nfo
win32:copy_config.commands = xcopy \
    /F \
    /Y \
    testVersion.nfo \
    "\"$$DESTDIR\""
unix:!macx:copy_config.commands = cp \
    -vrf \
    testVersion.nfo \
    $$DESTDIR/
macx:copy_config.commands = cp \
    -vrf \
    testVersion.nfo \
    $$DESTDIR/; cp \
    -vrf \
    testVersion.nfo \
    $$DESTDIR/version.nfo
POST_TARGETDEPS += copy_config
QMAKE_EXTRA_TARGETS += copy_config
