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
#ifndef IPLAYLIST_OLD_H
#define IPLAYLIST_OLD_H

// Project Includes
#include "IPlaylistItem.h"
#include "MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Playlist {
            class IPlaylist : public QObject {
                    Q_OBJECT

                public:
                    virtual ~IPlaylist() {}
                    virtual QList<IPlaylistItem *> items() = 0;
                    virtual void addItem( RedBullPlayer::Modules::Playlist::IPlaylistItem *item ) = 0;
                    virtual void addItem( RedBullPlayer::Modules::MediaLibrary::IMedia *media ) = 0;
                    virtual void insertItem( int atIndex, RedBullPlayer::Modules::MediaLibrary::IMedia *media ) = 0;
                    virtual void addItems( QList<RedBullPlayer::Modules::Playlist::IPlaylistItem *> items ) = 0;
                    virtual RedBullPlayer::Modules::Playlist::IPlaylistItem * at( int index ) = 0;
                    virtual void removeItemAt( int itemIndex ) = 0;
                    virtual int count() = 0;
                    virtual int getItemIndexByGuid( QUuid mediaId ) = 0;
                    virtual void sortByNewFlagCreatedAt() = 0;
            };
        }
    }
}

#endif // IPLAYLIST_OLD_H
