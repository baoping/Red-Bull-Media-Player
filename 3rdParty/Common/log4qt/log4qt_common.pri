# Common file to be included in *.pro files to build/link against log4qt
CONFIG += qt log4qt
LOG4QT_VERSION = 0.3
LOG4QT_LIBNAME = log4qt-$$LOG4QT_VERSION

!log4qt-buildlib {
LIBS += -L$$LOG4QT_LIBDIR -l$$LOG4QT_LIBNAME
INCLUDEPATH += $$PWD/src/
}
