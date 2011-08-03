include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = PlayerModule
TEMPLATE = lib
win32:DEFINES += QT_EXPERIMENTAL_SOLUTION \
    QT_STATEMACHINE_SOLUTION \
    Q_ANIMATION_EXPORT=__declspec(dllimport)
unix:DEFINES += QT_EXPERIMENTAL_SOLUTION \
    QT_STATEMACHINE_SOLUTION \
    Q_ANIMATION_EXPORT=
CONFIG += plugin
INCLUDEPATH += ../../Widgets
SOURCES += PlayerModule.cpp \
    MediaPlayer/SnapshotHelper.cpp \
    View/AudioAnimationEngine.cpp \
    View/VideoStackedWidget.cpp \
    MediaPlayer/MediaPlayerHandler.cpp \
    StateMachine/PlayerStateMachine.cpp \
    States/PlayerOpening.cpp \
    States/PlayerPlaying.cpp \
    States/PlayerStopped.cpp \
    States/PlayerBuffering.cpp \
    States/PlayerPaused.cpp \
    States/PlayerEnded.cpp \
    States/PlayerError.cpp \
    PlayerModel.cpp \
    PlayerController.cpp \
    PlayerView.cpp \
    ControllBarEventBridge.cpp \
    PlayListEventBridge.cpp \
    PlayerEventBridge.cpp \
    SeekBarEventBridge.cpp \
    ChapterSelectEventBridge.cpp \
    StatisticEventBridge.cpp
HEADERS += PlayerModule.h \
    MediaPlayer/SnapshotHelper.h \
    View/AudioAnimationEngine.h \
    View/VideoStackedWidget.h \
    MediaPlayer/MediaPlayerHandler.h \
    StateMachine/PlayerStateMachine.h \
    States/PlayerOpening.h \
    StateMachine/IPlayerState.h \
    States/PlayerPlaying.h \
    States/PlayerStopped.h \
    States/PlayerBuffering.h \
    States/PlayerPaused.h \
    States/PlayerEnded.h \
    States/PlayerError.h \
    PlayerModel.h \
    PlayerController.h \
    View/IPlayerView.h \
    PlayerView.h \
    ControllBarEventBridge.h \
    PlayListEventBridge.h \
    PlayerEventBridge.h \
    SeekBarEventBridge.h \
    ChapterSelectEventBridge.h \
    StatisticEventBridge.h
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/Widgets.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/QTVLC.dll \
    $$DESTDIR/ToolLib.dll \
    $$DESTDIR/UpdateServices.dll \
    ../../../3rdParty/Win32/qtanimationframework-2.3/lib/QtSolutions_AnimationFramework-2.3.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libWidgets.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libQTVLC.so \
    $$DESTDIR/libToolLib.so \
    $$DESTDIR/libUpdateServices.so \
    ../../../3rdParty/Linux/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.so.1

macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libWidgets.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/libUpdateServices.1.0.0.dylib \
    ../../../3rdParty/MacOSX/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
contains( CONFIG, ppc ) {
    macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libWidgets.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libQTVLC.1.0.0.dylib \
    $$DESTDIR/libUpdateServices.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    ../../../3rdParty/MacOSX/ppc/qtanimationframework-2.3/lib/libQtSolutions_AnimationFramework-2.3.dylib
}

include("../../../libref.pri")
include("../../../macbinaries.pri")
unix:copy_media.commands = test \
    -d \
    "$$DESTDIR/media/$$TARGET/images" \
    || \
    mkdir \
    -p \
    $$DESTDIR/media/$$TARGET/images \
    && \
    cp \
    -vr \
    media/$$TARGET/images \
    $$DESTDIR/media/$$TARGET
win32:image_src_path = $$PWD/media/$$TARGET//*
win32:image_dst_path = $$DESTDIR/media/$$TARGET//
win32:copy_media.commands += ..\..\..\copy_files.bat \
    $$replace(image_src_path,/,\) \
    $$replace(image_dst_path,/,\)
POST_TARGETDEPS += copy_media
QMAKE_EXTRA_TARGETS += copy_media
FORMS += VideoStackedWidget.ui \
    PlayerView.ui
