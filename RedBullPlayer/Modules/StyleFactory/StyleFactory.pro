include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = StyleFactoryModule
TEMPLATE = lib
CONFIG += plugin
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/QTVLC.dll \
    $$DESTDIR/ToolLib.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libQTVLC.so \
    $$DESTDIR/libToolLib.so
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
HEADERS += StyleFactoryModule.h \
    TextStyle.h \
    styleparser.h
SOURCES += StyleFactoryModule.cpp \
    TextStyle.cpp \
    styleparser.cpp
OTHER_FILES += styles_Win.xml \
               styles_Macx.xml \
               styles_Linux.xml
win32:copy_styles.commands = xcopy \
    /F \
    /Y \
    styles_Win.xml \
    "\"$$DESTDIR\""
unix:!macx:copy_styles.commands = cp \
    -vr \
    styles_Linux.xml \
    $$DESTDIR

macx:copy_styles.commands = cp \
    -vr \
    styles_Macx.xml \
    $$DESTDIR

POST_TARGETDEPS += copy_styles
QMAKE_EXTRA_TARGETS += copy_styles
RESOURCES += 
