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
#include "DummyPlaylist.h"

// Qt Includes
#include <QDebug>
#include <QUuid>
#include <QDateTime>

#include "DummyPlaylistItem.h"

using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::Modules::MediaLibrary;

DummyPlaylist::DummyPlaylist() : IPlaylist() {
}

DummyPlaylist::~DummyPlaylist() {
    while ( !_items.isEmpty() ) {
        delete _items.takeFirst();
    }
}

QList<IPlaylistItem *> DummyPlaylist::items() {
    return _items;
}

void DummyPlaylist::addItem( IPlaylistItem *item ) {
    _items.append( item );
}

void DummyPlaylist::addItem( IMedia *media ) {
    DummyPlaylistItem *item = new DummyPlaylistItem( media );
    _items.append( item );
}

void DummyPlaylist::insertItem( int atIndex, IMedia *media ) {
    DummyPlaylistItem *item = new DummyPlaylistItem( media );
    _items.insert( atIndex, item );
}

void DummyPlaylist::addItems( QList<IPlaylistItem *> items ) {
    _items.append( items );
}

IPlaylistItem * DummyPlaylist::at( int index ) {
    if( index >= 0 && index < _items.count() ) {
        return _items.at( index );
    }

    return NULL;
}

void DummyPlaylist::removeItemAt( int itemIndex ) {
    _items.removeAt( itemIndex );
}

int DummyPlaylist::count() {
    return _items.count();
}

int DummyPlaylist::getItemIndexByGuid( QUuid mediaId ) {
    int itemIndex = -1;

    for( int i = 0; i < _items.count(); i++ ) {
        if( _items.at( i )->getMedia()->guid() == mediaId ) {
            itemIndex = i;
            break;
        }
    }

    return itemIndex;
}

void DummyPlaylist::sortByNewFlagCreatedAt() {
    if ( _items.count() == 0 ) {
        return;
    }

    qSort ( _items.begin(), _items.end(), lessByNewFlagCreatedAtThen );
}

bool DummyPlaylist::lessByNewFlagCreatedAtThen( IPlaylistItem* first, IPlaylistItem* second ) {
    if( ( first->getMedia()->createdAt().secsTo( second->getMedia()->createdAt() ) ) < 0 )
        return false;
    else
        return true;
}



