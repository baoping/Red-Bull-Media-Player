TEMPLATE = lib
TARGET = QTVLC
LIBS += /home/hubingch/devel/VLC-Win32/VLC/libvlc.dll \
	-lmingw32 

TARGET_EXT = .dll	
INCLUDEPATH += Interfaces/ \
    Factory/ \
    Exceptions 
	
OBJECTS_DIR = ../obj/
DESTDIR = ../../bin/
HEADERS += Interfaces/MediaPlayerInterface.h \
    Interfaces/MediaObjectInterface.h \
    Interfaces/MediaMetaDataInterface.h \
    VLCMediaPlayer.h \
    VLCMediaObject.h \
    Factory/MediaPlayerFactory.h \
    Interfaces/MediaPlayerState.h \
    Exceptions/MediaPlayerException.h \
    Exceptions/Exception.h \
    Exceptions/VLCException.h \
    VLCMediaMetaData.h \
    VLCEventBridge.h
SOURCES += VLCMediaPlayer.cpp \
    Interfaces/MediaPlayerInterface.cpp \
    VLCMediaObject.cpp \
    Interfaces/MediaObjectInterface.cpp \
    Factory/MediaPlayerFactory.cpp \
    Exceptions/MediaPlayerException.cpp \
    Exceptions/Exception.cpp \
    Exceptions/VLCException.cpp \
    VLCMediaMetaData.cpp \
    Interfaces/MediaMetaDataInterface.cpp \
    VLCEventBridge.cpp
OTHER_FILES += 
QMAKE_LFLAGS  = -mthreads -Wl,-enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -shared
