include( "../../../globals.pri" )
include( "../../../unittest.pri" )
TEMPLATE = app
TARGET = TaskManagerTest
CONFIG -= app_bundle
QT += testlib \
    network \
    xml \
    svg \
    sql
win32_libs = $$APPLICATION_DESTDIR/Interfaces.dll \
    $$APPLICATION_DESTDIR/Container.dll \
    $$APPLICATION_DESTDIR/PlayerShell.dll \
    $$APPLICATION_DESTDIR/EventAggregator.dll \
    $$APPLICATION_DESTDIR/Events.dll \
    $$APPLICATION_DESTDIR/ToolLib.dll \
    $$APPLICATION_DESTDIR/Widgets.dll
unix_libs = $$APPLICATION_DESTDIR/libInterfaces.so \
    $$APPLICATION_DESTDIR/libContainer.so \
    $$APPLICATION_DESTDIR/libPlayerShell.so \
    $$APPLICATION_DESTDIR/libEventAggregator.so \
    $$APPLICATION_DESTDIR/libEvents.so \
    $$APPLICATION_DESTDIR/libWidgets.so \
    $$APPLICATION_DESTDIR/libToolLib.so \
    ../../../3rdParty/Common/log4qt/lib/release/liblog4qt-0.3.so.1
macx_libs = $$APPLICATION_DESTDIR/libInterfaces.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libContainer.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libPlayerShell.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEventAggregator.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libEvents.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libToolLib.1.0.0.dylib \
    $$APPLICATION_DESTDIR/libWidgets.1.0.0.dylib \
    $$APPLICATION_DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")

# include("../../../macbinaries.pri")
HEADERS += ../AutoTest.h \
    ../../Modules/TaskManager/taskrepository.h \
    ../../Modules/TaskManager/taskrepositoryeventbridge.h \
    ../../Modules/TaskManager/taskexecutor.h \
    ../../ToolLib/TaskManager/Task.h \
    ../../ToolLib/TaskManager/FileTask.h \
    ../../ToolLib/helper.h
SOURCES += ../main.cpp \
    ../../Modules/TaskManager/taskrepository.cpp \
    ../../Modules/TaskManager/taskrepositoryeventbridge.cpp \
    ../../Modules/TaskManager/taskexecutor.cpp \
    ../../ToolLib/TaskManager/Task.cpp \
    ../../ToolLib/TaskManager/FileTask.cpp \
    ../../ToolLib/helper.cpp
INCLUDEPATH += ../../Modules/TaskManager/ \
    ../../Container/Enumerations/ \
    ../../ToolLib/ \
    ../../ToolLib/TaskManager/ \
    ../../Interfaces/ \
    ../../Interfaces/TaskManager \
    ../../EventAggregator/ \
    ../../Events/
