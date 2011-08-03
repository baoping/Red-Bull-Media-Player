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
#include "PlayerController.h"

#include <QDebug>
#include <QDesktopServices>
#include <QApplication>

#include "../../Container/Constants.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/FileSystem/Storage.h"
#include "../../ToolLib/FileSystem/StorageFactory.h"
#include "../../Widgets/RedBullDialog.h"
#include "../../UpdateServices/PlayerVersion.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Updater;

PlayerController::PlayerController( RedBullPlayer::PlayerShell::Shell* shell,
                                    RedBullPlayer::PlayerShell::Region* region,
                                    PlayerModel* model,
                                    IPlayerView* view,
                                    QObject *parent ) :
    QObject( parent ) {
    _shell = shell;
    _region = region;
    _model = model;
    connect( _model, SIGNAL( mediaChanged() ), this, SLOT( mediaChanged() ) );
    _view = view;
    connect( _view, SIGNAL( mouseMovedOnPlayerView() ), this, SLOT( mouseMovedOnPlayerView() ) );
    _mediaPlayerHandler = 0;
    _stateMachine = 0;
    _snapshotHelper = 0;

    /* !! HardCoded Initial State of CentralFocus (its Hardcoded in the other Modules as Well) */
    _currentCentralWidgetType = CentralFocusChangedEvent::COVERVIEW_PLAYLIST;
    _previousCentralWidgetType = CentralFocusChangedEvent::UNKOWN;
    _controllBarEventBridge = createControllBarEventBridge( true );
    _playListEventBridge = createPlayListEventBridge( true );
    _playerEventBridge = createPlayerEventBridge( true );
    _seekBarEventBridge = createSeekBarEventBridge( true );
    _chapterSelectEventBridge = createChaperSelectEventBridge( true );
    _statisticEventBridge = createStatisticEventBridge( true );
    _audioAnimationEngine = createAudioAnimationEngine( _view );
    _mediaPlayerHandler = createMediaPlayerHandler( true );
    _mediaPlayerHandler->setVout( _view->videoPage() );
    _stateMachine = createStateMachine( _model, _mediaPlayerHandler, _view, _audioAnimationEngine );
    _snapshotHelper = createSnapshotHelper( _model, _mediaPlayerHandler );
    _keyDelayTimer = new QTimer( this );
    _keyDelayTimer->setSingleShot( true );
    _keyDelayTimer->setInterval( 5 );
    connect( _keyDelayTimer, SIGNAL( timeout() ), this, SLOT( onKeyDelayTimer() ) );
    _loopPlaylistTimer = new QTimer( this );
    _loopPlaylistTimer->setSingleShot( true );
    connect( _loopPlaylistTimer, SIGNAL( timeout() ), this, SLOT( onLoopPlaylistTimer() ) );
    _isFullScreen = false;
    _wasFullScreen = false;
    _isClosed = true;
    _hasMainFocus = false;
    _tvBuffering = false;
    _isInitializing = false;
    _setTimeOnNextTick = false;
    _aboutToClose = false;
    _timeForNextTick = 0;
    _keyDelayState = NONE;
    _overrideFunctionSelectedEvent = false;
    _postCloseCallbackObject = 0;
    _postCloseCallbackSlot = 0;
}

PlayerController::~PlayerController() {
    _keyDelayTimer->stop();
    delete _keyDelayTimer;
    delete _controllBarEventBridge;
    delete _playListEventBridge;
    delete _playerEventBridge;
    delete _seekBarEventBridge;
    delete _chapterSelectEventBridge;
    delete _audioAnimationEngine;
    delete _statisticEventBridge;
    disconnect( _model, SIGNAL( mediaChanged() ), this, SLOT( mediaChanged() ) );
    _model = 0;

    if ( _snapshotHelper != 0 ) {
        delete _snapshotHelper;
    }

    if ( _stateMachine != 0 ) {
        delete _stateMachine;
    }

    if ( _mediaPlayerHandler != 0 ) {
        delete _mediaPlayerHandler;
    }
}

