include( "../../globals.pri" )

# Fix Deps and linker flags for win32 cross compile
x-win32:include("../../x-win32-dll.pri")
TEMPLATE = lib
QT += network \
    xml \
    svg \
    sql
TARGET = Container
macx:DYLIBS += libContainer.1.dylib
HEADERS += Plugin.h \
    IError.h \
    Container.h \
    Error.h \
    Constants.h
SOURCES += Plugin.cpp \
    Container.cpp \
    Error.cpp \
    Constants.cpp
OTHER_FILES += plugins.xml
win32:copy_plugins_def.commands = xcopy \
    /F \
    /Y \
    plugins.xml \
    "\"$$DESTDIR\""
unix:!macx:!x-win32:copy_plugins_def.commands = cp \
    -vr \
    plugins.xml \
    $$DESTDIR
macx:!x-win32:copy_plugins_def.commands = cp \
    -vr \
    plugins.xml \
    $$DESTDIR
x-win32:copy_plugins_def.commands = cp \
    -vr \
    plugins.xml \
    $$DESTDIR

# include("../../macbinaries.pri")
POST_TARGETDEPS += copy_plugins_def
QMAKE_EXTRA_TARGETS += copy_plugins_def
