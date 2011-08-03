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
#ifndef PODCASTWORKER_H
#define PODCASTWORKER_H

//Qt includes
#include <QObject>
#include <QUuid>
#include <QQueue>
#include <QHash>

//Project includes
#include "../../Interfaces/FileSystem/IStorageReservation.h"
#include "../../Interfaces/MediaLibrary/IContent.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../ToolLib/Network/DownloadUtil.h"
#include "../../ToolLib/TaskManager/FileTask.h"

//Module includes
#include "PodCastTask.h"
#include "PodCastCache.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class PodCastWorker : public QObject {
                    Q_OBJECT

                public:
                    PodCastWorker( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary,
                                   RedBullPlayer::Modules::StyleFactory::IStyleFactory* styleFactory,
                                   PodCastCache* podCastCache,
                                   QObject* parent = 0 );
                    ~PodCastWorker();

                public slots:
                    void parseStarted();
                    void parseFinished();

                protected:
                    bool event( QEvent* event );

                private:
                    void bootstrap();

                    void initiateDownload( QString contentId );
                    void cancelDownload( QString contentId );
                    void taskStarted( QUuid taskId );
                    void taskExecuted( QUuid taskId );
                    void taskProgress( QUuid taskId, int progress );
                    void taskCanceled( QUuid taskId );
                    void skipContent( QString contentId );
                    void banContent( QString contentId );
                    void showDiskSpaceError();

                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory* _styleFactory;
                    PodCastCache* _podCastCache;
                    bool _useQueue;

                    QQueue<QString>* _downloadQueue;

                    RedBullPlayer::Tools::Network::DownloadUtil* _rssFeedImageDownloadUtil;

                    QHash<QString, PodCastTask*>* _podCastTasks;
                    QHash<QString, RedBullPlayer::Tools::IStorageReservation*>* _storageReservations;
                    QHash<QString, RedBullPlayer::Modules::TaskManager::FileTask*>* _moveTasks;

                    bool _internetAvailable;

                private slots:
                    void rssFeedImagedownloadFinished( QString, QString, QString );
                    void rssFeedImagedownloadFailed( QString, int, QString );
            };
        }
    }
}

#endif // PODCASTWORKER_H
