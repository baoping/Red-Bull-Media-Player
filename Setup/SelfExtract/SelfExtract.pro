QT += gui \
    svg \
    xml
TEMPLATE = app
TARGET = RedBullPlayer
CONFIG -= debug
DEFINES += SELFEXTRACT
# QTPLUGIN += qgif
CONFIG += release windows static
CONFIG -= console

#CONFIG += debug console static
#CONFIG -= windows release

win32:DESTDIR = ../InstallerStubs/Win32/
#win32:QMAKE_MOC = C:\Projects\rb\3rdParty\Win32\Qt-4.5.0-static\bin$${DIR_SEPARATOR}moc.exe
#win32:QMAKE_UIC = C:\Projects\rb\3rdParty\Win32\Qt-4.5.0-static\bin$${DIR_SEPARATOR}uic.exe
#win32:QMAKE_IDC = C:\Projects\rb\3rdParty\Win32\Qt-4.5.0-static\bin$${DIR_SEPARATOR}idc.exe
#win32:QMAKE_RCC = C:\Projects\rb\3rdParty\Win32\Qt-4.5.0-static\bin$${DIR_SEPARATOR}rcc.exe
#win32:QMAKE_INCDIR_QT = C:\Projects\rb\3rdParty\Win32\Qt-4.5.0-static\include
#win32:QMAKE_LIBDIR_QT = C:\Projects\rb\3rdParty\Win32\Qt-4.5.0-static\lib

#win32:LIBS += C:\Qt\qt4-win-static\plugins\imageformats\libqgif.a \
#    C:\Qt\qt4-win-static\plugins\imageformats\libqsvg.a \
#    C:\Qt\qt4-win-static\plugins\imageformats\libqmng.a

message($$QMAKE_MOC)

# win32:QMAKE_PLUGINSDIR = C:\Qt\qt4-win-static\plugins\
# unix:QMAKE_INCDIR_QT = /home/majerju/devel/qt-x11-opensource-src-4.5.3/include
# unix:QMAKE_LIBDIR_QT = /home/majerju/devel/qt-x11-opensource-src-4.5.3/lib
unix:DESTDIR = ../InstallerStubs/Linux/

INCLUDEPATH += ../../RedBullPlayer/Package/ \
	       ../../RedBullPlayer/ToolLib/FileSystem/ \
		../../RedBullPlayer/Widgets/


SOURCES += SelfExtract.cpp \
    WorkerThread.cpp \
    ../../RedBullPlayer/Player/splashscreen.cpp \
    ../../RedBullPlayer/Widgets/RedBullDialog.cpp \
    ../../RedBullPlayer/Widgets/CommandButton.cpp \
    ../../RedBullPlayer/Package/ArchivData.cpp \
    ../../RedBullPlayer/Package/ArchivFile.cpp \
    ../../RedBullPlayer/Package/ArchivBuilder.cpp \
    ../../RedBullPlayer/Package/ArchivIndex.cpp \
    ../../RedBullPlayer/Package/ArchivFileReference.cpp \
    ../../RedBullPlayer/Package/ArchivDataReader.cpp \
    ../../RedBullPlayer/Package/Package.cpp \
    ../../RedBullPlayer/Widgets/CircleProgress.cpp \
    ../../RedBullPlayer/ToolLib/FileSystem/FileSystem.cpp

win32:SOURCES += ../../RedBullPlayer/ToolLib/FileSystem/Win32FileSystem.cpp
unix:!macx:SOURCES += ../../RedBullPlayer/ToolLib/FileSystem/LinuxFileSystem.cpp
macx:SOURCES += ../../RedBullPlayer/ToolLib/FileSystem/MacFileSystem.cpp

HEADERS += SelfExtract.h \
    WorkerThread.h \
    ../../RedBullPlayer/Player/splashscreen.h \
    ../../RedBullPlayer/Widgets/RedBullDialog.h \
    ../../RedBullPlayer/Widgets/CommandButton.h \
    ../../RedBullPlayer/Package/ArchivData.h \
    ../../RedBullPlayer/Package/ArchivFile.h \
    ../../RedBullPlayer/Package/ArchivBuilder.h \
    ../../RedBullPlayer/Package/ArchivIndex.h \
    ../../RedBullPlayer/Package/ArchivFileReference.h \
    ../../RedBullPlayer/Package/ArchivDataReader.h \
    ../../RedBullPlayer/Package/Package.h \
    ../../RedBullPlayer/Widgets/CircleProgress.h \
    ../../RedBullPlayer/ToolLib/FileSystem/FileSystem.h

# LIBS += stdlib
static { # Everything below takes effect with CONFIG += static
    CONFIG += static
    DEFINES += STATIC # // Equivalent to "#define STATIC" in source code
    message("Static build.")
}
RESOURCES += ../../RedBullPlayer/Player/Player.qrc
FORMS += ../../RedBullPlayer/Player/splashscreen.ui \
	../../RedBullPlayer/Widgets/RedBullDialog.ui
win32:RC_FILE = ../../RedBullPlayer/Player/icon.rc

