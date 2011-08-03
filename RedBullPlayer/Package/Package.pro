include( "../../globals.pri" )
QT += xml
TARGET = Package
TEMPLATE = lib
DEFINES += PACKAGE_LIBRARY
win32_libs = 
unix_libs = 
macx_libs = 
include("../../libref.pri")
	
HEADERS += ArchivData.h \
    ArchivFile.h \
    ArchivBuilder.h \
    ArchivIndex.h \
    ArchivFileReference.h \
    ArchivDataReader.h \
    Package.h 
SOURCES += ArchivData.cpp \
    ArchivFile.cpp \
    ArchivBuilder.cpp \
    ArchivIndex.cpp \
    ArchivFileReference.cpp \
    ArchivDataReader.cpp \
    Package.cpp 
