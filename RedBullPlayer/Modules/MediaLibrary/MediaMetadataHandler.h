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
#ifndef MEDIAMETADATAHANDLER_H
#define MEDIAMETADATAHANDLER_H

#include "../../Interfaces/MediaLibrary/IMediaMetadataHandler.h"
// Qt includes
#include <QObject>
#include <QString>
#include <QList>
#include <QPointer>

// Project includes
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/ISnapshotCreationRequest.h"
#include "../../Events/SnapshotCreator/SnapshotCreatedEvent.h"
#include "../../Events/MediaLibrary/CoverImageRequestedEvent.h"
#include "../../Events/MediaLibrary/CancelCoverImageCreationEvent.h"
#include "../../Events/MediaLibrary/ChapterImageRequestEvent.h"
#include "TagLibBasedMetaDataHandler.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class MediaMetadataHandler : public QObject, public IMediaMetadataHandler {
            public:
                MediaMetadataHandler( IMediaLibrary* mediaLibrary, QObject* parent );
                virtual void setMetaDataUsingTagLib( IMedia* media );
            protected:
                virtual void generateCoverImage( IMedia* media, float position = -1.0F, uint second = 70 );
                virtual bool event( QEvent* event );
                virtual void onSnapshotCreated( RedBullPlayer::Events::SnapshotCreatedEvent* ev );
                virtual void onCoverImageRequested( RedBullPlayer::Events::CoverImageRequestedEvent* ev );
                virtual void onChapterImageRequest( RedBullPlayer::Events::ChapterImageRequestEvent* ev );
                virtual void onCancelCoverImageCreation( RedBullPlayer::Events::CancelCoverImageCreationEvent *ev );

            private:
                /* Handle Request data */
                class MediaRequestObj {
                public:
                    MediaRequestObj( QUuid mediaId, QUuid requestId, float position, bool isCoverImage ) {
                        _mediaId = mediaId;
                        _requestId = requestId;
                        _position = position;
                        _isCoverImage = isCoverImage;
                    }
                    QUuid mediaId() const {
                        return _mediaId;
                    }
                    QUuid requestId() const  {
                        return _requestId;
                    }
                    float position() const {
                        return _position;
                    }
                    bool isCoverImage() const {
                        return _isCoverImage;
                    }


                private:
                    QUuid _mediaId;
                    QUuid _requestId;
                    float _position;
                    bool _isCoverImage;
                };

            private:
                IMediaLibrary* _mediaLibrary;
                QList<MediaRequestObj*>* _sentRequests;
                TagLibBasedMetaDataHandler* _tagLibMetaData;
            };
        }
    }
}
#endif // MEDIAMETADATAHANDLER_H
