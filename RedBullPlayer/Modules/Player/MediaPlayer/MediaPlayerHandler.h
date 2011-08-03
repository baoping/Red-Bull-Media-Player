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
#ifndef MEDIAPLAYERHANDLER_H
#define MEDIAPLAYERHANDLER_H

#include <QObject>
#include <QWidget>
#include <QUrl>
#include <QTimer>

// QTVLC includes

// Project Includes
#include "../../../Container/Error.h"
#include "../../../Interfaces/Enumerations/MediaPlayerStateEnum.h"
#include "../../../../QTVLC/src/Exceptions/Exception.h"

//Project forwards
class IMediaPlayer;

class MediaPlayerHandler : public QObject {
        Q_OBJECT

    public:
        MediaPlayerHandler( QObject* parent );
        ~MediaPlayerHandler();

        //Qt Display

        void setVout( QWidget* vout );

        void initMedia( QString mediaLocation, qint64 startTime );
        void closeMedia();


        //Playback
        void playMedia();
        void pauseMedia();
        void stopMedia();

        bool isPlaying();

        //Player Properties
        bool mute();
        void setMute( bool mute );
        void setVolume( int volume );
        int volume();
        void setFullscreen( bool fullScreen );
        void toogleFullScreen();
        bool isFullScreen();
        qint64 length();
        float position();
        qint64 time();
        void setTime( qint64 value );
        void addTime( qint64 value );
        void setAspectRatio( int value1, int value2 );
        QString aspectRatio();
        void setCropGeometry( int value1, int value2 );
        QString cropGeometry();

        int videoWidth();
        int videoHeight();

        void setForwardSpeed( float speed, bool resume );
        float forwardSpeed();
        void setBackwardSpeed( float speed, bool resume );
        float backwardSpeed();

        RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum state();

        //TV-Streams
        int getMediaSubItemsCount();
        void setMediaToSubItemAt( int index );

        //Tools
        void takeSnapshot( QString fileName, unsigned int width = 0, unsigned int height = 0 );
        void cleanUp();

        //Error handling
        RedBullPlayer::Container::Error* lastError() const {
            return _lastError;
        }

        bool hasError() const {
            return ( _lastError != 0 );
        }

        //Internals
        bool isInitialized() const {
            return _initialized;
        }

        bool timeOnNextTick() const {
            return _setTimeOnNextTick;
        }

        void setTimeOnNextTick( bool value ) {
            _setTimeOnNextTick = value;
        }

        void showText( QString text, int x, int y, int duration );

   signals:
        void aspectRatioChanged( float newAspectRatio );
        void forwardSpeedChanged( float newForwardSpeed );
        void backwardSpeedChanged( float newBackwardSpeed );
        void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState* state );
        void playerTimeChanged( qint64 newTime );
        void playerPositionChanged( float p );

    private slots:
        void timeChanged( qint64 newTime );
        void positionChanged( float position );
        void onBackwardTimer();
        void onForwardTimer();
        void onPlayerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState* state );

    private: //data
        volatile bool _initialized;
        bool _pausePlayerOnNextTick;
        bool _unmuteOnNextTick;

        IMediaPlayer* _mediaPlayer;
        QUrl _mediaUrl;
        QTimer* _backwardTimer;
        QTimer* _forwardTimer;
        float _backwardSpeed;
        float _forwardSpeed;

        //Error handling
        RedBullPlayer::Container::Error* _lastError;

        qint64 _startTime;
        bool _setTimeOnNextTick;
    private: //functions
        void bootstrap();
        void clearError();
        void setErrorMessage( QString message, RedBullPlayer::Container::Error::ErrorType errorType );
        void setExceptionMessage( Exception* exp );
        void setUnkownErrorMessage();
        bool prepareExecution();
};

#endif // MEDIAPLAYERHANDLER_H