ControllBarEventBridge* PlayerController::createControllBarEventBridge( bool connectSignals ) {
    ControllBarEventBridge* controllBarEventBridge = new ControllBarEventBridge( this );

    if ( connectSignals ) {
        connect( controllBarEventBridge, SIGNAL( pauseClicked() ), this, SLOT( pauseClicked() ) );
        connect( controllBarEventBridge, SIGNAL( playClicked() ), this, SLOT( playClicked() ) );
        connect( controllBarEventBridge, SIGNAL( snapShotClicked() ), this, SLOT( snapShotClicked() ) );
        connect( controllBarEventBridge, SIGNAL( fullScreenClicked() ), this, SLOT( fullScreenClicked() ) );
        connect( controllBarEventBridge, SIGNAL( muteClicked( bool ) ), this, SLOT( muteClicked( bool ) ) );
        connect( controllBarEventBridge, SIGNAL( forwardClicked() ), this, SLOT( forwardClicked() ) );
        connect( controllBarEventBridge, SIGNAL( backwardClicked() ), this, SLOT( backwardClicked() ) );
        connect( controllBarEventBridge, SIGNAL( resolutionClicked() ), this, SLOT( resolutionClicked() ) );
    }

    return controllBarEventBridge;
}

PlayListEventBridge* PlayerController::createPlayListEventBridge( bool connectSignals ) {
    PlayListEventBridge* playListEventBridge = new PlayListEventBridge( this );

    if ( connectSignals ) {
        connect( playListEventBridge,
                 SIGNAL( playListItemSelected( RedBullPlayer::Modules::MediaLibrary::IMedia*, bool, bool, bool ) ),
                 this,
                 SLOT( playListItemSelected( RedBullPlayer::Modules::MediaLibrary::IMedia*, bool, bool, bool ) ) );
    }

    return playListEventBridge;
}

MediaPlayerHandler* PlayerController::createMediaPlayerHandler( bool connectSignals ) {
    MediaPlayerHandler* mediaPlayerHandler = new MediaPlayerHandler( this );

    if ( connectSignals ) {
        connect( mediaPlayerHandler, SIGNAL( aspectRatioChanged( float ) ), this, SLOT( aspectRatioChanged( float ) ) );
        connect( mediaPlayerHandler, SIGNAL( backwardSpeedChanged( float ) ), this, SLOT( backwardSpeedChanged( float ) ) );
        connect( mediaPlayerHandler, SIGNAL( forwardSpeedChanged( float ) ), this, SLOT( forwardSpeedChanged( float ) ) );
        connect( mediaPlayerHandler,
                 SIGNAL( playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState* ) ),
                 this,
                 SLOT( playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState* ) ) );
        connect( mediaPlayerHandler, SIGNAL( playerTimeChanged( qint64 ) ), this, SLOT( playerTimeChanged( qint64 ) ) );
        connect( mediaPlayerHandler, SIGNAL( playerPositionChanged( float ) ), this, SLOT( playerPositionChanged( float ) ) );
    }

    return mediaPlayerHandler;
}

PlayerEventBridge* PlayerController::createPlayerEventBridge( bool connectSignals ) {
    PlayerEventBridge* playerEventBridge = new PlayerEventBridge( this );

    if ( connectSignals ) {
        connect( playerEventBridge,
                 SIGNAL( centralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType ) ),
                 this,
                 SLOT( centralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType ) ) );
        connect( playerEventBridge,
                 SIGNAL( playerVolumeChanged( int ) ),
                 this,
                 SLOT( playerVolumeChanged( int ) ) );
        connect( playerEventBridge,
                 SIGNAL( playerCloseButtonClicked() ),
                 this,
                 SLOT( playerCloseButtonClicked() ) );
        connect( playerEventBridge,
                 SIGNAL( playerExitClicked() ),
                 this,
                 SLOT( playerExitClicked() ) );
        connect( playerEventBridge,
                 SIGNAL( keyPressed( QKeyEvent* ) ),
                 this,
                 SLOT( keyPressed( QKeyEvent* ) ) );
        connect( playerEventBridge,
                 SIGNAL( mainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion ) ),
                 this,
                 SLOT( mainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion ) ) );
        connect( playerEventBridge,
                 SIGNAL( functionSelected( QString ) ),
                 this,
                 SLOT( functionSelected( QString ) ) );
    }

    return playerEventBridge;
}

SeekBarEventBridge* PlayerController::createSeekBarEventBridge( bool connectSignals ) {
    SeekBarEventBridge* seekBarEventBridge = new SeekBarEventBridge( this );

    if ( connectSignals ) {
        connect( seekBarEventBridge,
                 SIGNAL( seekBarPlayerHeadChanged( float ) ),
                 this,
                 SLOT( seekBarPlayerHeadChanged( float ) ) );
    }

    return seekBarEventBridge;
}

