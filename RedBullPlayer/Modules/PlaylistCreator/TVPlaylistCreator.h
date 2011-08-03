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
#ifndef TVPLAYLISTCREATOR_H
#define TVPLAYLISTCREATOR_H

#include <QObject>
#include <QList>

// Project Includes
#include "../../Interfaces/IPlaylistCreator.h"
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlaylistCreator {

            class TVPlaylistCreator : public QObject {
                    Q_OBJECT
                public:
                    TVPlaylistCreator( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent );
                    ~TVPlaylistCreator();
                    virtual void createTVPlaylist();
                private: // Data
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;

            };

        }
    }
}
#endif // TVPLAYLISTCREATOR_H
