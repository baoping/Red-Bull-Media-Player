/*
 *  Red Bull Media Player
 *  Copyright (C) 2011, Red Bull
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <QSize>
#include <QTimer>
#include <QKeyEvent>

//MVC
#include "PlayerModel.h"
#include "View/IPlayerView.h"

#include "View/AudioAnimationEngine.h"

//Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../PlayerShell/Shell.h"

//MediaPlayer
#include "MediaPlayer/MediaPlayerHandler.h"
#include "ControllBarEventBridge.h"
#include "PlayListEventBridge.h"
#include "PlayerEventBridge.h"
#include "SeekBarEventBridge.h"
#include "ChapterSelectEventBridge.h"
#include "StatisticEventBridge.h"

#include "StateMachine/PlayerStateMachine.h"

#include "MediaPlayer/SnapshotHelper.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {
            class PlayerController : public QObject {
                    Q_OBJECT
                public:
                    explicit PlayerController( RedBullPlayer::PlayerShell::Shell* shell,
                                               RedBullPlayer::PlayerShell::Region* region,
                                               PlayerModel* model,
                                               IPlayerView* view,
                                               QObject *parent = 0 );
                    ~PlayerController();

                    bool playMedia();

                    void setDialogStyleSheet( QString styleSheet ) {
                        _styleSheet = styleSheet;
                    }

                private: //data
                    PlayerModel* _model;
                    IPlayerView* _view;

                    MediaPlayerHandler* _mediaPlayerHandler;
                    QString _styleSheet;

                    //Events
                    ControllBarEventBridge* _controllBarEventBridge;
                    PlayListEventBridge* _playListEventBridge;
                    PlayerEventBridge* _playerEventBridge;
                    SeekBarEventBridge* _seekBarEventBridge;
                    ChapterSelectEventBridge* _chapterSelectEventBridge;
                    StatisticEventBridge* _statisticEventBridge;

                    //StateMachine
                    PlayerStateMachine* _stateMachine;

                    //Audio Animation
                    AudioAnimationEngine* _audioAnimationEngine;

                    //Snapshot Helper
                    SnapshotHelper* _snapshotHelper;

                    //Focus
                    RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType _currentCentralWidgetType;
                    RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType _previousCentralWidgetType;

                    //Shell
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    bool _isFullScreen;
                    bool _wasFullScreen;
                    bool _hasMainFocus;
                    bool _tvBuffering;
                    volatile bool _isClosed;
                    volatile bool _isInitializing;
                    volatile bool _setTimeOnNextTick;
                    volatile qint64 _timeForNextTick;
                    volatile bool _aboutToClose;
                    bool _overrideFunctionSelectedEvent;

                    enum ResolutionState {
                        DEFAULT = 0,
                        ZOOMED = 1,
                        AR_16t9 = 2,
                        AR_4t3 = 3
                    };

                    ResolutionState _currentResolutionState;

                    QTimer* _keyDelayTimer;

                    enum KeyDelayState {
                        NONE = 0,
                        ESCAPE = 1,
                        LEFT = 2,
                        RIGHT = 3,
                        SPACE = 4
                    };

                    KeyDelayState _keyDelayState;

                    QTimer *_loopPlaylistTimer;


                private: //functions

                    bool playVideo();
                    bool playAudio();
                    bool showImage();
                    bool playStream();

                    ControllBarEventBridge* createControllBarEventBridge( bool connectSignals = true );
                    PlayListEventBridge* createPlayListEventBridge( bool connectSignals = true );
                    PlayerEventBridge* createPlayerEventBridge( bool connectSignals = true );
                    SeekBarEventBridge* createSeekBarEventBridge( bool connectSignals = true );
                    ChapterSelectEventBridge* createChaperSelectEventBridge( bool connectSignals = true );
                    StatisticEventBridge* createStatisticEventBridge( bool connectSignals = true );

                    PlayerStateMachine* createStateMachine( PlayerModel* model,
                                                            MediaPlayerHandler* playerHandler,
                                                            IPlayerView* view,
                                                            AudioAnimationEngine* engine );

                    AudioAnimationEngine* createAudioAnimationEngine( IPlayerView* view );
                    MediaPlayerHandler* createMediaPlayerHandler( bool connectSignals = true );
                    SnapshotHelper* createSnapshotHelper( PlayerModel* model, MediaPlayerHandler* playerHandler );

                    void setPlayerVisibility( bool visible );
                    void toggleFullScreen();
                    void hidePlayer();
                    void setTimeOnTextTick( qint64 time );
                    void setTime( qint64 time );
                    void updateLength();
                    void startLoopPlaylistTimer();

                private slots:
                    //Model
                    void mediaChanged();
                    //ControllBar
                    void pauseClicked();
                    void playClicked();
                    void snapShotClicked();
                    void fullScreenClicked();
                    void muteClicked( bool muted );
                    void forwardClicked();
                    void backwardClicked();
                    void resolutionClicked();
                    //PlayList
                    void playListItemSelected( RedBullPlayer::Modules::MediaLibrary::IMedia* media,
                                               bool skipDocuments,
                                               bool fromSelectPreviousItemEvent,
                                               bool overrideFunctionSelectEvent );
                    //MediaPlayer
                    void aspectRatioChanged( float newAspectRatio );
                    void forwardSpeedChanged( float newForwardSpeed );
                    void backwardSpeedChanged( float newBackwardSpeed );
                    void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState* state );
                    void playerTimeChanged( qint64 time );
                    void playerPositionChanged( float p );
                    //Focus
                    void centralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType centralWidgetType );
                    //View
                    void mouseMovedOnPlayerView();

                    //User
                    void requestTimeChange( qint64 time );
                    void playerVolumeChanged( int volume );
                    void functionSelected( QString functionTitle );

                    //Close
                    void playerCloseButtonClicked();
                    void playerExitClicked();

                    //SeekBar
                    void seekBarPlayerHeadChanged( float position );

                    //KeyPressed
                    void keyPressed( QKeyEvent* keyEvent );

                    //ChapterSelect
                    void chapterSelected( int startTime );

                    //Focus
                    void mainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion uiRegion );

                    //CloseDelayTimer
                    void onKeyDelayTimer();

                    //ResetFunctionSelect override
                    void resetFunctionSelectOverride();

                    //Loop Playlist Timer
                    void onLoopPlaylistTimer();

                    // Close Player
                    void closePlayer();
                    void retainPointer( QObject* callbackObject, const char* callbackSlot );
                    void stopMediaPlayerEngine();

                    // Play next/previous from PLaylist
                    void playNext();
                    void playPrevious();


                private:
                    void setPostCloseCallback( QObject* callbackObject, const char* callbackSlot );
                    void clearPostCloseCallback();
                    bool executePostCloseCallBack();
                    QObject* _postCloseCallbackObject;
                    const char* _postCloseCallbackSlot;
            };
        }
    }
}

#endif // PLAYERCONTROLLER_H