ChapterSelectEventBridge* PlayerController::createChaperSelectEventBridge( bool connectSignals ) {
    ChapterSelectEventBridge* chapterSelectEventBridge = new ChapterSelectEventBridge( this );

    if ( connectSignals ) {
        connect( chapterSelectEventBridge,
                 SIGNAL( chapterSelected( int ) ),
                 this,
                 SLOT( chapterSelected( int ) ) );
    }

    return chapterSelectEventBridge;
}

PlayerStateMachine* PlayerController::createStateMachine( PlayerModel* model,
        MediaPlayerHandler* playerHandler,
        IPlayerView* view,
        AudioAnimationEngine* engine ) {
    return new PlayerStateMachine( model, playerHandler, view, engine, this );
}

AudioAnimationEngine* PlayerController::createAudioAnimationEngine( IPlayerView* view ) {
    return new AudioAnimationEngine( view->audioPage(), this );
}

SnapshotHelper* PlayerController::createSnapshotHelper( PlayerModel* model, MediaPlayerHandler* playerHandler ) {
    return new SnapshotHelper( model, playerHandler );
}

StatisticEventBridge* PlayerController::createStatisticEventBridge( bool connectSignals ) {
    StatisticEventBridge* bridge = new StatisticEventBridge( this );

    if ( connectSignals ) {
    }

    return bridge;
}

void PlayerController::mediaChanged() {
    switch( _model->mediaType() ) {
        case PlayerModel::VIDEO:
            _view->showVideoPage();
            break;

        case PlayerModel::AUDIO:
            _view->showAudioPage();
            break;

        case PlayerModel::IMAGE:
            _view->showImagePage();
            break;

        case PlayerModel::STREAM:
            _view->showImagePage();  //for Buffering
            break;

        case PlayerModel::DOCUMENT:
            _view->showDummyPage();
            break;

        case PlayerModel::UNKOWN:
            _view->showDummyPage();
            break;
    }
}

bool PlayerController::playMedia() {
    _isInitializing = true;
    _isClosed = false;
    _aboutToClose = false;
    _playerEventBridge->changeBufferingAnimationVisibility( false );
    setPlayerVisibility( _model->mediaType() != PlayerModel::UNKOWN );
    bool retVal = false;

    switch( _model->mediaType() ) {
        case PlayerModel::VIDEO:
            retVal = playVideo();
            break;

        case PlayerModel::AUDIO:
            retVal = playAudio();
            break;

        case PlayerModel::IMAGE:
            retVal = showImage();
            startLoopPlaylistTimer();
            break;

        case PlayerModel::STREAM:
            retVal = playStream();
            break;

        case PlayerModel::DOCUMENT:
            break;

        case PlayerModel::UNKOWN:
            break;
    }

    //Call PlayerTrackStarted for legacy support (RBS-705)
    _playerEventBridge->playerTrackStarted( _model->id(), _model->mediaTypeId() );

    if ( _model->isPodCast() && _model->isNew() ) {
        _model->setIsNew( false );
        _playListEventBridge->mediaItemContentChanged( _model->id() );
    }

    return retVal;
}

void PlayerController::startLoopPlaylistTimer() {
    if( ConfigManager::instance()->loopPlaylist() )
        _loopPlaylistTimer->start( 1000 * ConfigManager::instance()->staticContentLoopInterval() );
}

bool PlayerController::playVideo() {
    _mediaPlayerHandler->initMedia( _model->fileName(), _model->startTime() );
    _mediaPlayerHandler->playMedia();
    _mediaPlayerHandler->setAspectRatio( 0, 0 ); //to Default;
    _mediaPlayerHandler->setCropGeometry( 0, 0 ); //to Default
    _currentResolutionState = DEFAULT;
    return true;
}

bool PlayerController::playAudio() {
    _mediaPlayerHandler->initMedia( _model->fileName(), _model->startTime() );
    _mediaPlayerHandler->playMedia();
    return true;
}

bool PlayerController::showImage() {
    _view->setImage( _model->fileName() );
    _isInitializing = false;
    return true;
}

