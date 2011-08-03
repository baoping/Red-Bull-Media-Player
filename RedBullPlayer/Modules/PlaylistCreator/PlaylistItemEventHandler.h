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
#ifndef PLAYLISTITEMEVENTHANDLER_H
#define PLAYLISTITEMEVENTHANDLER_H

#include <QObject>
#include <QHash>
#include <QFileInfo>
#include <QString>
#include <QMutex>

//Project includes
#include "../../Events/Playlist/PlaylistItemDropEvent.h"
#include "../../Events/Playlist/PlaylistItemRemovedEvent.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/TaskManager/ITask.h"
#include "../../Events/TaskManager/TaskFinishedEvent.h"
#include "../../Events/TaskManager/TaskCanceledEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlaylistCreator {

            class PlaylistItemEventHandler : public QObject {
                    Q_OBJECT

                public:
                    PlaylistItemEventHandler( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent );
                    ~PlaylistItemEventHandler();

                    static void processNewMediaFile( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QFileInfo file, bool userAdded );

                protected:
                    bool event( QEvent *event );
                    void onPlaylistItemDrop( RedBullPlayer::Events::PlaylistItemDropEvent *e );
                    void onPlaylistItemRemoved( RedBullPlayer::Events::PlaylistItemRemovedEvent *e );

                    void onTaskExecuteResponse( RedBullPlayer::Events::TaskFinishedEvent* e );
                    void onTaskCancelResponse( RedBullPlayer::Events::TaskCanceledEvent* e );

                    void removeMediaItem( QUuid mediaId );

                private:
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;

                    QHash<QUuid, RedBullPlayer::Modules::TaskManager::ITask*>* _copyTasks;
                    QHash<QUuid, RedBullPlayer::Modules::TaskManager::ITask*>* _moveTasks;

                    void fileCopySuccessful( QString newFileName );
                    void fileCopyFailed( QString oldFileName );

                    void fileMoveSuccessful( QString newFileName );
                    void fileMoveFailed( QString oldFileName );

                    void processNewMediaFile( QFileInfo file, bool userAdded );

                    static void mediaDiscovered( QString mediaGuid, int source );
                    static void mediaContentChanged( QString mediaGuid );

                    static QMutex* _processNewMediaFileMutex;
            };
        }
    }
}

#endif // PLAYLISTITEMEVENTHANDLER_H
