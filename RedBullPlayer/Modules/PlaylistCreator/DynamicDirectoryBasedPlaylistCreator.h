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
#ifndef DYNAMICDIRECTORYBASEDPLAYLISTCREATOR_H
#define DYNAMICDIRECTORYBASEDPLAYLISTCREATOR_H

#include <QObject>
#include <QList>
#include <QPointer>

#include "../../Interfaces/IPlaylistCreator.h"
#include "../../Interfaces/IPlaylist.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/MediaLibrary/CoverImageRequestedEvent.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/MediaLibrary/CancelCoverImageCreationEvent.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlaylistCreator {
            class DynamicDirectoryBasedPlaylistCreator : public QObject {
                    Q_OBJECT

                public:
                    DynamicDirectoryBasedPlaylistCreator( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent );
                    virtual void createPlaylistFromBaseDirectory();

                private:
                    QList<QPair<QUuid, QString>*>* readMediaFromMediaLibrary(  );
                    void readNewMediaFromFS( QList<QPair<QUuid, QString>*>* discoveredMedia );
                    void filterVanishedMedia( QList<QPair<QUuid, QString>*>* discoveredMedia );
                    void processNewMediaFile( QFileInfo file, bool userAdded );
                    bool isInWhiteList( QString whiteListConfigUrl, QFileInfo file );
                    void initBaseDir();
                    void initDeleteDir();
                    void mkDirIfNotExists( QString dir );

                    bool findInPairList( QString searchText, QList<QPair<QUuid, QString>*>* list );

                private: // Data
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    QList<QString> _mediaDirectories;
                    QString _deleteDir;

                signals:
                    void playlistCreated( RedBullPlayer::Modules::Playlist::IPlaylist* playlist );
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
#endif // DYNAMICDIRECTORYBASEDPLAYLISTCREATOR_H
