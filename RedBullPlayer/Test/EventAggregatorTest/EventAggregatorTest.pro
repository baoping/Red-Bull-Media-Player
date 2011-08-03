include( "../../../globals.pri" )
include( "../../../unittest.pri" )
TEMPLATE = app
TARGET = EventAggregatorTest
CONFIG -= app_bundle
QT += testlib network \
    xml \
    svg \
    sql
win32_libs = 	$$APPLICATION_DESTDIR/EventAggregator.dll \
                $$APPLICATION_DESTDIR/Interfaces.dll \
                $$APPLICATION_DESTDIR/Events.dll \
                $$APPLICATION_DESTDIR/Container.dll

unix_libs = 	$$APPLICATION_DESTDIR/libEventAggregator.so \
                $$APPLICATION_DESTDIR/libInterfaces.so \
                $$APPLICATION_DESTDIR/libEvents.so \
                $$APPLICATION_DESTDIR/libContainer.so

macx_libs = 	$$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
                $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib  \
                $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
		$$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
		$$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
		$$APPLICATION_DESTDIR/liblog4qt-0.3.1.dylib \
		$$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib

include("../../../libref.pri")   
# include("../../../macbinaries.pri")
HEADERS += ../AutoTest.h \
    EventAggregatorTest.h \
    TestEventAggregator.h \
    EventSpy.h
SOURCES += ../main.cpp \
    EventAggregatorTest.cpp \
    EventSpy.cpp
INCLUDEPATH += ../../EventAggregator/
