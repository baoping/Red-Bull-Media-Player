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

#include "VLCMediaPlayer.h"

// STD inludes
#include <cmath>

// QT Includes
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QWidget>
#include <QDir>
#include <QThread>
#include <QEvent>

// Project includes
#include "../../RedBullPlayer/ToolLib/SimpleLogger.h"
#include "../../RedBullPlayer/Container/Constants.h"

#include "VLCException.h"
#include "MediaPlayerException.h"
#include "CocoaContainer.h"


using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::Player;


VLCMediaPlayer::VLCMediaPlayer( VLCInstance* vlcInstance, QObject* parent )
    : IMediaPlayer( parent ), _vlcInstance( vlcInstance )

{
    SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC, "Create new VLCMediaPlayer instance." );
    //Create new vlc_media_player
    _mediaPlayer = libvlc_media_player_new( vlcInstance->vlcInstance() );

    if ( !_mediaPlayer )
        throw new VLCException( libvlc_errmsg() );

    libvlc_video_set_logo_int( _mediaPlayer, libvlc_logo_enable, 0 );
    //VLCEventBridge Proxy
    _eventBridge = new VLCEventBridge( this );
    _eventBridge->attachEvents();
    _mediaEventBridge = 0;
    _state = MediaPlayerState::NONE;
}

VLCMediaPlayer::~VLCMediaPlayer() {
#if !defined(Q_OS_MAC)
    libvlc_video_set_marquee_int( _mediaPlayer, libvlc_marquee_Enable, 0 );
#endif
    //detachEvents
    _eventBridge->detachEvents();
    // Delete EventBridge to avoid dangling pointers
    delete _eventBridge;
    //Stop playing
    //libvlc_media_player_stop (_mediaPlayer);
    //Free the media_player
    libvlc_media_player_release ( _mediaPlayer );
}

void VLCMediaPlayer::setMediaLocation( QString mediaLocation ) {
    if ( _mediaEventBridge != 0 ) {
        _mediaEventBridge->detachEvents();
        delete _mediaEventBridge;
        _mediaEventBridge = 0;
    }

    //Create vlc_media from MediaUrl
    libvlc_media_t* vlc_media = libvlc_media_new_location( _vlcInstance->vlcInstance(), mediaLocation.toUtf8() );

    if ( !vlc_media )
        throw new VLCException( libvlc_errmsg() );

    libvlc_media_player_set_media( _mediaPlayer, vlc_media );
    //safely release vlc_media
    libvlc_media_release( vlc_media );
}

void VLCMediaPlayer::setVout( QWidget* widget ) {
    if ( widget == 0 ) {
        throw new MediaPlayerException( "Vout can not be NULL!" );
    }

#if defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd( _mediaPlayer, widget->winId() );
#elif defined(Q_OS_MAC)
    CocoaContainer c;
    QWidget* videoWidget = c.createCocoaWidget( _mediaPlayer, widget );
    widget->layout()->addWidget( videoWidget );
#else //Linux
    libvlc_media_player_set_xwindow( _mediaPlayer, widget->winId() );
#endif
}

bool VLCMediaPlayer::hasVout() {
    return ( libvlc_media_player_has_vout( _mediaPlayer ) > 0 );
}

bool VLCMediaPlayer::willPlay() {
    //return (libvlc_media_player_will_play(_mediaPlayer) == 1);
    return true;
}

bool VLCMediaPlayer::isPlaying() {
    return ( libvlc_media_player_is_playing( _mediaPlayer ) == 1 );
}

void VLCMediaPlayer::play() {
    if ( willPlay() ) {
        libvlc_media_player_play( _mediaPlayer );
    } else {
        //throw new MediaPlayerException( tr("Invalid Operation: Player will not play.") );
        throw new VLCException( libvlc_errmsg() );
    }
}

int VLCMediaPlayer::getMediaSubItemsCount() {
    SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC, QString( "Get libvlc_media_t from libvlc_media_player." ) );
    libvlc_media_t* vlc_media = libvlc_media_player_get_media( _mediaPlayer );
    QByteArray mrl = libvlc_media_get_mrl( vlc_media );
    QString mrlStr = QString( mrl ).toLatin1();

    if ( !mrlStr.contains( "http" ) )
        return 0; //only if online content

    SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC, QString( "Get subitems from libvlc_media_t." ) );
    libvlc_media_list_t* vlc_media_list = libvlc_media_subitems( vlc_media );

    if ( vlc_media_list != 0 ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "Lock libvlc_media_list_t." ) );
        libvlc_media_list_lock( vlc_media_list );
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "Get libvlc_media_list_t item count." ) );
        int count = libvlc_media_list_count( vlc_media_list );
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "libvlc_media_list_t item count: %1" )
                                         .arg( count ) );
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "Unlock libvlc_media_list_t." ) );
        libvlc_media_list_unlock( vlc_media_list );
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "Release libvlc_media_list_t." ) );
        libvlc_media_list_release( vlc_media_list );
        return count;
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "libvlc_media_list_t is NULL." ) );
    }

    return 0;
}

