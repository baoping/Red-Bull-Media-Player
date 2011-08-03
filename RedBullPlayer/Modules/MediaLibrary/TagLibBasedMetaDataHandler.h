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
#ifndef TAGLIBBASEDMETADATAHANDLER_H
#define TAGLIBBASEDMETADATAHANDLER_H

#include <QObject>
#include <QImage>

#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Interfaces/MediaLibrary/IMediaRepository.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class TagLibBasedMetaDataHandler : public QObject {
                    Q_OBJECT
                public:
                    TagLibBasedMetaDataHandler( IMediaLibrary* mediaLibrary, QObject* parent );
                    void setMetaDataIfAvailable( IMedia* media );
                    QImage getAlbumCoverFromMP3( QString fileName );
                private:
                    IMediaLibrary* _mediaLibrary;
            };
        }
    }
}
#endif // TAGLIBBASEDMETADATAHANDLER_H
