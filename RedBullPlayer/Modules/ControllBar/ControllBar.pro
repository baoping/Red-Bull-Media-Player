include( "../../../globals.pri" )

x-win32:include("../../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = ControllBarModule
TEMPLATE = lib
CONFIG += plugin
win32:DEFINES += QT_EXPERIMENTAL_SOLUTION \
    QT_STATEMACHINE_SOLUTION \
    Q_ANIMATION_EXPORT=__declspec(dllimport)
unix:DEFINES += QT_EXPERIMENTAL_SOLUTION \
    QT_STATEMACHINE_SOLUTION \
    Q_ANIMATION_EXPORT=
SOURCES += ControllBarModule.cpp \
    ControllBarUI.cpp \
    ControllBarModel.cpp \
    ControllBarController.cpp \
    ControllBarEventBridge.cpp
HEADERS += ControllBarModule.h \
    ControllBarUI.h \
    ControllBarModel.h \
    ControllBarController.h \
    ControllBarEventBridge.h
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/Widgets.dll \
    $$DESTDIR/ToolLib.dll \
    ../../../3rdParty/Win32/qtanimationframework-2.3/lib/QtSolutions_AnimationFramework-2.3.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libWidgets.so \
    $$DESTDIR/libToolLib.so \
    ../../../3rdParty/Linux/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.so.1
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libWidgets.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    ../../../3rdParty/MacOSX/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
FORMS += ControllBarUI.ui
unix:copy_media.commands = test \
    -d \
    "$$DESTDIR/media/$$TARGET/images" \
    || \
    mkdir \
    -p \
    $$DESTDIR/media/$$TARGET/images \
    && \
    cp \
    -vr \
    media/$$TARGET/images \
    $$DESTDIR/media/$$TARGET
win32:image_src_path = $$PWD/media/$$TARGET//*
win32:image_dst_path = $$DESTDIR/media/$$TARGET//
win32:copy_media.commands += ..\..\..\copy_files.bat \
    $$replace(image_src_path,/,\) \
    $$replace(image_dst_path,/,\)
unix:copy_animation_framework.commands = cp \
    -vr \
    ../../../3rdParty/Linux/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.so.1 \
    $$DESTDIR
macx:copy_animation_framework.commands = cp \
    -vr \
    ../../../3rdParty/MacOSX/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.1.0.0.dylib \
    $$DESTDIR
contains( CONFIG, ppc ):macx:copy_animation_framework.commands = cp \
    -vr \
    ../../../3rdParty/MacOSX/ppc/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.1.0.0.dylib \
    $$DESTDIR
POST_TARGETDEPS += copy_media \
    copy_animation_framework
QMAKE_EXTRA_TARGETS += copy_media \
    copy_animation_framework
RESOURCES += ControlBarResources.qrc