bool PlayerController::playStream() {
    if ( !_model->animationFile().isNull() && !_model->animationFile().isEmpty() ) {
        _view->setBufferingAnimation( _model->animationFile(), _model->animationFrameCount() );
    } else {
        _view->showVideoPage();
    }

    _playerEventBridge->changeBufferingAnimationVisibility( true );
    _mediaPlayerHandler->initMedia( _model->fileName(), _model->startTime() );
    _mediaPlayerHandler->playMedia();
    _mediaPlayerHandler->setAspectRatio( 0, 0 ); //to Default;
    _mediaPlayerHandler->setCropGeometry( 0, 0 ); //to Default
    _currentResolutionState = DEFAULT;
    return true;
}

void PlayerController::hidePlayer() {
    setPlayerVisibility( false );
    _isClosed = true;
    _isInitializing = false;
    _aboutToClose = false;
}

void PlayerController::setPostCloseCallback( QObject* callbackObject, const char* callbackSlot ) {
    _postCloseCallbackObject = callbackObject;
    _postCloseCallbackSlot = callbackSlot;
}

void PlayerController::clearPostCloseCallback() {
    _postCloseCallbackObject = 0;
    _postCloseCallbackSlot = 0;
}

bool PlayerController::executePostCloseCallBack() {
    if ( _postCloseCallbackObject && _postCloseCallbackSlot ) {
        QTimer::singleShot( 5, _postCloseCallbackObject, _postCloseCallbackSlot );
        clearPostCloseCallback();
        return true;
    } else {
        return false;
    }
}

void PlayerController::closePlayer() {
    qDebug() << Q_FUNC_INFO;
    _aboutToClose = true;

    if( _loopPlaylistTimer->isActive() )
        _loopPlaylistTimer->stop();

    /* Image */
    if ( _model->mediaType() == PlayerModel::IMAGE ) {
        _view->clearImage();

        if( !executePostCloseCallBack() ) {
            if ( _isFullScreen )
                toggleFullScreen();

            hidePlayer();
        }
    }
    /* Stream & still buffering */
    else if ( _model->mediaType() == PlayerModel::STREAM ) {
        if ( _view->currentWidget() == _view->imagePage() ) {
            _playerEventBridge->changeBufferingAnimationVisibility( false );
            _view->clearImage();
        }

        retainPointer( this, SLOT( stopMediaPlayerEngine() ) );
    }
    /* Video or Audio */
    else if ( ( _model->mediaType() == PlayerModel::AUDIO ) ||
              ( _model->mediaType() == PlayerModel::VIDEO ) ) {
        /* Save current time as next start time */
        qint64 time = 0;

        if ( _mediaPlayerHandler->state() != MediaPlayerState::ENDED ) {
            time = _mediaPlayerHandler->time();

            if ( _mediaPlayerHandler->hasError() ) {
                Error::raise( _mediaPlayerHandler->lastError() );
            }

            _model->saveStartTime( time );
        } else {
            _model->saveStartTime( 0 ); //reset to 0 if played to end
        }

        _playListEventBridge->mediaItemContentChanged( _model->id() ); //inform env about new starttime
        retainPointer( this, SLOT( stopMediaPlayerEngine() ) );
    }
}

void PlayerController::retainPointer( QObject* callbackObject, const char* callbackSlot ) {
    qDebug() << Q_FUNC_INFO;
    QPoint mousePos = QCursor::pos();
    mousePos.rx() -= 5; //Magic numer so mouse will be displayed >= 3 ;)
    mousePos.ry() -= 5;
    QCursor::setPos( mousePos );
    QApplication::processEvents();
    QTimer::singleShot( 20, callbackObject, callbackSlot );
}

void PlayerController::stopMediaPlayerEngine() {
    qDebug() << Q_FUNC_INFO;
    QPoint mousePos = QCursor::pos();
    mousePos.rx() += 5; //Magic numer so mouse will be displayed >= 3 ;)
    mousePos.ry() += 5;
    QCursor::setPos( mousePos );
    QApplication::processEvents();
    _mediaPlayerHandler->stopMedia();

    if( !executePostCloseCallBack() ) {
        if ( _isFullScreen )
            toggleFullScreen();

        hidePlayer();
    }
}

/***** CONTROLL BAR ****/

void PlayerController::pauseClicked() {
    _mediaPlayerHandler->pauseMedia();
}

void PlayerController::playClicked() {
    if ( _mediaPlayerHandler->state() == MediaPlayerState::ENDED ) {
        playMedia();
    } else {
        _mediaPlayerHandler->playMedia();
    }
}

