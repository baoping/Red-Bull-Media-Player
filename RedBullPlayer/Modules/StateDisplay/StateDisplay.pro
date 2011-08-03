include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = StateDisplayModule
TEMPLATE = lib
CONFIG += plugin
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/QTVLC.dll \
    $$DESTDIR/ToolLib.dll \
    $$DESTDIR/Widgets.dll
unix_libs = $$DESTDIR/libInterfaces.so \
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
    $$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
HEADERS += StateDisplayModule.h \
    TaskModel.h \
    TaskPresenter.h \
    TaskViewFactory.h \
    StateViewFactory.h \
    HoverBoxUI.h \
    TaskViewUI.h \
    taskviewmodeleventbridge.h \
    taskpresentereventbridge.h \
    DiskSpaceModel.h \
    DiskSpacePresenter.h \
    StatePresenterEventBridge.h \
    InternetAvailabilityModel.h \
    InternetAvailabilityPresenter.h \
    InternetAvailabilityPresenterEventBridge.h
SOURCES += StateDisplayModule.cpp \
    TaskModel.cpp \
    TaskPresenter.cpp \
    TaskViewFactory.cpp \
    StateViewFactory.cpp \
    HoverBoxUI.cpp \
    TaskViewUI.cpp \
    taskviewmodeleventbridge.cpp \
    taskpresentereventbridge.cpp \
    DiskSpaceModel.cpp \
    DiskSpacePresenter.cpp \
    StatePresenterEventBridge.cpp \
    InternetAvailabilityModel.cpp \
    InternetAvailabilityPresenter.cpp \
    InternetAvailabilityPresenterEventBridge.cpp
unix:copy_media.commands = test \
    -d \
    "$$DESTDIR/media/$$TARGET" \
    || \
    mkdir \
    -p \
    $$DESTDIR/media/$$TARGET \
    && \
    cp \
    -vr \
    media/$$TARGET \
    $$DESTDIR/media
win32:image_src_path = $$PWD/media/$$TARGET//*
win32:image_dst_path = $$DESTDIR/media/$$TARGET//
win32:copy_media.commands += ..\..\..\copy_files.bat \
    $$replace(image_src_path,/,\) \
    $$replace(image_dst_path,/,\)
POST_TARGETDEPS += copy_media
QMAKE_EXTRA_TARGETS += copy_media
FORMS += HoverBoxUI.ui \
    TaskViewUI.ui
RESOURCES += StateDisplayResources.qrc
