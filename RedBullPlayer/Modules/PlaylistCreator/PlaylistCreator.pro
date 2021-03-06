include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = PlaylistCreatorModule
TEMPLATE = lib
CONFIG += plugin
SOURCES += PlaylistCreatorModule.cpp \
    DummyPlaylistItem.cpp \
    ../MediaLibrary/MediaType.cpp \
    ../MediaLibrary/Media.cpp \
    DummyChapter.cpp \
    DummyPlaylist.cpp \
    TVPlaylistCreator.cpp \
    NewsPlaylistCreator.cpp \
    DynamicDirectoryBasedPlaylistCreator.cpp \
    AboutPlaylistCreator.cpp \
    SearchDrivenPlaylistCreator.cpp \
    DynamicDirectoryBasedPlaylistCreatorThread.cpp \
    PlaylistItemEventHandler.cpp \
    SettingsPlaylistCreator.cpp \
    SettingsEventHandler.cpp \
    DynamicFileExtensionsPlaylistProcessor.cpp \
    DynamicFileExtensionsPlaylistProcessorThread.cpp \
    ScanedDirectoriesPlaylistProcessor.cpp \
    ScanedDirectoriesPlaylistProcessorThread.cpp
HEADERS += PlaylistCreatorModule.h \
    DummyPlaylistItem.h \
    ../MediaLibrary/MediaType.h \
    ../MediaLibrary/Media.h \
    DummyChapter.h \
    DummyPlaylist.h \
    TVPlaylistCreator.h \
    NewsPlaylistCreator.h \
    DynamicDirectoryBasedPlaylistCreator.h \
    AboutPlaylistCreator.h \
    SearchDrivenPlaylistCreator.h \
    DynamicDirectoryBasedPlaylistCreatorThread.h \
    PlaylistItemEventHandler.h \
    SettingsPlaylistCreator.h \
    SettingsEventHandler.h \
    DynamicFileExtensionsPlaylistProcessor.h \
    DynamicFileExtensionsPlaylistProcessorThread.h \
    ScanedDirectoriesPlaylistProcessor.h \
    ScanedDirectoriesPlaylistProcessorThread.h
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/UpdateServices.dll \
    $$DESTDIR/ToolLib.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libUpdateServices.so \
    $$DESTDIR/libToolLib.so
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libUpdateServices.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
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
