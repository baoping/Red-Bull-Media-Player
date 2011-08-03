include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = TopUIModule
TEMPLATE = lib
CONFIG += plugin
SOURCES += TopUIModule.cpp \
    TopUICreator.cpp \
    StateIcon/StateIconModel.cpp \
    StateIcon/DiskSpaceModel.cpp \
    StateIcon/StateIconPresenter.cpp \
    StateIcon/StateDisplayButtonFactory.cpp \
    StateIcon/StateIconController.cpp \
    StateIcon/DiskSpacePresenter.cpp \
    StateIcon/ContentManagementModel.cpp \
    StateIcon/ContentManagementPresenter.cpp \
    StateIcon/InternetAvailabilityModel.cpp \
    StateIcon/InternetAvailabilityPresenter.cpp
HEADERS += TopUIModule.h \
    TopUICreator.h \
    StateIcon/StateIconModel.h \
    StateIcon/DiskSpaceModel.h \
    StateIcon/StateIconPresenter.h \
    StateIcon/StateDisplayButtonFactory.h \
    StateIcon/StateIconController.h \
    StateIcon/DiskSpacePresenter.h \
    StateIcon/ContentManagementModel.h \
    StateIcon/ContentManagementPresenter.h \
    StateIcon/InternetAvailabilityModel.h \
    StateIcon/InternetAvailabilityPresenter.h
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/Widgets.dll \
    $$DESTDIR/ToolLib.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libWidgets.so \
    $$DESTDIR/libToolLib.so
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libWidgets.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
FORMS += TopUICreator.ui
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
POST_TARGETDEPS += copy_media
QMAKE_EXTRA_TARGETS += copy_media
