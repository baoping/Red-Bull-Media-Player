include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
TEMPLATE = lib
QT += network \
    xml \
    svg \
    sql
TARGET = XmlNewsFeedModule
CONFIG += plugin
HEADERS += XmlNewsFeedModule.h \
    RssFeedParser.h \
    RssFeedParserQueue.h \
    RssFeedParserEventBridge.h \
    RssFeedToNewsConverter.h
SOURCES += XmlNewsFeedModule.cpp \
    RssFeedParser.cpp \
    RssFeedParserQueue.cpp \
    RssFeedParserEventBridge.cpp \
    RssFeedToNewsConverter.cpp
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/ToolLib.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libToolLib.so
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
win32:copy_news_Cache_File.commands = xcopy \
    /F \
    /Y \
    news.xml \
    "\"$$DESTDIR\""
unix:copy_news_Cache_File.commands = cp \
    -vr \
    news.xml \
    $$DESTDIR
POST_TARGETDEPS += copy_news_Cache_File
QMAKE_EXTRA_TARGETS += copy_news_Cache_File
