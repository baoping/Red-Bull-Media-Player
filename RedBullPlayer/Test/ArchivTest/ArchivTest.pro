include( "../../../globals.pri" )
include( "../../../unittest.pri" )
TEMPLATE = app
TARGET = ArchivTest
CONFIG -= app_bundle
QT += testlib \
    network \
    xml \
    svg \
    sql
win32_libs = 
unix_libs = 
macx_libs = 
include("../../../libref.pri")
HEADERS += ../AutoTest.h \
    ../../../RedBullPlayer/Package/ArchivData.h \
    ../../../RedBullPlayer/Package/ArchivFile.h \
    ../../../RedBullPlayer/Package/ArchivBuilder.h \
    ../../../RedBullPlayer/Package/ArchivIndex.h \
    ../../../RedBullPlayer/Package/ArchivFileReference.h \
    ../../../RedBullPlayer/Package/ArchivDataReader.h \
    ../../../RedBullPlayer/Package/Package.h \
    PackageTest.h \
    ArchivIndexTest.h \
    ArchivFileReferenceTest.h \
    ArchivFileTest.h \
    ArchivDataTest.h \
    ArchivDataReaderTest.h \
    ArchivBuilderTest.h
SOURCES += ../main.cpp \
    ArchivFileTest.cpp \
    ../../../RedBullPlayer/Package/ArchivFile.cpp \
    ../../../RedBullPlayer/Package/ArchivBuilder.cpp \
    ../../../RedBullPlayer/Package/ArchivIndex.cpp \
    ../../../RedBullPlayer/Package/ArchivFileReference.cpp \
    ArchivIndexTest.cpp \
    ArchivFileReferenceTest.cpp \
    ../../../RedBullPlayer/Package/ArchivData.cpp \
    ArchivDataTest.cpp \
    ../../../RedBullPlayer/Package/ArchivDataReader.cpp \
    ArchivDataReaderTest.cpp \
    ArchivBuilderTest.cpp \
    ../../../RedBullPlayer/Package/Package.cpp \
    PackageTest.cpp
OTHER_FILES += data1.dat \
    data2.dat \
    data3.dat \
    data4.dat \
    data5.dat
win32:image_src_path = $$PWD//*.dat
win32:image_dst_path = $$DESTDIR//
win32:copy_testdata.commands += ..\..\..\copy_files.bat \
    $$replace(image_src_path,/,\) \
    $$replace(image_dst_path,/,\)
unix:copy_testdata.commands = cp \
    -vr \
    data1.dat \
    data2.dat \
    data3.dat \
    data4.dat \
    data5.dat \
    $$DESTDIR
POST_TARGETDEPS += copy_testdata
QMAKE_EXTRA_TARGETS += copy_testdata
