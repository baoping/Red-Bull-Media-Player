include("../../globals.pri")
TEMPLATE = lib
TARGET = QTVLC

# Fix Deps and linker flags for win32 cross compile
x-win32:include("../../x-win32-dll.pri")
INCLUDEPATH += . \
    Interfaces/ \
    Factory/ \
    Exceptions \
    ../../3rdParty/src/vlc-1.1/include/
HEADERS += Interfaces/IMediaPlayer.h \
    VLCMediaPlayer.h \
    Exceptions/MediaPlayerException.h \
    Exceptions/Exception.h \
    Exceptions/VLCException.h \
    Factory/MediaPlayerFactory.h \
    VLCEventBridge.h \
    VLCInstance.h \
    Interfaces/MediaPlayerPurpose.h \
    VLCMediaEventBridge.h
SOURCES += VLCMediaPlayer.cpp \
    Interfaces/IMediaPlayer.cpp \
    Exceptions/MediaPlayerException.cpp \
    Exceptions/Exception.cpp \
    Exceptions/VLCException.cpp \
    Factory/MediaPlayerFactory.cpp \
    VLCEventBridge.cpp \
    VLCInstance.cpp \
    VLCMediaEventBridge.cpp
OTHER_FILES += 
win32_libs = ../../3rdParty/Win32/VLC/libvlc.dll \
    -lmingw32 \
    $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/ToolLib.dll
unix_libs = ../../3rdParty/src/vlc-1.1.x-BuildDir/Linux_runtime/libvlc.so \
    $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libToolLib.so
macx_libs = ../../3rdParty/MacOSX/Frameworks/VLCKit.framework/Versions/A/lib/libvlc.5.dylib \
    ../../3rdParty/MacOSX/Frameworks/VLCKit.framework/VLCKit \
    $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/liblog4qt-0.3.1.dylib
macx:
 { 
    HEADERS += CocoaContainer.h
    OBJECTIVE_SOURCES += CocoaContainer.mm
}

contains( CONFIG, ppc ) {
    CONFIG += no_keywords

    macx_libs = ../../3rdParty/MacOSX/ppc/VLCKit.framework/Versions/A/lib/libvlc.5.dylib \
    ../../3rdParty/MacOSX/ppc/VLCKit.framework/VLCKit \
    $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib

    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk

}

include("../../libref.pri")
use_unix_libs:vlc_files = ../../3rdParty/src/vlc-1.1.x-BuildDir/Linux_runtime/*
use_unix_libs:vlc_copy.commands = cp \
    -vr \
    $$vlc_files \
    $$DESTDIR
POST_TARGETDEPS += vlc_copy
QMAKE_EXTRA_TARGETS += vlc_copy
