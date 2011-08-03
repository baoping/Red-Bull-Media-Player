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
#include "VLCMediaEventBridge.h"

// QT includes
#include <QDebug>

// Project includes
#include "VLCMediaPlayer.h"

void VLCMediaEventBridge::onVLCEvent( const libvlc_event_t * event, void * mediaPlayerInstance ) {
    VLCMediaPlayer* p = ( VLCMediaPlayer* )mediaPlayerInstance;

    switch( event->type ) {
        case libvlc_MediaStateChanged: {
                libvlc_state_t newState = event->u.media_state_changed.new_state;
                p->emitPlayerStateChanged( newState );
                break;
            }

        default:
            qDebug() << "Unhandled EventType: " << libvlc_event_type_name( event->type );
    }
}

VLCMediaEventBridge::VLCMediaEventBridge( VLCMediaPlayer* mediaPlayer, libvlc_media_t* media ) {
    Q_ASSERT( mediaPlayer != 0 );
    Q_ASSERT( mediaPlayer->_mediaPlayer != 0 );
    _mediaPlayer = mediaPlayer;
    _eventManager = libvlc_media_event_manager( media );

    if ( !_eventManager )
        throw libvlc_errmsg();
}

VLCMediaEventBridge::~VLCMediaEventBridge() {
    //detachEvents();
}

void VLCMediaEventBridge::attachEvent( libvlc_event_type_t event_type ) {
    int ret = libvlc_event_attach( _eventManager,
                                   event_type,
                                   onVLCEvent,
                                   _mediaPlayer );

    if ( ret != 0 ) {
        throw libvlc_errmsg();
    }
}

void VLCMediaEventBridge::detachEvent( libvlc_event_type_t event_type ) {
    libvlc_event_detach( _eventManager,
                         event_type,
                         onVLCEvent,
                         _mediaPlayer );
}

void VLCMediaEventBridge::attachEvents() {
    attachEvent( libvlc_MediaStateChanged );
}
void VLCMediaEventBridge::detachEvents() {
    detachEvent( libvlc_MediaStateChanged );
}