void VLCMediaPlayer::setMediaToSubItemAt( int index ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC, QString( "Get libvlc_media_t from libvlc_media_player." ) );
    libvlc_media_t* vlc_media = libvlc_media_player_get_media( _mediaPlayer );
    SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC, QString( "Get subitems from libvlc_media_t." ) );
    libvlc_media_list_t* vlc_media_list = libvlc_media_subitems( vlc_media );

    if ( vlc_media_list != 0 ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "Lock libvlc_media_list_t." ) );
        libvlc_media_list_lock( vlc_media_list );
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "Get libvlc_media_t at index 0." ) );
        libvlc_media_t* new_media = libvlc_media_list_item_at_index( vlc_media_list, index );
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "Unlock libvlc_media_list_t." ) );
        libvlc_media_list_unlock( vlc_media_list );
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "Release libvlc_media_list_t." ) );
        libvlc_media_list_release( vlc_media_list );

        if ( new_media != 0 ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                             QString( "Set libvlc_media_t to libvlc_media_player_t." ) );
            libvlc_media_player_set_media( _mediaPlayer, new_media );
        }
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_QTVLC,
                                         QString( "libvlc_media_list_t is NULL." ) );
    }
}

bool VLCMediaPlayer::canPause() {
    return ( libvlc_media_player_can_pause( _mediaPlayer ) == 1 );
}

void VLCMediaPlayer::pause() {
    if ( canPause() ) {
        libvlc_media_player_pause( _mediaPlayer );
    } else {
        throw new MediaPlayerException( tr( "Invalid Operation: Player cannot pause." ) );
    }
}

void VLCMediaPlayer::stop() {
    if ( isPlaying() ) {
        libvlc_media_player_stop ( _mediaPlayer );
    }
}

void VLCMediaPlayer::setVolume( int volume ) {
    if ( volume < 0 || volume > 100 ) {
        throw new MediaPlayerException( tr( "Invalid volume setting: %1. Volume setting be in the range between %2-%3" ).arg( volume, 0, 100 ) );
    }

    //calculate "VLC-volume" 0-400
    float step = IMediaPlayer::MAX_VOLUME / IMediaPlayer::MAX_VOLUME;
    float value = step * volume;
    int vlc_volume = ( int )value;
    libvlc_audio_set_volume( _mediaPlayer, vlc_volume );
}

int VLCMediaPlayer::volume() {
    int retVal = libvlc_audio_get_volume( _mediaPlayer );
    float step = IMediaPlayer::MAX_VOLUME / IMediaPlayer::MAX_VOLUME;
    return ( int ) ( retVal / step );
}

void VLCMediaPlayer::setMute( bool mute ) {
    if ( mute ) {
        libvlc_audio_set_mute( _mediaPlayer, 1 );
    } else {
        libvlc_audio_set_mute( _mediaPlayer, 0 );
    }
}

void VLCMediaPlayer::toggleMute() {
    libvlc_audio_toggle_mute( _mediaPlayer );
}

bool VLCMediaPlayer::mute() {
    return ( libvlc_audio_get_mute( _mediaPlayer ) == 1 );
}

void VLCMediaPlayer::setTime( qint64 newTime ) {
    if ( newTime < 0 ) return;

    libvlc_media_player_set_time( _mediaPlayer, newTime );
}

void VLCMediaPlayer::setFullscreen( bool enableFullScreen ) {
    if ( enableFullScreen ) {
        libvlc_set_fullscreen( _mediaPlayer, 1 );
    } else {
        libvlc_set_fullscreen( _mediaPlayer, 0 );
    }
}

void VLCMediaPlayer::toggleFullscreen() {
    libvlc_toggle_fullscreen( _mediaPlayer );
}

bool VLCMediaPlayer::isFullscreen() {
    return ( libvlc_get_fullscreen( _mediaPlayer ) == 1 );
}

void VLCMediaPlayer::displayTextInVideo( QString text, int x, int y, int duration, int fontSize ) {
#if !defined(Q_OS_MAC)

    if ( text == "" ) {
        libvlc_video_set_marquee_int( _mediaPlayer, libvlc_marquee_Enable, 0 );
    } else {
        libvlc_video_set_marquee_int( _mediaPlayer, libvlc_marquee_Enable, 1 );
        libvlc_video_set_marquee_string( _mediaPlayer, libvlc_marquee_Text, text.toLatin1() );
        libvlc_video_set_marquee_int( _mediaPlayer, libvlc_marquee_Timeout, duration );
        libvlc_video_set_marquee_int( _mediaPlayer, libvlc_marquee_X, x );
        libvlc_video_set_marquee_int( _mediaPlayer, libvlc_marquee_Y, y );
        libvlc_video_set_marquee_int( _mediaPlayer, libvlc_marquee_Size, fontSize );
    }

#endif
}

float VLCMediaPlayer::position() {
    return libvlc_media_player_get_position( _mediaPlayer );
}

void VLCMediaPlayer::setPosition( float newPos ) {
    libvlc_media_player_set_position( _mediaPlayer, newPos );
}

