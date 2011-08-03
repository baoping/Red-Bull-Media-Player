QT -= gui
QT += xml
include( "../../globals.pri" )
INCLUDEPATH += ../../RedBullPlayer/Package
TARGET = QCompress
CONFIG += console \
    release
CONFIG -= app_bundle \
    debug
TEMPLATE = app
win32_libs =  $$DESTDIR/Package.dll
unix_libs = $$DESTDIR/libPackage.so
macx_libs = $$DESTDIR/libPackage.1.0.0.dylib
include("../../libref.pri")
SOURCES += main.cpp \
    DirectoryPacker.cpp
HEADERS += DirectoryPacker.h
DESTDIR = $$SETUP_TOOLS_DSESTDIR
message( $$DESTDIR )