void PlayerController::snapShotClicked() {
    if (  StorageFactory::defaultStorage()->freeBytes() < 5242880 ) {
        QString dialogText;
        QFile downloadFinishedFile( Constants::ROOT_UI_TEXT_DIR + Constants::DISKSPACE_FULL_TEXT );

        if ( downloadFinishedFile.open( QFile::ReadOnly ) ) {
            QByteArray fileArray = downloadFinishedFile.readAll();
            downloadFinishedFile.close();
            dialogText = QString( fileArray );
        } else {
            dialogText = "There is not enough disk space left on your volume. Please free up some space. Until that RedBull Media Player will not fully function.";
        }

        PlayerVersion::instance()->parseString( &dialogText );
        RedBullDialog* dialog = new RedBullDialog( dialogText,
                _styleSheet,
                RedBullDialog::Ok,
                0 );
        dialog->exec();
    } else {
        Error err;
        _snapshotHelper->takeSnapshot( &err );
        Error::raise( &err, true );
    }
}

void PlayerController::fullScreenClicked() {
    toggleFullScreen();
}

void PlayerController::muteClicked( bool muted ) {
    bool isMuted = _mediaPlayerHandler->mute();

    if ( isMuted != muted ) {
        _mediaPlayerHandler->setMute( muted );
    }

    if ( !muted ) {
        _mediaPlayerHandler->setVolume( _mediaPlayerHandler->volume() );
    }
}

void PlayerController::forwardClicked() {
    if( _model->mediaType() == PlayerModel::AUDIO ) {
        _mediaPlayerHandler->addTime( Constants::PLAYER_AUDIO_FORWARD_STEP );

        if ( _mediaPlayerHandler->hasError() )
            Error::raise( _mediaPlayerHandler->lastError() );

        _playerEventBridge->forwardSpeedChanged( 1 );
    } else {
        // reset backward speed
        _mediaPlayerHandler->setBackwardSpeed( 1, false );
        int currentSpeed = ( int )_mediaPlayerHandler->forwardSpeed();
        int index = 0;

        for( int i = 0; i < Constants::PLAYER_FORWARD_RATES_COUNT; i++ ) {
            if( Constants::PLAYER_FORWARD_RATES[i] == currentSpeed ) {
                index = i;
                break;
            }
        }

        index++;

        if( index >= Constants::PLAYER_FORWARD_RATES_COUNT ) {
            index -= Constants::PLAYER_FORWARD_RATES_COUNT;
        }

        _mediaPlayerHandler->setForwardSpeed( Constants::PLAYER_FORWARD_RATES[index], true );
        _playerEventBridge->forwardSpeedChanged( (int) Constants::PLAYER_FORWARD_RATES[index] );
    }
}

void PlayerController::backwardClicked() {
    if( _model->mediaType() == PlayerModel::AUDIO ) {
        _mediaPlayerHandler->addTime( Constants::PLAYER_AUDIO_BACKWARD_STEP );

        if ( _mediaPlayerHandler->hasError() )
            Error::raise( _mediaPlayerHandler->lastError() );

        _playerEventBridge->backwardSpeedChanged( 1 );
    } else {
        // reset forward speed
        _mediaPlayerHandler->setForwardSpeed( 1, false );
        int currentSpeed = ( int )_mediaPlayerHandler->backwardSpeed();
        int index = 0;

        for( int i = 0; i < Constants::PLAYER_BACKWARD_RATES_COUNT; i++ ) {
            if( Constants::PLAYER_BACKWARD_RATES[i] == currentSpeed ) {
                index = i;
                break;
            }
        }

        index++;

        if( index >= Constants::PLAYER_BACKWARD_RATES_COUNT ) {
            index -= Constants::PLAYER_BACKWARD_RATES_COUNT;
        }

        _mediaPlayerHandler->setBackwardSpeed( Constants::PLAYER_BACKWARD_RATES[index], true );
        _playerEventBridge->backwardSpeedChanged( (int) Constants::PLAYER_BACKWARD_RATES[index] );
    }
}