void VLCMediaPlayer::takeSnapshot( QString fileName,
                                   unsigned int width,
                                   unsigned int height ) {
    libvlc_video_take_snapshot( _mediaPlayer, 0, fileName.toUtf8().data(), width, height );
}

int VLCMediaPlayer::videoWidth() {
    uint w;
    uint h;

    if( ! libvlc_video_get_size( _mediaPlayer, 0, &w, &h )  == -1 ) {
        return -1;
    }

    return w;
}

int VLCMediaPlayer::videoHeight() {
    uint w;
    uint h;

    if( ! libvlc_video_get_size( _mediaPlayer, 0, &w, &h )  == -1 ) {
        return -1;
    }

    return h;
}

float VLCMediaPlayer::videoScale() {
    return libvlc_video_get_scale( _mediaPlayer );
}

QString VLCMediaPlayer::cropFilterGeometry() {
    return ( QString )libvlc_video_get_crop_geometry( _mediaPlayer );
}

void VLCMediaPlayer::setScale( float newScale ) {
    libvlc_video_set_scale( _mediaPlayer, newScale );
}

void VLCMediaPlayer::setRate( float newRate ) {
    libvlc_media_player_set_rate( _mediaPlayer, newRate );
}

float VLCMediaPlayer::rate() {
    return libvlc_media_player_get_rate( _mediaPlayer );
}

void VLCMediaPlayer::setAspectRatio( QString aspectRatio ) {
    if ( aspectRatio == "Default" ) {
        libvlc_video_set_aspect_ratio( _mediaPlayer, NULL );
    } else {
        libvlc_video_set_aspect_ratio( _mediaPlayer, aspectRatio.toUtf8() );
    }
}

QString VLCMediaPlayer::VLCMediaPlayer::aspectRatio() {
    return QString( libvlc_video_get_aspect_ratio( _mediaPlayer ) );
}

void VLCMediaPlayer::setCropGeometry( QString newValue ) {
    libvlc_video_set_crop_geometry( _mediaPlayer, newValue.toUtf8() );
}

QString VLCMediaPlayer::cropGeometry() {
    return libvlc_video_get_crop_geometry( _mediaPlayer );
}

MediaPlayerState::StateEnum VLCMediaPlayer::state() {
    return _state;
}

void VLCMediaPlayer::emitPaused() {
    Q_EMIT paused();
}

void VLCMediaPlayer::emitEnded() {
    Q_EMIT ended();
}

void VLCMediaPlayer::emitStopped() {
    Q_EMIT stopped();
}

void VLCMediaPlayer::emitOpening() {
    Q_EMIT opening();
}

void VLCMediaPlayer::emitPlaying() {
    Q_EMIT playing();
}

void VLCMediaPlayer::emitBuffering() {
    Q_EMIT buffering();
}

void VLCMediaPlayer::emitPausable( bool newPausable ) {
    Q_EMIT pausable( newPausable );
}

void VLCMediaPlayer::emitSeekable( bool newSeekable ) {
    Q_EMIT seekable( newSeekable );
}

void VLCMediaPlayer::emitPositionChanged( float newPosition ) {
    Q_EMIT positionChanged( newPosition );
}

void VLCMediaPlayer::emitTimeChanged( qint64 newTime ) {
    Q_EMIT timeChanged( newTime );
}

void VLCMediaPlayer::emitEncounteredError() {
    Q_EMIT encounteredError();
}

void VLCMediaPlayer::emitSnapshotTaken( QString fileName ) {
    Q_EMIT snapshotTaken( fileName );
}

void VLCMediaPlayer::handleMediaChanged( libvlc_media_t* new_media ) {
    _mediaEventBridge = new VLCMediaEventBridge( this, new_media );
    _mediaEventBridge->attachEvents();
}

void VLCMediaPlayer::emitPlayerStateChanged( libvlc_state_t new_state ) {
    //Translate vlc_state to MediaPlayerState
    MediaPlayerState::StateEnum newState;

    switch( new_state ) {
        case libvlc_NothingSpecial:
            newState = MediaPlayerState::NONE;
            break;

        case libvlc_Opening :
            newState = MediaPlayerState::OPENING;
            break;

        case libvlc_Buffering :
            newState = MediaPlayerState::BUFFERING;
            break;

        case libvlc_Playing  :
            newState = MediaPlayerState::PLAYING;
            break;

        case libvlc_Paused  :
            newState = MediaPlayerState::PAUSED;
            break;

        case libvlc_Stopped  :
            newState = MediaPlayerState::STOPPED;
            break;

        case libvlc_Ended  :
            newState = MediaPlayerState::ENDED;
            break;

        case libvlc_Error  :
            newState = MediaPlayerState::ERROR;
            break;

        default:
            newState = MediaPlayerState::NONE;
            break;
    }

    //Store for later use
    _state = newState;
    //Emit for rest of the world
    Q_EMIT playerStateChanged( new MediaPlayerState( newState )  );
}

qint64 VLCMediaPlayer::length() {
    return libvlc_media_player_get_length( _mediaPlayer );
}

qint64 VLCMediaPlayer::time() {
    return libvlc_media_player_get_time( _mediaPlayer );
}



