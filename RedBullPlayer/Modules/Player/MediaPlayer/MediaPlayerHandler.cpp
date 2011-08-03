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
#include "MediaPlayerHandler.h"

// Qt Includes
#include <QDebug>

// QTVLC includes
#include "../../../../QTVLC/src/Factory/MediaPlayerFactory.h"
#include "../../../../QTVLC/src/Interfaces/IMediaPlayer.h"

// Project Includes
#include "../../../Container/Constants.h"
#include "../../../Container/Constants.h"
#include "../../../ToolLib/SimpleLogger.h"
#include "../../../Interfaces/Enumerations/MediaPlayerStateEnum.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::Player;

MediaPlayerHandler::MediaPlayerHandler( QObject* parent ): QObject( parent ) {
    bootstrap();
}

MediaPlayerHandler::~MediaPlayerHandler() {
    if ( _mediaPlayer ) {
        delete _mediaPlayer;
        _mediaPlayer = 0;
    }
}

void MediaPlayerHandler::bootstrap() {
    _lastError = 0;
    _mediaPlayer = 0;
    _backwardSpeed = 1;
    _forwardSpeed = 1;
    _backwardTimer = new QTimer( this );
    _backwardTimer->setInterval( Constants::PLAYER_BACKWARD_TIMER_PERIOD );
    connect( _backwardTimer, SIGNAL( timeout() ), this, SLOT( onBackwardTimer() ) );
    _forwardTimer = new QTimer( this );
    _forwardTimer->setInterval( Constants::PLAYER_FORWARD_TIMER_PERIOD );
    connect( _forwardTimer, SIGNAL( timeout() ), this, SLOT( onForwardTimer() ) );
    _pausePlayerOnNextTick = false;
    _unmuteOnNextTick = false;
    _initialized = false;
    _setTimeOnNextTick = false;
    _mediaPlayer = MediaPlayerFactory::instance()->createMediaPlayer(
                       MediaPlayerPurpose::PLAY_BACK,
                       this ); //possible remove the parent param
    connect( _mediaPlayer,
             SIGNAL( playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState* ) ),
             this,
             SLOT( onPlayerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState* ) ) );
    connect( _mediaPlayer,
             SIGNAL( timeChanged( qint64 ) ),
             this,
             SLOT( timeChanged( qint64 ) ) );
    connect( _mediaPlayer,
             SIGNAL( positionChanged( float ) ),
             this,
             SLOT( positionChanged( float ) ) );
}

void MediaPlayerHandler::closeMedia() {
    clearError();
    _backwardTimer->stop();
    _forwardTimer->stop();
    _backwardSpeed = 1;
    _forwardSpeed = 1;
    _initialized = false;
}

