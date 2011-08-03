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
#include "PlayListEventBridge.h"

//Project includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Playlist/PlaylistNextItemSelectedEvent.h"
#include "../../Events/Playlist/PlaylistPreviousItemSelectedEvent.h"
#include "../../Events/Playlist/MediaItemContentChangedEvent.h"

using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

PlayListEventBridge::PlayListEventBridge( QObject *parent ) :
    QObject( parent ) {
    Aggregator::instance()->registerListener( PlaylistItemSelected, this );
}

PlayListEventBridge::~PlayListEventBridge() {
    Aggregator::instance()->unregisterListener( this );
}

bool PlayListEventBridge::event( QEvent* event ) {
    if( event->type() == PlaylistItemSelected ) {
        onPlaylistItemSelected( static_cast<PlaylistItemSelectedEvent*>( event ) );
    }

    return false;
}

void PlayListEventBridge::onPlaylistItemSelected( RedBullPlayer::Events::PlaylistItemSelectedEvent* e ) {
    if ( e->playlistItem() != 0 && e->playlistItem()->getMedia() != 0 ) {
        if( e->playlistItem()->getMedia()->mediaType().name() == "news" ||
                e->playlistItem()->getMedia()->mediaType().name() == "about" ||
                e->playlistItem()->getMedia()->mediaType().name() == "settings" ) {
            return;
        }

        emit playListItemSelected( e->playlistItem()->getMedia() ,
                                   e->skipDocuments(),
                                   e->fromSelectPreviousItemEvent(),
                                   e->overrideFunctionSelectedEvent() );
    }
}

void PlayListEventBridge::selectNextItem() {
    PlaylistNextItemSelectedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void PlayListEventBridge::selectPreviousItem() {
    PlaylistPreviousItemSelectedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void PlayListEventBridge::mediaItemContentChanged( QUuid mediaId ) {
    MediaItemContentChangedEvent e( mediaId );
    Aggregator::instance()->sendEvent( &e );
}
