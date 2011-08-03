include( "../../globals.pri" )
x-win32:include("../../x-win32-dll.pri")
QT += network \
    xml \
    xmlpatterns \
    svg \
    sql
TARGET = EventAggregator
TEMPLATE = lib
DEFINES += EVENTAGGREGATOR_LIBRARY
SOURCES += Aggregator.cpp
HEADERS += Aggregator.h\
	EventAggregator_global.h

macx:DYLIBS += libEventAggregator.1.dylib

win32_libs = $$DESTDIR/Events.dll

unix_libs =  $$DESTDIR/libEvents.so

macx_libs =  $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/liblog4qt-0.3.1.dylib



# include("../../macbinaries.pri")

include("../../libref.pri")
