include( "../../../globals.pri" )
include( "../../../unittest.pri" )
TEMPLATE = app
TARGET = MediaLibraryTest
CONFIG -= app_bundle
QT += testlib \
    xml \
    network \
    svg \
    sql
win32_libs = $$APPLICATION_DESTDIR/Interfaces.dll \
    $$APPLICATION_DESTDIR/Container.dll \
    $$APPLICATION_DESTDIR/PlayerShell.dll \
    $$APPLICATION_DESTDIR/EventAggregator.dll \
    $$APPLICATION_DESTDIR/Events.dll \
    $$APPLICATION_DESTDIR/QTVLC.dll \
    $$APPLICATION_DESTDIR/ToolLib.dll \
    ../../../3rdParty/Win32/taglib-1.6.1/bin/libtag.dll \
    $$APPLICATION_DESTDIR/Widgets.dll
unix_libs = $$APPLICATION_DESTDIR/libInterfaces.so \
    $$APPLICATION_DESTDIR/libContainer.so \
    $$APPLICATION_DESTDIR/libPlayerShell.so \
    $$APPLICATION_DESTDIR/libEventAggregator.so \
    $$APPLICATION_DESTDIR/libEvents.so \
    ../../../3rdParty/Common/log4qt/lib/release/liblog4qt-0.3.so.1 \
    $$APPLICATION_DESTDIR/libEvents.so \
    $$APPLICATION_DESTDIR/libQTVLC.so \
    $$APPLICATION_DESTDIR/libToolLib.so \
    ../../../3rdParty/Linux/taglib-1.6.1/lib/libtag.so.1.6.1 \
    ../../../3rdParty/src/vlc-1.1.x-BuildDir/Linux_runtime/libvlc.so \
    $$APPLICATION_DESTDIR/libWidgets.so
macx_libs = $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPlayerShell.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libQTVLC.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib \
    ../../../3rdParty/MacOSX/taglib-1.6.1/lib/libtag.1.6.1.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
    $$APPLICATION_DESTDIR/liblog4qt-0.3.1.dylib

contains( CONFIG, ppc ) {
macx_libs = $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPlayerShell.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libQTVLC.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib \
    ../../../3rdParty/MacOSX/ppc/taglib-1.6.1/lib/libtag.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib
}

include("../../../libref.pri")

# include("../../../macbinaries.pri")
HEADERS += ../AutoTest.h \
    StatisticClientCacheRepositporyTest.h \
    SystemMetaDataRepositoryTest.h \
    MediaLibraryModuleTest.h \
    ../../Modules/MediaLibrary/MediaLibraryModule.h \
    ../../Modules/MediaLibrary/MediaDB.h \
    ../../Modules/MediaLibrary/MediaType.h \
    ../../Modules/MediaLibrary/Tag.h \
    ../../Modules/MediaLibrary/TagRepository.h \
    ../../Modules/MediaLibrary/Media.h \
    ../../Modules/MediaLibrary/MediaRepository.h \
    ../../Modules/MediaLibrary/Snapshot.h \
    ../../Modules/MediaLibrary/SnapshotRepository.h \   
    ../../Modules/MediaLibrary/MediaMetadataHandler.h \
    ../../Modules/MediaLibrary/SearchEngine.h \
    ../../Modules/MediaLibrary/TagLibBasedMetaDataHandler.h \
    ../../Modules/MediaLibrary/FileTypeResolver.h \
    ../../Modules/MediaLibrary/News/News.h \
    ../../Modules/MediaLibrary/News/NewsImage.h \
    ../../Modules/MediaLibrary/News/NewsRepository.h \
    ../../Modules/MediaLibrary/MediaDBExport.h \
    ../../Modules/MediaLibrary/SystemMetaDataRepository.h \
    ../../Modules/MediaLibrary/StatisticClientCacheRepository.h \
    ../../Modules/MediaLibrary/StatisticEntry.h \
    ../../Modules/MediaLibrary/Content/ContentRepository.h \
    ../../Modules/MediaLibrary/Content/Content.h \
    MediaTest.h \
    NewsRepositoryTest.h \
    NewsTest.h \
    TageRepositoryTest.h \
    MediaRepositoryTest.h \
    SnapshotRepositoryTest.h \
    MediaDBExportTest.h \
    MediaDBTest.h

SOURCES += ../main.cpp \
    SystemMetaDataRepositoryTest.cpp \
    MediaLibraryModuleTest.cpp \
    ../../Modules/MediaLibrary/MediaLibraryModule.cpp \
    ../../Modules/MediaLibrary/MediaDB.cpp \
    ../../Modules/MediaLibrary/MediaType.cpp \
    ../../Modules/MediaLibrary/Tag.cpp \
    ../../Modules/MediaLibrary/TagRepository.cpp \
    ../../Modules/MediaLibrary/Media.cpp \
    ../../Modules/MediaLibrary/MediaRepository.cpp \
    ../../Modules/MediaLibrary/Snapshot.cpp \
    ../../Modules/MediaLibrary/SnapshotRepository.cpp \    
    ../../Modules/MediaLibrary/MediaMetadataHandler.cpp \
    ../../Modules/MediaLibrary/SearchEngine.cpp \
    ../../Modules/MediaLibrary/TagLibBasedMetaDataHandler.cpp \
    ../../Modules/MediaLibrary/FileTypeResolver.cpp \
    ../../Modules/MediaLibrary/News/News.cpp \
    ../../Modules/MediaLibrary/News/NewsImage.cpp \
    ../../Modules/MediaLibrary/News/NewsRepository.cpp \
    ../../Modules/MediaLibrary/MediaDBExport.cpp \
    ../../Modules/MediaLibrary/SystemMetaDataRepository.cpp \
    ../../Modules/MediaLibrary/StatisticClientCacheRepository.cpp \
    ../../Modules/MediaLibrary/StatisticEntry.cpp \
    ../../Modules/MediaLibrary/Content/ContentRepository.cpp \
    ../../Modules/MediaLibrary/Content/Content.cpp \
    MediaDBTest.cpp \
    TageRepositoryTest.cpp \
    MediaRepositoryTest.cpp \
    SnapshotRepositoryTest.cpp \
    NewsRepositoryTest.cpp \
    MediaDBExportTest.cpp \
    NewsTest.cpp \
    MediaTest.cpp \
    StatisticClientCacheRepositporyTest.cpp
INCLUDEPATH += ../../Modules/MediaLibrary/ \
    ../../Interfaces/ \
    ../../Container/ \
    ../../PlayerShell/ \
    ../../ToolLib/
macx:INCLUDEPATH += ../../../3rdParty/MacOSX/taglib-1.6.1/include/taglib
win32:INCLUDEPATH += ../../../3rdParty/Win32/taglib-1.6.1/include/taglib
unix:!macx:INCLUDEPATH += ../../../3rdParty/Linux/taglib-1.6.1/include/taglib
win32:image_src_path = $$PWD//*.png
win32:image_dst_path = $$DESTDIR//
win32:copy_media.commands += ..\..\..\copy_files.bat \
    $$replace(image_src_path,/,\) \
    $$replace(image_dst_path,/,\)
unix:copy_media.commands = cp \
    -vrf \
    $$PWD/*.png \
    $$DESTDIR//
POST_TARGETDEPS += copy_media
QMAKE_EXTRA_TARGETS += copy_media
OTHER_FILES += 