void PlayerController::resolutionClicked() {
    ResolutionState nextResolutionState;

    switch( _currentResolutionState ) {
        case DEFAULT:
            nextResolutionState = AR_16t9; //set to ZOOMED if setCropGeometry finally works right
            break;

        case ZOOMED:
            nextResolutionState = AR_16t9;
            break;

        case AR_16t9:
            nextResolutionState = AR_4t3;
            break;

        case AR_4t3:
            nextResolutionState = DEFAULT;
            break;

        default:
            nextResolutionState = DEFAULT;
            break;
    }

    switch( nextResolutionState ) {
        case DEFAULT:
            _mediaPlayerHandler->setAspectRatio( 0, 0 );
            _mediaPlayerHandler->showText( "Default", 10, 10, 5000 );
            break;

        case ZOOMED:
            _mediaPlayerHandler->setAspectRatio( 0, 0 );
            _mediaPlayerHandler->showText( "Zoom", 10, 10, 5000 );
            break;

        case AR_16t9:
            _mediaPlayerHandler->setAspectRatio( 16, 9 );
            _mediaPlayerHandler->showText( "16:9", 10, 10, 5000 );
            break;

        case AR_4t3:
            _mediaPlayerHandler->setAspectRatio( 4, 3 );
            _mediaPlayerHandler->showText( "4:3", 10, 10, 5000 );
            break;
    }

    _currentResolutionState = nextResolutionState;
}

/***** PLAYLIST *****/

void PlayerController::playListItemSelected( RedBullPlayer::Modules::MediaLibrary::IMedia* media,
        bool skipDocuments,
        bool fromSelectPreviousItemEvent,
        bool overrideFunctionSelectEvent ) {
    _overrideFunctionSelectedEvent = overrideFunctionSelectEvent;
    QTimer::singleShot( 50, this, SLOT( resetFunctionSelectOverride() ) );

    if( _loopPlaylistTimer->isActive() )
        _loopPlaylistTimer->stop();

    if ( ( media->mediaType().name() == "news" ) || ( media->mediaType().name() == "about" )
            || ( media->mediaType().name() == "settings" ) ) {
        return;
    }

    //set new media
    _model->setMedia( media );

    if ( _model->isDocument() ) {
        if( skipDocuments ) {
            if( fromSelectPreviousItemEvent )
                _playListEventBridge->selectPreviousItem();
            else
                _playListEventBridge->selectNextItem();
        } else {
            QDesktopServices::openUrl( QUrl::fromLocalFile( _model->fileName() ) ); //RBS-731 Document View

            if( media->isNew() ) {
                _model->setIsNew( false );
                _playerEventBridge->documentOpened( media->guid() );
            }
        }
    } else {
        //Check if PodCast. Send StatisticEntryEvent if so
        if ( _model->isPodCast() ) {
            _statisticEventBridge->createPodCastViewStatisticEntry( _model->podCastId() );
        }

        //play media
        playMedia();
    }
}

void PlayerController::resetFunctionSelectOverride() {
    _overrideFunctionSelectedEvent = false;
}

void PlayerController::playNext() {
    _playListEventBridge->selectNextItem();
}
void PlayerController::playPrevious() {
    _playListEventBridge->selectPreviousItem();
}

/***** MEDIAPLAYER ****/
void PlayerController::aspectRatioChanged( float newAspectRatio ) {
}

void PlayerController::forwardSpeedChanged( float newForwardSpeed ) {
    _playerEventBridge->forwardSpeedChanged( (int) newForwardSpeed );
}

void PlayerController::backwardSpeedChanged( float newBackwardSpeed ) {
    _playerEventBridge->backwardSpeedChanged( (int) newBackwardSpeed );
}

void PlayerController::playerStateChanged( MediaPlayerState* state ) {
    _playerEventBridge->playerStateChanged( state->state() );
    _stateMachine->changeStateTo( state->state() );

    switch( state->state() ) {
        case MediaPlayerState::OPENING:
            break;

        case MediaPlayerState::PLAYING:
            if ( _isInitializing ) {
                _isInitializing = false;
            }

            if ( _model->length() == 0 ) {
                updateLength();
            }

            if ( ( _tvBuffering ) &&
                    ( _model->mediaType() == PlayerModel::STREAM ) ) {
                _view->showVideoPage();
                _playerEventBridge->changeBufferingAnimationVisibility( false );
            }

            break;

        case MediaPlayerState::STOPPED:
            /*if (_aboutToClose) {
                hidePlayer();
            }*/
            break;

        case MediaPlayerState::ENDED:
            _tvBuffering = ( _model->mediaType() == PlayerModel::STREAM );

            if( !_tvBuffering && ConfigManager::instance()->loopPlaylist() )
                _playListEventBridge->selectNextItem();

            break;

        default:
            break;
    }
}

