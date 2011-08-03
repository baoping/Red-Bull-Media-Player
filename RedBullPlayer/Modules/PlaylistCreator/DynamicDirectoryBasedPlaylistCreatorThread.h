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
#ifndef DYNAMICDIRECTORYBASEDPLAYLISTCREATORTHREAD_H
#define DYNAMICDIRECTORYBASEDPLAYLISTCREATORTHREAD_H

#include <QThread>
#include <QString>
#include <QUuid>

#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/IPlaylist.h"

#include "DynamicDirectoryBasedPlaylistCreator.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlaylistCreator {
            class DynamicDirectoryBasedPlaylistCreatorThread : public QThread {
                    Q_OBJECT
                public:
                    DynamicDirectoryBasedPlaylistCreatorThread( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent );
                    ~DynamicDirectoryBasedPlaylistCreatorThread();
                    virtual void run();
                private:
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    DynamicDirectoryBasedPlaylistCreator* _dynDirPlaylistCreator;

                protected slots:
                    void playlistCreated( RedBullPlayer::Modules::Playlist::IPlaylist * playlist );
                    void mediaDiscovered( QString mediaGuid, int source );
                    void mediaVanished( QString mediaGuid );
                    void mediaRemoved( QString mediaGuid );
                    void playlistGenerationFinished();
                    void startupPlaylistGenerationStarted();
                    void startupPlaylistGenerationFinished();
            };
        }
    }
}
#endif // DYNAMICDIRECTORYBASEDPLAYLISTCREATORTHREAD_H
