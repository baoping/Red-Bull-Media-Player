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
#ifndef MEDIAPLAYERINTERFACE_H
#define MEDIAPLAYERINTERFACE_H

// QT Includes
#include <QObject>
#include <QUrl>
#include <QTime>

// Project includes
#include "../../../RedBullPlayer/Interfaces/Enumerations/MediaPlayerStateEnum.h"

class IMediaPlayer : public QObject {
        Q_OBJECT

    public: //data
        static const int POSITION_RESOLUTION = 100000;
        static const int MAX_VOLUME = 100;
        static const int MIN_VOLUME = 0;

    public: //functions
        IMediaPlayer( QObject* parent );
        virtual ~IMediaPlayer();

        virtual void setMediaLocation( QString mediaLocation ) = 0;

        //Qt Display
        virtual void setVout( QWidget* widget ) = 0;
        virtual bool hasVout() = 0;

        //TV-Stream
        virtual int getMediaSubItemsCount() = 0;
        virtual void setMediaToSubItemAt( int index ) = 0;

        //MediaPlayer Properties
        virtual RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum state() = 0;
        virtual qint64 length() = 0;
        virtual qint64 time() = 0;
        virtual int volume() = 0;
        virtual bool mute() = 0;
        virtual float position() = 0;
        virtual int videoWidth() = 0;
        virtual int videoHeight() = 0;
        virtual float videoScale() = 0;
        virtual QString cropFilterGeometry() = 0;
        virtual bool isPlaying() = 0;
        virtual bool isFullscreen() = 0;
        virtual float rate() = 0;
        virtual QString cropGeometry() = 0;
        virtual QString aspectRatio() = 0;

        //Tools
        virtual void displayTextInVideo( QString text, int x, int y, int duration, int fontSize ) = 0;

    public Q_SLOTS:

        //Player
        virtual void play() = 0;
        virtual void pause() = 0;
        virtual void stop() = 0;
        virtual bool willPlay() = 0;
        virtual bool canPause() = 0;

        //MediaPlayer Properties
        virtual void setVolume( int volume ) = 0;
        virtual void setMute( bool mute ) = 0;
        virtual void setTime( qint64 newTime ) = 0;
        virtual void setFullscreen( bool enableFullScreen ) = 0;
        virtual void toggleFullscreen() = 0;
        virtual void toggleMute() = 0;
        virtual void setPosition( float newPos ) = 0;
        virtual void setScale( float newScale ) = 0;
        virtual void setRate( float newRate ) = 0;
        virtual void setAspectRatio( QString aspectRatio ) = 0;
        virtual void setCropGeometry( QString newValue ) = 0;

        //Tools
        virtual void takeSnapshot( QString fileName, unsigned int width = 0, unsigned int height = 0 ) = 0;
    Q_SIGNALS:
        void stopped();
        void paused();
        void playing();
        void buffering();
        void ended();
        void opening();
        void pausable( bool );
        void seekable( bool );
        void positionChanged( float );
        void timeChanged( qint64 newTime );
        //void mediaChanged( IMediaObject* newMedia );
        void snapshotTaken( QString fileName );
        void encounteredError();
        void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState* state );
};

#endif // MEDIAPLAYERINTERFACE_H


