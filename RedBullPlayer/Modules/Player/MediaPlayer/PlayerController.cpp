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

using namespace RedBullPlayer::Modules::Player;

PlayerController::PlayerController( PlayerModel* model, IPlayerView* view,  QObject *parent ) :
    QObject( parent ) {
    _model = model;
    connect( _model, SIGNAL( mediaChanged() ), this, SLOT( mediaChanged() ) );
    _view = view;
    _mediaPlayerHandler = 0;
    _controllBarEventBridge = createControllBarEventBridge( true );
    _playListEventBridge = createPlayListEventBridge( true );
}

PlayerController::~PlayerController() {
    disconnect( _model, SIGNAL( mediaChanged() ), this, SLOT( mediaChanged() ) );
    _model = 0;

    if ( _mediaPlayerHandler != 0 ) {
        delete _mediaPlayerHandler;
    }

    delete _controllBarEventBridge;
}

ControllBarEventBridge* PlayerController::createControllBarEventBridge( bool connectSignals ) {
    ControllBarEventBridge* controllBarEventBridge = new ControllBarEventBridge( this );

    if ( connectSignals ) {
        connect( controllBarEventBridge, SIGNAL( pauseClicked() ), this, SLOT( pauseClicked() ) );
        connect( controllBarEventBridge, SIGNAL( playClicked() ), this, SLOT( playClicked() ) );
        connect( controllBarEventBridge, SIGNAL( snapShotClicked() ), this, SLOT( snapShotClicked() ) );
        connect( controllBarEventBridge, SIGNAL( fullScreenClicked() ), this, SLOT( fullScreenClicked() ) );
        connect( controllBarEventBridge, SIGNAL( muteClicked() ), this, SLOT( muteClicked() ) );
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
                 SIGNAL( playListItemSelected( RedBullPlayer::Modules::MediaLibrary::IMedia* ) ),
                 this,
                 SLOT( playListItemSelected( RedBullPlayer::Modules::MediaLibrary::IMedia* ) ) );
    }

    return playListEventBridge;
}

MediaPlayerHandler* PlayerController::createMediaPlayerHandler( bool connectSignals ) {
    MediaPlayerHandler* mediaPlayerHandler = new MediaPlayerHandler( this );

    if ( connectSignals ) {
        connect( mediaPlayerHandler, SIGNAL( aspectRatioChanged( float ) ), this, SLOT( aspectRatioChanged( float ) ) );
        connect( mediaPlayerHandler, SIGNAL( backwardSpeedChanged( float ) ), this, SLOT( backwardSpeedChanged( float ) ) );
        connect( mediaPlayerHandler, SIGNAL( forwardSpeedChanged( float ) ), this, SLOT( forwardSpeedChanged( float ) ) );
        connect( mediaPlayerHandler, SIGNAL( playerStateChanged() ), this, SLOT( playerStateChanged() ) );
        connect( mediaPlayerHandler, SIGNAL( playerTimeChanged( qint64 ) ), this, SLOT( playerTimeChanged( qint64 ) ) );
    }

    return mediaPlayerHandler;
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
            _view->showBufferingPage();
            break;

        case PlayerModel::UNKOWN:
            _view->showDummyPage();
            break;
    }
}

bool PlayerController::playMedia() {
    switch( _model->mediaType() ) {
        case PlayerModel::VIDEO:
            return playVideo();

        case PlayerModel::AUDIO:
            return playAudio();

        case PlayerModel::IMAGE:
            return showImage();

        case PlayerModel::STREAM:
            return playStream();

        case PlayerModel::UNKOWN:
            break;
    }

    return false;
}

bool PlayerController::playVideo() {
    if ( _mediaPlayerHandler == 0 ) {
        _mediaPlayerHandler = createMediaPlayerHandler( true );
        _mediaPlayerHandler->setVout( _view->videoPage() );
    }

    _mediaPlayerHandler->initVideo( _model->fileName(), _model->startTime() );
    _mediaPlayerHandler->playVideo();
    return true;
}

bool PlayerController::playAudio() {
    return true;
}

bool PlayerController::showImage() {
    return true;
}

bool PlayerController::playStream() {
    return true;
}

/***** CONTROLL BAR ****/

void PlayerController::pauseClicked() {
}

void PlayerController::playClicked() {
}

void PlayerController::snapShotClicked() {
}

void PlayerController::fullScreenClicked() {
}

void PlayerController::muteClicked() {
}

void PlayerController::forwardClicked() {
}

void PlayerController::backwardClicked() {
}

void PlayerController::resolutionClicked() {
}

/***** PLAYLIST *****/

void PlayerController::playListItemSelected( RedBullPlayer::Modules::MediaLibrary::IMedia* media ) {
    //set new media
    _model->setMedia( media );
    //play media
    playMedia();
}

/***** MEDIAPLAYER ****/
void PlayerController::aspectRatioChanged( float newAspectRatio ) {
    qDebug() << Q_FUNC_INFO;
}

void PlayerController::forwardSpeedChanged( float newForwardSpeed ) {
    qDebug() << Q_FUNC_INFO;
}

void PlayerController::backwardSpeedChanged( float newBackwardSpeed ) {
    qDebug() << Q_FUNC_INFO;
}

void PlayerController::playerStateChanged() {
    qDebug() << Q_FUNC_INFO;
}

void PlayerController::playerTimeChanged( qint64 time ) {
    qDebug() << Q_FUNC_INFO;
}
