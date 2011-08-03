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
#ifndef DUMMYPLAYLIST_H
#define DUMMYPLAYLIST_H

#include "../../Interfaces/IPlaylist.h"
#include "../../Interfaces/IPlaylistItem.h"

#include "DummyPlaylistItem.h"

class DummyPlaylist :  public RedBullPlayer::Modules::Playlist::IPlaylist {
    public:
        DummyPlaylist();
        ~DummyPlaylist();

        virtual QList< RedBullPlayer::Modules::Playlist::IPlaylistItem *> items();

        void addItem( RedBullPlayer::Modules::Playlist::IPlaylistItem *item );
        void addItem( RedBullPlayer::Modules::MediaLibrary::IMedia *media );
        void insertItem( int atIndex, RedBullPlayer::Modules::MediaLibrary::IMedia *media );
        void addItems( QList<RedBullPlayer::Modules::Playlist::IPlaylistItem *> items );
        virtual RedBullPlayer::Modules::Playlist::IPlaylistItem * at( int index );
        void removeItemAt( int itemIndex );
        virtual int count();
        virtual int getItemIndexByGuid( QUuid mediaId );
        virtual void sortByNewFlagCreatedAt();
    private:
        QList< RedBullPlayer::Modules::Playlist::IPlaylistItem *> _items;
        static bool lessByNewFlagCreatedAtThen( RedBullPlayer::Modules::Playlist::IPlaylistItem* first, RedBullPlayer::Modules::Playlist::IPlaylistItem* second );
};

#endif // DUMMYPLAYLIST_H
