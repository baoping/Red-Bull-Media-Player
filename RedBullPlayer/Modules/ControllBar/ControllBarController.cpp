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
#include "ControllBarController.h"

#include <QDebug>
#include <QPainter>
#include <QBitmap>
#include <QPoint>
#include <QCursor>

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::ControllBar;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

ControllBarController::ControllBarController( RedBullPlayer::PlayerShell::Shell* shell,
        RedBullPlayer::PlayerShell::Region* region,
        ControllBarModel* model,
        ControllBarUI* view,
        QObject *parent ) :
    QObject( parent ) {
    _shell = shell;
    _region = region;
    _model = model;
    connect( _model, SIGNAL( mediaChanged() ), this, SLOT( onMediaChanged() ) );
    _view = view;
    connect( _view, SIGNAL( action() ), this, SLOT( onViewAction() ) );
    connect( _view, SIGNAL( backwardClicked() ), this, SLOT( onBackwardClicked() ) );
    connect( _view, SIGNAL( playClicked() ), this, SLOT( onPlayClicked() ) );
    connect( _view, SIGNAL( forwardClicked() ), this, SLOT( onForwardClicked() ) );
    connect( _view, SIGNAL( previewClicked() ), this, SLOT( onPreviewClicked() ) );
    connect( _view, SIGNAL( fullscreenClicked() ), this, SLOT( onFullScreenClicked() ) );
    connect( _view, SIGNAL( snapshotClicked() ), this, SLOT( onSnapshotClicked() ) );
    connect( _view, SIGNAL( muteClicked() ), this, SLOT( onMuteClicked() ) );
    connect( _view, SIGNAL( resolutionClicked() ), this, SLOT( onResolutionClicked() ) );
    _controllBarEventBridge = createControllBarEventBridge( true );
    _isPlaying = false; //helps to handle the dirty way play+pause button is handled...
    _tvBufferingTime = 0;
    _tvBufferingTimer = new QTimer( this );
    _tvBufferingTimer->setInterval( 1000 );
    connect( _tvBufferingTimer, SIGNAL( timeout() ), this, SLOT( onTvBufferingTimer() ) );
    _hideTimer = new QTimer( this );
    connect( _hideTimer, SIGNAL( timeout() ), this, SLOT( onHideTimer() ) );
    _isVisible = false;
    _isBuffering = false;
    _lastParentHeight = 0;
    _lastParentWitdh = 0;
    _skipDocuments = false;
    _isPlayerFullScreen = false;
    initRegion();
}

ControllBarController::~ControllBarController() {
    delete _controllBarEventBridge;
}

