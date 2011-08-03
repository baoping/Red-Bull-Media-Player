include( "../../globals.pri" )
QT += network \
    xml \
    svg \
    sql
TARGET = Player
unix:!macx:TARGET = Player.com
TEMPLATE = app

#CONFIG += console
#CONFIG -= windows
CONFIG -= console
CONFIG += windows
SOURCES += main.cpp \
    splashscreen.cpp
HEADERS += splashscreen.h
FORMS += splashscreen.ui
OTHER_FILES += config.ini \
    point.png

LOG4QT_LIBDIR = ../../3rdParty/Common/log4qt/lib/release

win32_libs = $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/Widgets.dll \
    $$DESTDIR/ToolLib.dll \
    $$DESTDIR/Interfaces.dll \
    $$LOG4QT_LIBDIR/log4qt-0.3.dll
unix_libs = $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libWidgets.so \
    $$DESTDIR/libToolLib.so \
    $$DESTDIR/libInterfaces.so \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.so.1.0.0
macx_libs = $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib \
    $$DESTDIR/libWidgets.1.0.0.dylib \
    $$DESTDIR/libUpdateServices.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/libInterfaces.1.0.0.dylib \
    ../../3rdParty/MacOSX/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.1.0.0.dylib \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.1.0.0.dylib \
    ../../3rdParty/MacOSX/taglib-1.6.1/lib/libtag.1.6.1.dylib

contains( CONFIG, ppc ) {
LOG4QT_LIBDIR = ../../3rdParty/MacOSX/ppc/log4qt/lib/release
macx_libs = $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib \
    $$DESTDIR/libWidgets.1.0.0.dylib \
    $$DESTDIR/libUpdateServices.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/libInterfaces.1.0.0.dylib \
    ../../3rdParty/MacOSX/ppc/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.1.0.0.dylib \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.1.0.0.dylib \
    ../../3rdParty/MacOSX/ppc/taglib-1.6.1/lib/libtag.dylib
}

include("../../libref.pri")
win32:RC_FILE = icon.rc
ICON = icon.icns
win32:copy_config.commands = xcopy \
    /F \
    /Y \
    config.ini \
    "\"$$DESTDIR\""
unix:copy_config.commands = cp \
    -vr \
    config.ini \
    $$DESTDIR

macx:change_deps_path.commands = install_name_tool \
    -change \
    @loader_path/lib/libvlc.5.dylib \
    @loader_path/../Frameworks/VLCKit.framework/Versions/A/lib/libvlc.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib

macx:copy_frameworks_dir.commands = if [ ! -d ../../bin/MacOSX/Frameworks ]; then cp -vRf ../../3rdParty/MacOSX/Frameworks ../../bin/MacOSX; fi

contains( CONFIG, ppc ) {
macx:copy_frameworks_dir.commands = if [ ! -d ../../bin/MacOSX/Frameworks ]; then cp -vRf ../../3rdParty/MacOSX/ppc/VLCKit.Framework ../../bin/MacOSX/Framework; fi
}

POST_TARGETDEPS += \
    copy_frameworks_dir \
    copy_config \
    change_deps_path
QMAKE_EXTRA_TARGETS +=  \
    copy_frameworks_dir \
    copy_config \
    change_deps_path
RESOURCES += Player.qrc
OTHER_FILES += splash.png
