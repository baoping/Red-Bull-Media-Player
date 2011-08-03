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
#include "VLCEventBridge.h"

// QT includes
#include <QDebug>

// Project includes
#include "VLCMediaPlayer.h"
#include "VLCMediaEventBridge.h"

void VLCEventBridge::onVLCEvent( const libvlc_event_t * event, void * playerInstance ) {
    VLCMediaPlayer* p = ( VLCMediaPlayer* )playerInstance;

    switch( event->type ) {
        case libvlc_MediaPlayerPaused: {
                p->emitPaused();
                break;
            }

        case libvlc_MediaPlayerBuffering: {
                p->emitBuffering();
                break;
            }

        case libvlc_MediaPlayerPlaying: {
                p->emitPlaying();
                break;
            }

        case libvlc_MediaPlayerOpening: {
                p->emitOpening();
                break;
            }

        case libvlc_MediaPlayerStopped: {
                p->emitStopped();
                break;
            }

        case libvlc_MediaPlayerEndReached: {
                p->emitEnded();
                break;
            }

        case libvlc_MediaPlayerPausableChanged: {
                int newPausable = event->u.media_player_pausable_changed.new_pausable;
                p->emitPausable( newPausable == 1 );
                break;
            }

        case libvlc_MediaPlayerSeekableChanged: {
                int newSeekable = event->u.media_player_seekable_changed.new_seekable;
                p->emitSeekable( newSeekable == 1 );
                break;
            }

        case libvlc_MediaPlayerPositionChanged: {
                float newPosition = event->u.media_player_position_changed.new_position;
                p->emitPositionChanged( newPosition );
                break;
            }

        case libvlc_MediaPlayerTimeChanged: {
                libvlc_time_t newTime = event->u.media_player_time_changed.new_time;
                p->emitTimeChanged( newTime );
                break;
            }

        case libvlc_MediaPlayerEncounteredError: {
                p->emitEncounteredError();
                break;
            }

        case libvlc_MediaPlayerMediaChanged: {
                libvlc_media_t* newMedia = event->u.media_player_media_changed.new_media;
                p->handleMediaChanged( newMedia );
                break;
            }

        case libvlc_MediaPlayerSnapshotTaken: {
                char* pszFilename = event->u.media_player_snapshot_taken.psz_filename;
                QString fileName( pszFilename );
                p->emitSnapshotTaken( fileName );
                break;
            }

        default:
            qDebug() << "Unhandled EventType: " << libvlc_event_type_name( event->type );
    }
}

VLCEventBridge::VLCEventBridge( VLCMediaPlayer* mediaplayer ) {
    Q_ASSERT( mediaplayer != 0 );
    Q_ASSERT( mediaplayer->_mediaPlayer != 0 );
    _mediaPlayer = mediaplayer;
    _eventManager = libvlc_media_player_event_manager( _mediaPlayer->_mediaPlayer );

    if ( !_eventManager )
        throw libvlc_errmsg();
}

VLCEventBridge::~VLCEventBridge() {
    //detachEvents();
}

void VLCEventBridge::attachEvent( libvlc_event_type_t event_type ) {
    int ret = libvlc_event_attach( _eventManager,
                                   event_type,
                                   onVLCEvent,
                                   _mediaPlayer );

    if ( ret != 0 ) {
        throw libvlc_errmsg();
    }
}

void VLCEventBridge::detachEvent( libvlc_event_type_t event_type ) {
    libvlc_event_detach( _eventManager,
                         event_type,
                         onVLCEvent,
                         _mediaPlayer );
}

void VLCEventBridge::attachEvents() {
    //attachEvent(libvlc_MediaPlayerPaused);
    //attachEvent(libvlc_MediaPlayerBuffering);
    attachEvent( libvlc_MediaPlayerPlaying );
    //attachEvent(libvlc_MediaPlayerOpening);
    //attachEvent(libvlc_MediaPlayerStopped);
    //attachEvent(libvlc_MediaPlayerEndReached);
    //attachEvent(libvlc_MediaPlayerEncounteredError);
    //attachEvent(libvlc_MediaPlayerSeekableChanged);
    //attachEvent(libvlc_MediaPlayerPausableChanged);
    attachEvent( libvlc_MediaPlayerPositionChanged );
    attachEvent( libvlc_MediaPlayerTimeChanged );
    attachEvent( libvlc_MediaPlayerMediaChanged );
    attachEvent( libvlc_MediaPlayerSnapshotTaken );
}
void VLCEventBridge::detachEvents() {
    //detachEvent(libvlc_MediaPlayerPaused);
    //detachEvent(libvlc_MediaPlayerBuffering);
    detachEvent( libvlc_MediaPlayerPlaying );
    //detachEvent(libvlc_MediaPlayerOpening);
    //detachEvent(libvlc_MediaPlayerStopped);
    //detachEvent(libvlc_MediaPlayerEndReached);
    //detachEvent(libvlc_MediaPlayerEncounteredError);
    //detachEvent(libvlc_MediaPlayerSeekableChanged);
    //detachEvent(libvlc_MediaPlayerPausableChanged);
    detachEvent( libvlc_MediaPlayerPositionChanged );
    detachEvent( libvlc_MediaPlayerTimeChanged );
    detachEvent( libvlc_MediaPlayerMediaChanged );
    detachEvent( libvlc_MediaPlayerSnapshotTaken );
}
