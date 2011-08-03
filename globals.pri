
CONFIG += warn_on
CONFIG -= debug
# CONFIG += ppc

QT += network \
    xml \
    svg \
    sql \
    xmlpatterns

CURR_CONFIG = release

# CONFIG += force_debug

contains( CONFIG, force_debug ) {
    CONFIG -= release
    CONFIG += debug
}

contains( CONFIG, debug ) {
        CURR_CONFIG = debug
}

contains( CONFIG, ppc ) {
    message( "Arch: ppc" )
}


unix:PLATTFORM = Linux
macx:PLATTFORM = MacOSX
win32:PLATTFORM = Win32
x-win32:PLATTFORM = x-Win32

macx:CONFIG += use_macx_libs
unix:!macx:CONFIG += use_unix_libs
win32:CONFIG += use_win32_libs

DESTDIR = $$PLATTFORM

message( "Config: $$CURR_CONFIG" )
message( "Plattform: $$PLATTFORM" )

OBJECTS_DIR = $$PWD/obj/$$TARGET/$$PLATTFORM/$$CURR_CONFIG
DESTDIR = $$PWD/bin/$$PLATTFORM/$$CURR_CONFIG

SETUP_TOOLS_DSESTDIR = $$PWD/bin/$$PLATTFORM/Setup/
# win32:QMAKE_CXXFLAGS += -GR

WIN32_QT_DLLS_DIR = $$PWD/3rdParty/Win32/Qt-4.5.2/
WIN32_VLC_DIR = $$PWD/3rdParty/Win32/VLC


unix:QMAKE_RCC = /usr/bin/rcc

UI_DIR = $$PWD/obj/$$TARGET/$$PLATTFORM/$$CURR_CONFIG/GeneratedFiles
RCC_DIR = $$PWD/obj/$$TARGET/$$PLATTFORM/$$CURR_CONFIG/GeneratedFiles
MOC_DIR = $$PWD/obj/$$TARGET/$$PLATTFORM/$$CURR_CONFIG/GeneratedFiles

GENERATED_FILES_DIR = $$UI_DIR

message( "OBJECTS_DIR: $$OBJECTS_DIR" )
message( "TARGET_DIR: $$DESTDIR" )




