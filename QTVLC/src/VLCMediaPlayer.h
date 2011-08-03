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
#ifndef VLCMEDIAPLAYER_H
#define VLCMEDIAPLAYER_H

#include "IMediaPlayer.h"

// QT includes
#include <QTime>
#include <QVBoxLayout>
#include <qglobal.h>

// QT Forwards
class QWidget;

// VLC Includes
#include "vlc/vlc.h"

// Project includes
#include "VLCEventBridge.h"
#include "VLCMediaEventBridge.h"
#include "VLCInstance.h"

// Project Forwards
class QTimer;

class VLCMediaPlayer : public IMediaPlayer {
        Q_OBJECT

        friend class VLCEventBridge;
        friend class VLCMediaEventBridge;

    public:
        VLCMediaPlayer( VLCInstance* vlcInstance,
                        QObject* parent );
        virtual ~VLCMediaPlayer();

        virtual void setMediaLocation( QString mediaLocation );

        //Qt Display
        virtual void setVout( QWidget* widget );
        virtual bool hasVout();

        //TV-Stream
        virtual int getMediaSubItemsCount();
        virtual void setMediaToSubItemAt( int index );

        //MediaPlayer Properties
        virtual RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum state();
        virtual qint64 length();
        virtual qint64 time();
        virtual int volume();
        virtual bool mute();
        virtual float position();
        virtual int videoWidth();
        virtual int videoHeight();
        virtual float videoScale();
        virtual QString cropFilterGeometry();
        virtual bool isPlaying();
        virtual bool isFullscreen();
        virtual float rate();
        virtual QString cropGeometry();
        virtual QString aspectRatio();

        //Tools
        virtual void displayTextInVideo( QString text, int x, int y, int duration, int fontSize );

    public Q_SLOTS:

        //Player
        virtual void play();
        virtual void pause();
        virtual void stop();

        virtual bool willPlay();
        virtual bool canPause();

        //MediaPlayer Properties

        virtual void setVolume( int volume );
        virtual void setMute( bool mute );
        virtual void setTime( qint64 newTime );
        virtual void setFullscreen( bool enableFullScreen );
        virtual void toggleFullscreen();
        virtual void toggleMute();
        virtual void setPosition( float newPos );
        virtual void setScale( float newScale );
        virtual void setRate( float newRate );
        virtual void setAspectRatio( QString aspectRatio );
        virtual void setCropGeometry( QString newValue );

        //Tools
        virtual void takeSnapshot( QString fileName, unsigned int width = 0, unsigned int height = 0 );
    private: //functions
        void emitPaused();
        void emitEnded();
        void emitStopped();
        void emitOpening();
        void emitPlaying();
        void emitBuffering();
        void emitPausable( bool newPausable );
        void emitSeekable( bool newSeekable );
        void emitPositionChanged( float newPosition );
        void emitTimeChanged( qint64 newTime );
        void emitSnapshotTaken( QString fileName );
        void emitEncounteredError();
        void emitPlayerStateChanged( libvlc_state_t new_state );

        void handleMediaChanged( libvlc_media_t* new_media );

    private: //data
        VLCInstance* _vlcInstance;
        VLCEventBridge* _eventBridge;
        VLCMediaEventBridge* _mediaEventBridge;
        libvlc_media_player_t* _mediaPlayer;

        RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum _state;
};

#endif // VLCMEDIAPLAYER_H
