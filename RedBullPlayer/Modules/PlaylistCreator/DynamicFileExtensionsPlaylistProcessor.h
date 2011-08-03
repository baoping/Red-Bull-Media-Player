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
#ifndef DYNAMICFILEEXTENSIONSPLAYLISTPROCESSOR_H
#define DYNAMICFILEEXTENSIONSPLAYLISTPROCESSOR_H

#include <QObject>
#include <QList>
#include <QPointer>
#include <QStringList>

#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlaylistCreator {
            class DynamicFileExtensionsPlaylistProcessor : public QObject {
                    Q_OBJECT

                public:
                    DynamicFileExtensionsPlaylistProcessor( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *mediaLibrary,
                                                            QStringList addedFileExtensions, QStringList removedFileExtensions,
                                                            QObject* parent );
                    virtual void processPlaylist();

                signals:
                    void mediaRemoved( QString meidaId );

                private:
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *_mediaLibrary;
                    QStringList _addedFileExtensions;
                    QStringList _removedFileExtensions;
            };
        }
    }
}

#endif // DYNAMICFILEEXTENSIONSPLAYLISTPROCESSOR_H
