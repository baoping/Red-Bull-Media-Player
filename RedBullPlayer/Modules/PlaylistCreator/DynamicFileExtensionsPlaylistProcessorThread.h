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
#ifndef DYNAMICFILEEXTENSIONSPLAYLISTPROCESSORTHREAD_H
#define DYNAMICFILEEXTENSIONSPLAYLISTPROCESSORTHREAD_H

#include <QThread>
#include <QStringList>
#include <QUuid>

#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "DynamicFileExtensionsPlaylistProcessor.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlaylistCreator {
            class DynamicFileExtensionsPlaylistProcessorThread : public QThread {
                    Q_OBJECT
                public:
                    DynamicFileExtensionsPlaylistProcessorThread( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *mediaLibrary,
                            QStringList addedFileExtensions, QStringList removedFileExtensions,
                            QObject* parent );
                    virtual void run();

                protected slots:
                    void mediaRemovedSlot( QString mediaId );

                private:
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *_mediaLibrary;
                    QStringList _addedFileExtensions;
                    QStringList _removedFileExtensions;
                    DynamicFileExtensionsPlaylistProcessor *_worker;
            };
        }
    }
}

#endif // DYNAMICFILEEXTENSIONSPLAYLISTPROCESSORTHREAD_H
