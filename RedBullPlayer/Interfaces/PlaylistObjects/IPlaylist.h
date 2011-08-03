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
#ifndef IPLAYLIST_H
#define IPLAYLIST_H

// QT includes
#include <QObject>
#include <QList>

// Project Inlcudes
#include "../../Container/Plugin.h"
#include "../../Container/Error.h"
#include "IPlaylistItem.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Playlist {
            namespace PlaylistObjects {
                class IPlaylist {
                    public:
                        virtual ~IPlaylist() {}
                        virtual QString name() = 0;
                        virtual void setName( QString name );
                        virtual QList<IPlaylistItem*> items() = 0;
                        virtual IPlaylistItem* itemAt( uint index );
                        virtual void saveToFile( QString fileName, RedBullPlayer::Container::Error* error ) = 0;
                        virtual void clear() = 0;
                        virtual void addItem( IPlaylistItem* item ) = 0;
                        virtual void removeItem( IPlaylistItem* item ) = 0;
                        virtual void removeItemAt( int index ) = 0;
                        virtual void moveItemToPosition( IPlaylistItem* itemToMove, uint newPosition, RedBullPlayer::Container::Error* error ) = 0;
                        virtual bool editable() = 0;
                        virtual void setEditable( bool editable ) = 0;
                        virtual bool modified() = 0;
                };
            }
        }
    }
}



#endif // IPLAYLIST_H
