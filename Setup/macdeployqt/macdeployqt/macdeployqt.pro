include( "../../../globals.pri" )

TEMPLATE = app
TARGET = macdeployqt
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp ../shared/shared.cpp
HEADERS += ../shared/shared.h
CONFIG += qt warn_on
CONFIG -= app_bundle

DESTDIR = $$SETUP_TOOLS_DSESTDIR