void PlayerController::playerTimeChanged( qint64 time ) {
    if ( _isClosed || _aboutToClose ) return;

    _playerEventBridge->playerTimeChanged( time );

    if ( _setTimeOnNextTick ) {
        _setTimeOnNextTick = false;
        setTime( _timeForNextTick );
    }
}

void PlayerController::playerPositionChanged( float p ) {
    if ( _isClosed || _aboutToClose ) return;

    _playerEventBridge->playerPositionChanged( p );
}

/**** FOCUS ****/
void PlayerController::centralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType centralWidgetType ) {
    if( !_overrideFunctionSelectedEvent || centralWidgetType == CentralFocusChangedEvent::VIDEOPLAYER ) //same hack/Problem as Function Select @TV Direct Startup
        _currentCentralWidgetType = centralWidgetType;
}

/***** VIEW *****/
void PlayerController::mouseMovedOnPlayerView() {
    _playerEventBridge->mouseMovedOnPlayerView();
}

/**** HELPER ****/
void PlayerController::setPlayerVisibility( bool visible ) {
    if( visible ) {
        _region->widget()->show();

        if( _currentCentralWidgetType != CentralFocusChangedEvent::VIDEOPLAYER )  {
            _previousCentralWidgetType = _currentCentralWidgetType; //save current focus for restoration
        }

        _playerEventBridge->changeCentralFocus( CentralFocusChangedEvent::VIDEOPLAYER );
    } else {
        _region->widget()->hide();
        _audioAnimationEngine->stopAnimations();
        _playerEventBridge->changeCentralFocus( _previousCentralWidgetType ); //restore last focus
    }

    _playerEventBridge->changePlayerVisibility( visible );
}

void PlayerController::setTime( qint64 time ) {
    _mediaPlayerHandler->setTime( time );

    if ( _mediaPlayerHandler->hasError() ) {
        Error::raise( _mediaPlayerHandler->lastError() );
    }

    //If Player is paused. vlc_player doesn't emit timeChanged so we have to handle this.
    if ( ( _mediaPlayerHandler->state() == MediaPlayerState::PAUSED ) ||
            ( _mediaPlayerHandler->state() == MediaPlayerState::STOPPED ) ) {
        _playerEventBridge->playerTimeChanged( time );
    }
}

void PlayerController::setTimeOnTextTick( qint64 time ) {
    if ( _mediaPlayerHandler->state() == MediaPlayerState::PLAYING ) {
        _setTimeOnNextTick = true;
        _timeForNextTick = time;
    } else {
        _setTimeOnNextTick = false;
        _timeForNextTick = 0;
        setTime( time );
    }
}

void PlayerController::toggleFullScreen() {
    if( !_isFullScreen ) {
        _shell->findRegion( "BottomRegion" )->widget()->hide();
        _shell->findRegion( "TopRegion" )->widget()->hide();
        _shell->findRegion( "SearchRegion" )->widget()->hide();
        _wasFullScreen = _shell->isFullScreen();
        _shell->showFullScreen();
    } else {
        _shell->findRegion( "BottomRegion" )->widget()->show();
        _shell->findRegion( "TopRegion" )->widget()->show();
        _shell->findRegion( "SearchRegion" )->widget()->show();

        if( !_wasFullScreen ) {
            _shell->showNormal();
        }
    }

    _isFullScreen = !_isFullScreen;
    _playerEventBridge->fullScreenChanged( _isFullScreen );
}

void PlayerController::updateLength() {
    qint64 newLength = _mediaPlayerHandler->length();
    IMedia* media = _model->updateLength( newLength ); //store as MS
    qDebug() << "PlayerController:Length=" << newLength;
    qDebug() << "PlayerController:MediaLength=" << media->length();
    _playListEventBridge->mediaItemContentChanged( _model->id() ); //RBS-513 Update der Länge eines abgespielten Films erfolgt erst mit Restart
    //ControllBar
    _controllBarEventBridge->updateControllBarModel( media );
    //SeekBar
    _seekBarEventBridge->updateSeekBarModel( media );
    //ChaperSelect
    _chapterSelectEventBridge->updateChapterSelectModel( media );
}

