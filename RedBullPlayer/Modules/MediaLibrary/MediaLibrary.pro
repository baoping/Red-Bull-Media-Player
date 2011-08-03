include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
TEMPLATE = lib
QT += network \
    xml \
    svg \
    sql
TARGET = MediaLibraryModule
CONFIG += plugin
win32:INCLUDEPATH += ../../../3rdParty/Win32/taglib-1.6.1/include/taglib
macx:INCLUDEPATH += ../../../3rdParty/MacOSX/taglib-1.6.1/include/taglib
unix:!macx:INCLUDEPATH += ../../../3rdParty/Linux/taglib-1.6.1/include/taglib
HEADERS += MediaLibraryModule.h \
    MediaDB.h \
    MediaDB_sql.h \
    MediaType.h \
    Tag.h \
    TagRepository.h \
    Media.h \
    MediaRepository.h \
    Snapshot.h \
    SnapshotRepository.h \
    MediaMetadataHandler.h \
    SearchEngine.h \
    TagLibBasedMetaDataHandler.h \
    FileTypeResolver.h \
    News/News.h \
    News/NewsImage.h \
    News/NewsRepository.h \
    MediaDBExport.h \
    SystemMetaDataRepository.h \
    StatisticEntry.h \
    StatisticClientCacheRepository.h \
    Content/Content_sql.h \
    Content/Content.h \
    Content/ContentRepository.h
SOURCES += MediaLibraryModule.cpp \
    MediaDB.cpp \
    MediaType.cpp \
    Tag.cpp \
    TagRepository.cpp \
    Media.cpp \
    MediaRepository.cpp \
    Snapshot.cpp \
    SnapshotRepository.cpp \
    MediaMetadataHandler.cpp \
    SearchEngine.cpp \
    TagLibBasedMetaDataHandler.cpp \
    FileTypeResolver.cpp \
    News/News.cpp \
    News/NewsImage.cpp \
    News/NewsRepository.cpp \
    MediaDBExport.cpp \
    SystemMetaDataRepository.cpp \
    StatisticEntry.cpp \
    StatisticClientCacheRepository.cpp \
    Content/Content.cpp \
    Content/ContentRepository.cpp
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/QTVLC.dll \
    $$DESTDIR/ToolLib.dll \
    ../../../3rdParty/Win32/taglib-1.6.1/bin/libtag.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libQTVLC.so \
    $$DESTDIR/libToolLib.so \
    ../../../3rdParty/Linux/taglib-1.6.1/lib/libtag.so.1.6.1
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    ../../../3rdParty/MacOSX/taglib-1.6.1/lib/libtag.1.6.1.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib

contains( CONFIG, ppc ) {
    macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    ../../../3rdParty/MacOSX/ppc/taglib-1.6.1/lib/libtag.dylib
}
include("../../../libref.pri")
include("../../../macbinaries.pri")
OTHER_FILES += 
