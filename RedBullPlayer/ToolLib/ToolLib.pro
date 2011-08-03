include( "../../globals.pri" )
QT += network \
    xml \
    xmlpatterns \
    svg \
    sql
unix:!macx:QT += dbus

TARGET = ToolLib
TEMPLATE = lib
DEFINES += PLAYERTOOLS_LIBRARY
HEADERS += UuidCreator.h \
    SleeperThread.h \
    ConfigManager.h \
    Network/DownloadUtil.h \
    SimpleLogger.h \
    RssFeed/RssFeedItem.h \
    RssFeed/RssFeed.h \
    RssFeed/RssFeedImageDescriptor.h \
    Network/NetworkAccessManager.h \
    helper.h \
    FileSystem/Win32VolumeFinder.h \
    FileSystem/USBStickUtil.h \
    FileSystem/LinuxVolumeFinder.h \
    FileSystem/FileSystemHelper.h \
    FileSystem/FileSystem.h \
    TaskManager/Task.h \
    TaskManager/FileTask.h \
    Configuration/ConfigurationProvider.h \
    Configuration/TVChannel.h \
    Configuration/TVConfiguration.h \
    FileSystem/Storage.h \
    FileSystem/StorageFactory.h \
    FileSystem/PathResolver.h \
    TaskManager/FileThread.h \
    Network/InternetStatusChecker.h
SOURCES += UuidCreator.cpp \
    SleeperThread.cpp \
    ConfigManager.cpp \
    Network/DownloadUtil.cpp \
    SimpleLogger.cpp \
    RssFeed/RssFeed.cpp \
    RssFeed/RssFeedImageDescriptor.cpp \
    RssFeed/RssFeedItem.cpp \
    Network/NetworkAccessManager.cpp \
    helper.cpp \
    FileSystem/Win32VolumeFinder.cpp \
    FileSystem/USBStickUtil.cpp \
    FileSystem/LinuxVolumeFinder.cpp \
    FileSystem/FileSystem.cpp \
    TaskManager/Task.cpp \
    TaskManager/FileTask.cpp \
    Configuration/ConfigurationProvider.cpp \
    Configuration/TVConfiguration.cpp \
    FileSystem/Storage.cpp \
    FileSystem/StorageFactory.cpp \
    FileSystem/PathResolver.cpp \
    TaskManager/FileThread.cpp
win32:SOURCES += FileSystem/Win32FileSystem.cpp \
    Network/Win32InternetStatusChecker.cpp
unix:!macx:SOURCES += FileSystem/LinuxFileSystem.cpp \
    Network/LinuxInternetStatusChecker.cpp

macx:SOURCES += FileSystem/MacFileSystem.cpp \
    Network/MacOSXInternetStatusChecker.cpp




OTHER_FILES += log4qt.properties \
    log4qt.properties.macx

LOG4QT_LIBDIR = ../../3rdParty/Common/log4qt/lib/release
contains( CONFIG, ppc ) {
    LOG4QT_LIBDIR = ../../3rdParty/MacOSX/ppc/log4qt/lib/release
}

win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$LOG4QT_LIBDIR/log4qt-0.3.dll \
    -lWininet

unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.so.1.0.0

macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.1.0.0.dylib




include("../../libref.pri")
INCLUDEPATH += ../../3rdParty/Common/log4qt/src/
unix:!macx:log4qt_files = log4qt.properties \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.so.1.0.0 \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.so.1.0 \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.so.1 \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.so
macx:log4qt_files = log4qt.properties.macx \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.1.0.0.dylib \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.1.0.dylib \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.1.dylib \
    $$LOG4QT_LIBDIR/liblog4qt-0.3.dylib
unix:copy_log4qt.commands = cp \
    -vr \
    $$log4qt_files \
    $$DESTDIR
macx:rename_log4qtprop.commands = mv \
    $$DESTDIR/log4qt.properties.macx \
    $$DESTDIR/log4qt.properties
win32:copy_log4qt.commands = xcopy \
    /F \
    /Y \
    "\"$$LOG4QT_LIBDIR/log4qt-0.3.dll\"" \
    "\"$$DESTDIR\""
win32:copy_log4qtprop.commands = xcopy \
    /F \
    /Y \
    log4qt.properties \
    "\"$$DESTDIR\""

POST_TARGETDEPS += copy_log4qt
macx:POST_TARGETDEPS += rename_log4qtprop
win32:POST_TARGETDEPS += copy_log4qtprop
QMAKE_EXTRA_TARGETS += copy_log4qt
macx:QMAKE_EXTRA_TARGETS += rename_log4qtprop
win32:QMAKE_EXTRA_TARGETS += copy_log4qtprop