/***** USER *****/
void PlayerController::requestTimeChange( qint64 time ) {
    _mediaPlayerHandler->setTime( time );

    if ( _mediaPlayerHandler->hasError() ) {
        //raise error?
    }
}

void PlayerController::playerVolumeChanged( int volume ) {
    if ( _mediaPlayerHandler->mute() ) {
        _mediaPlayerHandler->setMute( false );
    }

    _mediaPlayerHandler->setVolume( volume );

    if ( _mediaPlayerHandler->hasError() )
        Error::raise( _mediaPlayerHandler->lastError() );
}

void PlayerController::functionSelected( QString functionTitle ) {
    if( _loopPlaylistTimer->isActive() )
        _loopPlaylistTimer->stop();

    if ( !_isClosed && !_overrideFunctionSelectedEvent ) {
        closePlayer();
    }
}



/***** CLOSE *****/
void PlayerController::playerCloseButtonClicked() {
    closePlayer();
}

void PlayerController::playerExitClicked() {
    if ( !_isClosed ) {
        closePlayer();
    }
}

/***** SEEKBAR *****/
void PlayerController::seekBarPlayerHeadChanged( float position ) {
    qint64 newTime = (qint64) ( position * _mediaPlayerHandler->length() );
    setTimeOnTextTick( newTime );
}

/***** KeyPressed *****/
void PlayerController::keyPressed( QKeyEvent* keyEvent ) {
    if ( _isInitializing ) {
        return;
    }

    if( _currentCentralWidgetType != CentralFocusChangedEvent::VIDEOPLAYER || !_hasMainFocus ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYER,
                                         QString( "key Pressed Canceld noFocus PREVcentralWidgetType: %1" )
                                         .arg( _previousCentralWidgetType ) );
        return;
    }

    _keyDelayState = PlayerController::NONE;

    if ( _keyDelayTimer->isActive() ) {
        _keyDelayTimer->stop();
    }

    if( keyEvent->key() == Qt::Key_Escape ) {
        if ( _isFullScreen ) {
            toggleFullScreen();
        } else {
            _keyDelayState = PlayerController::ESCAPE;
            _keyDelayTimer->start();
        }
    }

    if( keyEvent->key() == Qt::Key_Right ) {
        _keyDelayState = PlayerController::RIGHT;
        _keyDelayTimer->start();
    }

    if( keyEvent->key() == Qt::Key_Left ) {
        _keyDelayState = PlayerController::LEFT;
        _keyDelayTimer->start();
    }

    if ( keyEvent->key() == Qt::Key_Space ) {
        _keyDelayState = PlayerController::SPACE;
        _keyDelayTimer->start();
    }
}

/***** ChapterSelect *****/
void PlayerController::chapterSelected( int startTime ) {
    setTimeOnTextTick( startTime );
}

/***** FOCUS *****/
void PlayerController::mainUIRegionFocusChanged( MainUIRegionFocusChangedEvent::UIRegion uiRegion ) {
    _hasMainFocus = ( uiRegion == MainUIRegionFocusChangedEvent::CENTRAL );
}

/***** DELAYTIMER *****/
void PlayerController::onKeyDelayTimer() {
    switch( _keyDelayState ) {
        case PlayerController::NONE:
            break;

        case PlayerController::ESCAPE:
            if ( !_isClosed ) {
                closePlayer();
            }

            break;

        case PlayerController::LEFT:
            if ( !_isClosed ) {
                setPostCloseCallback( this, SLOT( playPrevious() ) );
                closePlayer();
            } else {
                playPrevious();
            }

            break;

        case PlayerController::RIGHT:
            if ( !_isClosed ) {
                setPostCloseCallback( this, SLOT( playNext() ) );
                closePlayer();
            } else {
                playNext();
            }

            break;

        case PlayerController::SPACE:
            if ( _model->mediaType() != PlayerModel::IMAGE ) {
                if ( _mediaPlayerHandler->isPlaying() ) {
                    _mediaPlayerHandler->pauseMedia();
                } else {
                    _mediaPlayerHandler->playMedia();
                }
            }

            break;
    }
}

/***** LOOPPLAYLIST TIMER *****/
void PlayerController::onLoopPlaylistTimer() {
    if ( !_isClosed ) {
        setPostCloseCallback( this, SLOT( playNext() ) );
        closePlayer();
    } else {
        playNext();
    }
}