void MediaPlayerHandler::setVout( QWidget* vout ) {
    try {
        _mediaPlayer->setVout( vout );
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

void MediaPlayerHandler::initMedia( QString mediaLocation, qint64 startTime ) {
    clearError();
    if ( _mediaPlayer->state() == MediaPlayerState::ENDED ) {
        _startTime = 0;
        _setTimeOnNextTick = false;
    } else {
        _startTime = startTime;

        if ( _startTime > 0 ) {
            _setTimeOnNextTick = true;
        }
    }

    _mediaPlayer->setMediaLocation( mediaLocation );
    _initialized = true;
}

/* PLAYBACK */
void MediaPlayerHandler::playMedia() {
    if ( !prepareExecution() ) return;

    try {
        if ( _mediaPlayer->state() == MediaPlayerState::ENDED ) {
            _mediaPlayer->stop();
        }

        _mediaPlayer->play();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

void MediaPlayerHandler::pauseMedia() {
    if ( !prepareExecution() ) return;

    try {
        _mediaPlayer->pause();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

void MediaPlayerHandler::stopMedia() {
    if ( !prepareExecution() ) return;

    try {
        _mediaPlayer->stop();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

bool MediaPlayerHandler::isPlaying() {
    if ( !prepareExecution() ) return false;

    try {
        return _mediaPlayer->isPlaying();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return false;
}

/* PLAYER PROPERTIES */
bool MediaPlayerHandler::mute() {
    if ( !prepareExecution() ) return false;

    try {
        return _mediaPlayer->mute();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return false;
}


void MediaPlayerHandler::setMute( bool mute ) {
    if ( !prepareExecution() ) return;

    try {
        _mediaPlayer->setMute( mute );
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

void MediaPlayerHandler::setVolume( int volume ) {
    if ( !prepareExecution() ) return;

    try {
        _mediaPlayer->setVolume( volume );
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

int MediaPlayerHandler::volume() {
    if ( !prepareExecution() ) return 0;

    try {
        return _mediaPlayer->volume();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return 0;
}

void MediaPlayerHandler::setFullscreen( bool fullScreen ) {
    if ( !prepareExecution() ) return;

    try {
        _mediaPlayer->setFullscreen( fullScreen );
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

void MediaPlayerHandler::toogleFullScreen() {
    if ( !prepareExecution() ) return;

    try {
        _mediaPlayer->toggleFullscreen();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

bool MediaPlayerHandler::isFullScreen() {
    if ( !prepareExecution() ) return false;

    try {
        return _mediaPlayer->isFullscreen();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return false;
}

qint64 MediaPlayerHandler::length() {
    if ( !prepareExecution() ) return 0;

    try {
        return _mediaPlayer->length();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return 0;
}

float MediaPlayerHandler::position() {
    if ( !prepareExecution() ) return 0.0F;

    try {
        return _mediaPlayer->position();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return 0.0F;
}

qint64 MediaPlayerHandler::time() {
    if ( !prepareExecution() ) return 0;

    try {
        return _mediaPlayer->time();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return 0;
}

void MediaPlayerHandler::setTime( qint64 value ) {
    if ( !prepareExecution() ) return;

    try {
        if ( _mediaPlayer->state() == MediaPlayerState::ENDED ) {
            _mediaPlayer->stop();
            _mediaPlayer->play();
            _mediaPlayer->setTime( value );
        } else {
            _mediaPlayer->setTime( value );
        }
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

void MediaPlayerHandler::addTime( qint64 value ) {
    qint64 newTime = time() + value;

    if ( newTime < 0 ) {
        newTime = 0;
    }

    if ( newTime > length() )
        newTime = length();

    setTime( newTime );
}

void MediaPlayerHandler::setAspectRatio( int value1, int value2 ) {
    if ( !prepareExecution() ) return;

    try {
        QString newValue = "Default";
        float ratio = 0;

        if ( value1 != 0 && value2 != 0 ) {
            newValue = QString( "%1:%2" ).arg( value1 ).arg( value2 );
            ratio = ( float ) value1 / ( float ) value2;
        }

        _mediaPlayer->setAspectRatio( newValue );
        emit aspectRatioChanged( ratio );
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

QString MediaPlayerHandler::aspectRatio() {
    if ( !prepareExecution() ) return "Default";

    try {
        return _mediaPlayer->aspectRatio();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return "Default";
}

void MediaPlayerHandler::setCropGeometry( int value1, int value2 ) {
    if ( !prepareExecution() ) return;

    try {
        QString newValue = "Default";

        if ( value1 != 0 && value2 != 0 ) {
            newValue = QString( "%1x%2+0+0" ).arg( value1 ).arg( value2 );
        }

        _mediaPlayer->setCropGeometry( newValue );
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

QString MediaPlayerHandler::cropGeometry() {
    if ( !prepareExecution() ) return "Default";

    try {
        return _mediaPlayer->cropGeometry();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return "Default";
}

int MediaPlayerHandler::videoWidth() {
    if ( !prepareExecution() ) return 0;

    try {
        return _mediaPlayer->videoWidth();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return 0;
}

int MediaPlayerHandler::videoHeight() {
    if ( !prepareExecution() ) return 0;

    try {
        return _mediaPlayer->videoHeight();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return 0;
}

void MediaPlayerHandler::showText( QString text, int x, int y, int duration ) {
    if ( !prepareExecution() ) return;

    try {
        _mediaPlayer->displayTextInVideo( text, x, y, duration, 0 );
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return;
}

void MediaPlayerHandler::setForwardSpeed( float speed, bool resume ) {
    if ( !prepareExecution() ) return;

    _forwardSpeed = speed;

    if ( _forwardSpeed == 1 ) {
        _mediaPlayer->displayTextInVideo( "", 10, 10, 1, 0 );
        _forwardTimer->stop();

        if ( resume ) {
            playMedia();
        }
    } else {
        if ( !_forwardTimer->isActive() ) {
            pauseMedia();
            _forwardTimer->start();
        }

        QString osdText = QString( "%1x" ).arg( _forwardSpeed );
        _mediaPlayer->displayTextInVideo( osdText, 10, 10, 2000000, 0 );
    }

    if ( _lastError == 0 ) {
        //PlayerForwardSpeedChangedEvent e(_forwardSpeed);
        //Aggregator::instance()->sendEvent(&e);
        emit forwardSpeedChanged( _forwardSpeed );
    }
}

float MediaPlayerHandler::forwardSpeed() {
    return _forwardSpeed;
}

void MediaPlayerHandler::setBackwardSpeed( float speed, bool resume ) {
    if ( !prepareExecution() ) return;

    _backwardSpeed = speed;

    if ( _backwardSpeed == 1 ) {
        _mediaPlayer->displayTextInVideo( "", 10, 10, 1, 0 );
        _backwardTimer->stop();

        if ( resume ) {
            playMedia();
        }
    } else {
        if ( !_backwardTimer->isActive() ) {
            pauseMedia();
            _backwardTimer->start();
        }

        QString osdText = QString( "%1x" ).arg( _backwardSpeed );
        _mediaPlayer->displayTextInVideo( osdText, 10, 10, 2000000, 0 );
    }

    if ( _lastError == 0 ) {
        //PlayerBackwardSpeedChangedEvent e(_backwardSpeed);
        //Aggregator::instance()->sendEvent(&e);
        emit backwardSpeedChanged( _backwardSpeed );
    }
}

float MediaPlayerHandler::backwardSpeed() {
    return _backwardSpeed;
}

RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum MediaPlayerHandler::state() {
    if ( !prepareExecution() ) return RedBullPlayer::Modules::Player::MediaPlayerState::NONE;

    return _mediaPlayer->state();
}

//TV-Streams
int MediaPlayerHandler::getMediaSubItemsCount() {
    if ( !prepareExecution() ) return 0;

    try {
        return _mediaPlayer->getMediaSubItemsCount();
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }

    return 0;
}

void MediaPlayerHandler::setMediaToSubItemAt( int index ) {
    if ( !prepareExecution() ) return;

    try {
        _mediaPlayer->setMediaToSubItemAt( index );
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

//Tools
void MediaPlayerHandler::takeSnapshot( QString fileName, unsigned int width, unsigned int height ) {
    if ( !prepareExecution() ) return;

    try {
        _mediaPlayer->takeSnapshot( fileName, width, height );
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

void MediaPlayerHandler::cleanUp() {
    _forwardTimer->stop();
    _backwardTimer->stop();
    clearError();

    if ( _mediaPlayer ) {
        _mediaPlayer->stop();
        _mediaPlayer->disconnect( this );
        _mediaPlayer->deleteLater();
    }
}

/* SLOTS */

void MediaPlayerHandler::timeChanged( qint64 newTime ) {
    if ( !_initialized ) return;

    emit playerTimeChanged( newTime );
}

void MediaPlayerHandler::positionChanged( float p ) {
    if ( !_initialized ) return;

    emit playerPositionChanged( p );
}

void MediaPlayerHandler::onBackwardTimer() {
    if ( !prepareExecution() ) return;

    try {
        float backStep = _backwardSpeed * Constants::PLAYER_BACKWARD_STEP;
        qint64 time = _mediaPlayer->time();

        if ( time - backStep > 0 ) {
            setTime( (qint64) ( time - backStep ) );

            //If Player is paused. vlc_player doesn't emit timeChanged so we have to handle this.
            if ( ( state() == MediaPlayerState::PAUSED ) ||
                    ( state() == MediaPlayerState::STOPPED ) ) {
                //PlayerTimeChangedEvent playerTimeChangedEvent(time - backStep,false);
                //Aggregator::instance()->sendEvent(&playerTimeChangedEvent);
                emit playerTimeChanged( (qint64) ( time - backStep ) );
            }
        } else {
            setBackwardSpeed( 1, true );
        }
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

void MediaPlayerHandler::onForwardTimer() {
    if ( !prepareExecution() ) return;

    try {
        float forwardStep = _forwardSpeed * Constants::PLAYER_FORWARD_STEP;
        qint64 time = _mediaPlayer->time();
        qint64 duration = _mediaPlayer->length();
        qint64 newTime = 0;

        if ( time + forwardStep <= duration ) {
            newTime = (qint64) ( time + forwardStep );
            setTime( newTime );
        } else {
            newTime = duration;
            setTime( duration );
            setForwardSpeed( 1, false );
        }

        //If Player is paused. vlc_player doesn't emit timeChanged so we have to handle this.
        if ( ( state() == MediaPlayerState::PAUSED ) ||
                ( state() == MediaPlayerState::STOPPED ) ) {
            //PlayerTimeChangedEvent playerTimeChangedEvent(newTime,false);
            //Aggregator::instance()->sendEvent(&playerTimeChangedEvent);
            emit playerTimeChanged( newTime );
        }
    } catch( Exception *exp ) {
        setExceptionMessage( exp );
    } catch( ... ) {
        setUnkownErrorMessage();
    }
}

void MediaPlayerHandler::onPlayerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState* state ) {
    emit playerStateChanged( state );
}

/* ERROR HANDLING */

void MediaPlayerHandler::clearError() {
    if ( _lastError != 0 )
        delete _lastError;

    _lastError = 0;
}

void MediaPlayerHandler::setExceptionMessage( Exception* exp ) {
    setErrorMessage( tr( exp->message().toUtf8().constData() ), Error::Fatal );
}

void MediaPlayerHandler::setUnkownErrorMessage() {
    setErrorMessage( tr( "Unkown VLC Error!" ), Error::Normal );
}

void MediaPlayerHandler::setErrorMessage( QString message, Error::ErrorType errorType ) {
    clearError();
    _lastError = new Error();
    _lastError->setErrorMessage( message, errorType );
}

bool MediaPlayerHandler::prepareExecution() {
    clearError();
    return _initialized;
}