ControllBarEventBridge* ControllBarController::createControllBarEventBridge( bool connectSignals ) {
    ControllBarEventBridge* controllBarEventBridge = new ControllBarEventBridge( this );

    if ( connectSignals ) {
        connect( controllBarEventBridge,
                 SIGNAL( bufferingAnimationChangedVisibility( bool ) ),
                 this,
                 SLOT( bufferingAnimationChangedVisibility( bool ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( snapshotCaptureEnabled( bool ) ),
                 this,
                 SLOT( snapshotCaptureEnabled( bool ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( fullScreenStatusChanged( bool ) ),
                 this,
                 SLOT( fullScreenStatusChanged( bool ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( updateControllBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia*, bool ) ),
                 this,
                 SLOT( updateControllBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia*, bool ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( playerTimeChanged( qint64 ) ),
                 this,
                 SLOT( playerTimeChanged( qint64 ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum ) ),
                 this,
                 SLOT( playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( playerVisibilityChanged( bool ) ),
                 this,
                 SLOT( playerVisibilityChanged( bool ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( centralUIRegionResized( int, int ) ),
                 this,
                 SLOT( centralUIRegionResized( int, int ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( videoScreenMouseMoved() ),
                 this,
                 SLOT( videoScreenMouseMoved() ) );
        connect( controllBarEventBridge,
                 SIGNAL( shellWidgetMoved() ),
                 this,
                 SLOT( shellWidgetMoved() ) );
        connect( controllBarEventBridge,
                 SIGNAL( playerBackwardSpeedChanged( int ) ),
                 this,
                 SLOT( playerBackwardSpeedChanged( int ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( playerForwardSpeedChanged( int ) ),
                 this,
                 SLOT( playerForwardSpeedChanged( int ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( playerVolumeChanged( int ) ),
                 this,
                 SLOT( playerVolumneChanged( int ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( keyPressed( QKeyEvent* ) ),
                 this,
                 SLOT( keyPressed( QKeyEvent* ) ) );
        connect( controllBarEventBridge,
                 SIGNAL( playerExitClicked() ),
                 this,
                 SLOT( playerExitClicked() ) );
    }

    return controllBarEventBridge;
}

void ControllBarController::updateControllBarModel( IMedia* media, bool skipDocuments ) {
    _skipDocuments = skipDocuments;

    if( skipDocuments && _model->resolveMediaType( media ) == IMediaModel::DOCUMENT )
        return;

    _model->setMedia( media );
}

void ControllBarController::onMediaChanged() {
    _view->enableButtons( false );

    switch( _model->mediaType() ) {
        case ControllBarModel::VIDEO:
            _view->enableButtons( true );
            break;

        case ControllBarModel::AUDIO:
            _view->enableButtonsForAudio( true );
            break;

        case ControllBarModel::IMAGE:
            _view->enableButtonsForImage( true );
            _view->setTimeText( "" );
            break;

        case ControllBarModel::STREAM:
            _view->enableButtonsForTv( true );
            break;

        case ControllBarModel::UNKOWN:
        case ControllBarModel::DOCUMENT:
            break;
    }

    _view->setMediaTitle( _model->title(), _model->artist() );
}

void ControllBarController::playerTimeChanged( qint64 newTime ) {
    switch( _model->mediaType() ) {
        case ControllBarModel::STREAM:
            return;

        case ControllBarModel::IMAGE:
            _view->setTimeText( 0, 0 );
            return;

        default:
            _view->setTimeText( newTime, _model->length() );
            break;
    }
}

void ControllBarController::playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState ) {
    switch( newState ) {
        case MediaPlayerState::PLAYING:
            if ( _model->mediaType() == ControllBarModel::IMAGE ) {
                _view->showPlayButton();
            } else {
                _view->showPauseButton();
            }

            _isPlaying = true;
            _view->setMediaTitle( _model->title(), _model->artist() );

            if ( _model->mediaType() == ControllBarModel::STREAM ) {
                _tvBufferingTime = 0;
                _tvBufferingTimer->start();
            }

            break;

        case MediaPlayerState::PAUSED:
        case MediaPlayerState::STOPPED:
        case MediaPlayerState::ENDED:
            _view->showPlayButton();
            _isPlaying = false;


            if ( _model->mediaType() == ControllBarModel::STREAM ) {
                _tvBufferingTimer->stop();
            }

            break;

        default:
            break;
    }
}

void ControllBarController::onTvBufferingTimer() {
    if( !_isBuffering ) {
        _tvBufferingTime += 1000;
        QString text = ControllBarUI::formatTime( _tvBufferingTime, ControllBarUI::getTimeFormat( _tvBufferingTime ) );
        _view->setTimeText( text );
    }
}

void ControllBarController::playerVisibilityChanged( bool visible ) {
    setVisible( visible );
}

void ControllBarController::centralUIRegionResized( int width, int height ) {
    _lastParentWitdh = width;
    _lastParentHeight = height;
    updateViewPosition( _lastParentWitdh, _lastParentHeight );
}

void ControllBarController::shellWidgetMoved() {
    updateViewPosition( _lastParentWitdh, _lastParentHeight );
}

void ControllBarController::updateViewPosition( int parentRegionWidth, int parentRegionHeight ) {
    QWidget *playerRegion = _shell->findRegion( "PlayerRegion" )->widget();
    QWidget *bottomRegion = _shell->findRegion( "BottomRegion" )->widget();
    int x = playerRegion->mapToGlobal( QPoint( 0, 0 ) ).x() + ( parentRegionWidth - Constants::CONTROLL_BAR_WIDTH ) / 2;
    int y = playerRegion->mapToGlobal( QPoint( 0, 0 ) ).y() + parentRegionHeight - Constants::CONTROLL_BAR_BOTTOM - Constants::CONTROLL_BAR_HEIGHT;

    if( _isPlayerFullScreen )
        y -= bottomRegion->height();

    _region->widget()->move( x, y );
}

void ControllBarController::onHideTimer() {
    QPoint mouseGlobalPos = QCursor::pos();
    QPoint topLeft = _view->mapToGlobal( QPoint( 0, 0 ) );
    QPoint bottomRight = _view->mapToGlobal( QPoint( _view->width(), _view->height() ) );
    bool isInControllBar = QRect( topLeft, bottomRight ).contains( mouseGlobalPos );
    QWidget *chapterRegion = _shell->findRegion( "ChapterSelectRegion" )->widget();
    topLeft = chapterRegion->mapToGlobal( QPoint( 0, 0 ) );
    bottomRight = chapterRegion->mapToGlobal( QPoint( chapterRegion->width(), chapterRegion->height() ) );
    bool isInChapterSelect = QRect( topLeft, bottomRight ).contains( mouseGlobalPos );

    if( !isInControllBar && !isInChapterSelect ) {
        setVisible( false );
    }
}

void ControllBarController::playerExitClicked() {
    setVisible( false );
}

void ControllBarController::initRegion() {
    _region->widget()->setAttribute( Qt::WA_TranslucentBackground, true ); //this makes it transparent!!!
    _region->widget()->resize( Constants::CONTROLL_BAR_WIDTH, Constants::CONTROLL_BAR_HEIGHT );
}

void ControllBarController::setVisible( bool visible ) {
    if ( visible == _isVisible ) return;

    if ( visible ) {
        _region->widget()->hide();
        _view->hide();
        updateViewPosition( _lastParentWitdh, _lastParentHeight );
        _region->widget()->show();
        _view->show();
        _view->update(); //4 mac no transparent corners with border
        _hideTimer->start( Constants::CONTROLL_BAR_HIDE_PERIOD );
    } else {
        _region->widget()->hide();
        _hideTimer->stop();
    }

    _isVisible = visible;
    _controllBarEventBridge->controllBarVisibiltyChanged( _isVisible );
}

void ControllBarController::videoScreenMouseMoved() {
    if( !_region->widget()->isVisible() ) {
        setVisible( true );
    } else {
        _hideTimer->start( Constants::CONTROLL_BAR_HIDE_PERIOD );
    }
}

void ControllBarController::onViewAction() {
    _hideTimer->start( Constants::CONTROLL_BAR_HIDE_PERIOD );
}

void ControllBarController::onBackwardClicked() {
    _controllBarEventBridge->backwardClicked();
}

void ControllBarController::onPlayClicked() {
    if ( _isPlaying ) {
        _controllBarEventBridge->pauseClicked();
    } else {
        _controllBarEventBridge->playClicked();
    }
}

void ControllBarController::onForwardClicked() {
    _controllBarEventBridge->forwardClicked();
}

void ControllBarController::onPreviewClicked() {
    _controllBarEventBridge->previewClicked();
}

void ControllBarController::onFullScreenClicked() {
    _controllBarEventBridge->fullScreenClicked();
}

void ControllBarController::onSnapshotClicked() {
    _controllBarEventBridge->snapshotClicked();
}

void ControllBarController::onMuteClicked() {
    _view->setMute( _view->mute() );
    _controllBarEventBridge->muteClicked( _view->mute() );
}

void ControllBarController::onResolutionClicked() {
    _controllBarEventBridge->resolutionClicked();
}

void ControllBarController::playerForwardSpeedChanged( int speed ) {
    _view->checkForwardButton( speed != 1 );
}

void ControllBarController::playerBackwardSpeedChanged( int speed ) {
    _view->checkBackwardButton( speed != 1 );
}

void ControllBarController::playerVolumneChanged( int volume ) {
    _view->setMute( volume == 0 );
    //_view->checkMuteButton(volume == 0);
}

void ControllBarController::keyPressed( QKeyEvent* keyEvent ) {
    /*if((keyEvent->key() == Qt::Key_Space) && _isVisible)
    {
        onPlayClicked();
    } */
}

void ControllBarController::bufferingAnimationChangedVisibility( bool visible ) {
    _view->bufferingAnimationChangedVisibility( visible );
    _isBuffering = visible;

    //disable/enable hide ControlBar
    if( visible )
        _hideTimer->disconnect( this );
    else
        connect( _hideTimer, SIGNAL( timeout() ), this, SLOT( onHideTimer() ) );
}

void ControllBarController::snapshotCaptureEnabled( bool enable ) {
    _view->enableSnapshotButtons( enable );
}

void ControllBarController::fullScreenStatusChanged( bool isFullScreen ) {
    _view->checkFullScreenButton( isFullScreen );
    _isPlayerFullScreen = isFullScreen;
    updateViewPosition( _lastParentWitdh, _lastParentHeight );
}
