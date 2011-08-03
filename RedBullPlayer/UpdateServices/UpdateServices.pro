include( "../../globals.pri" )
QT += network \
    xml \
    xmlpatterns \
    sql \
    svg
TARGET = UpdateServices
TEMPLATE = lib
DEFINES += UPDATESERVICES_LIBRARY
SOURCES += VersionServiceClient.cpp \
    PlayerVersion.cpp \
    Version.cpp
HEADERS += VersionServiceClient.h \
    PlayerVersion.h \
    Version.h
win32_libs = $$DESTDIR/ToolLib.dll
unix_libs = $$DESTDIR/libToolLib.so
macx_libs = $$DESTDIR/libToolLib.1.0.0.dylib \
	    $$DESTDIR/libInterfaces.1.0.0.dylib \
	    $$DESTDIR/libContainer.1.0.0.dylib \
	    $$DESTDIR/liblog4qt-0.3.1.dylib

include("../../libref.pri")
OTHER_FILES += 
win32:copy_version.commands = xcopy \
    /F \
    /Y \
    ..\..\version.nfo \
    "\"$$DESTDIR\""
unix:copy_version.commands = cp \
    -vr \
    ../../version.nfo \
    $$DESTDIR

POST_TARGETDEPS += copy_version
QMAKE_EXTRA_TARGETS += copy_version
