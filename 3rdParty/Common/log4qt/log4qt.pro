# Project file to build log4qt as dynamic library
TEMPLATE = lib
CONFIG += qt \
    dll \
    debug_and_release \
    build_all \
    log4qt-buildlib

CONFIG(debug, debug|release):BUILD_MODE = debug
else:BUILD_MODE = release

TMP_DIR = $$PWD/tmp/$$BUILD_MODE
OBJECTS_DIR = $$TMP_DIR/obj
MOC_DIR = $$TMP_DIR/moc
RCC_DIR = $$TMP_DIR/rcc
UI_DIR = ui

DESTDIR = $$PWD/lib/$$BUILD_MODE
LOG4QT_LIBDIR = $$DESTDIR

include(log4qt_common.pri)
TARGET = $$LOG4QT_LIBNAME

target.path = $$DESTDIR
INSTALLS += target

include(src/log4qt/log4qt.pri)
