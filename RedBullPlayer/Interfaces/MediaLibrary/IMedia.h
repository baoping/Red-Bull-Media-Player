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
#ifndef IMEDIA_H
#define IMEDIA_H

// Qt includes
#include <QUuid>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QImage>

// Project includes
#include "ITag.h"
#include "IMediaType.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {
            class IMedia {
                public:
                    virtual ~IMedia() {}
                    virtual const QUuid& guid() const = 0;
                    virtual const IMediaType& mediaType() const = 0;

                    virtual QDateTime createdAt() const = 0;

                    virtual qint64 length() const = 0;
                    virtual void setLength( qint64 length ) = 0;

                    virtual uint trackNumber() const = 0;
                    virtual void setTrackNumber( uint trackNumber ) = 0;

                    virtual QString resolvableFileName() const = 0;
                    virtual QString fileName() const = 0;
                    virtual void setFileName( QString fileName ) = 0;

                    virtual QString title() const = 0;
                    virtual void setTitle( QString title ) = 0;

                    virtual QString playListTitle() = 0;

                    virtual QString artist() const = 0;
                    virtual void setArtist( QString artist ) = 0;

                    virtual QString genre() const = 0;
                    virtual void setGenre( QString genre ) = 0;

                    virtual QString copyright() const = 0;
                    virtual void setCopyright( QString copyright ) = 0;

                    virtual QString album() const = 0;
                    virtual void setAlbum( QString album ) = 0;

                    virtual QString description() const = 0;
                    virtual void setDescription( QString description ) = 0;

                    virtual QString rating() const = 0;
                    virtual void setRating( QString rating ) = 0;

                    virtual QDateTime date() const = 0;
                    virtual void setDate( QDateTime date ) = 0;

                    virtual QString setting() const = 0;
                    virtual void setSetting( QString setting ) = 0;

                    virtual QString url() const = 0;
                    virtual void setUrl( QString url ) = 0;

                    virtual QString language() const = 0;
                    virtual void setLanguage( QString language ) = 0;

                    virtual QString publisher() const = 0;
                    virtual void setPublisher( QString publisher ) = 0;

                    virtual QString encodedBy() const = 0;
                    virtual void setEncodedBy( QString encodedBy ) = 0;

                    virtual QString artworkUrl() const = 0;
                    virtual void setArtworkUrl( QString artworkUrl ) = 0;

                    virtual QString trackID() const = 0;
                    virtual void setTrackID( QString trackID ) = 0;

                    virtual QImage coverImage() const = 0;
                    virtual void setCoverImage( QImage coverImage ) = 0;

                    virtual QString startAnimationFile() const = 0;
                    virtual void setStartAnimationFile( QString fileName ) = 0;

                    virtual int animationFrameCount() const = 0;
                    virtual void setAnimationFrameCount( int frameCount ) = 0;

                    virtual bool isNew() const = 0;
                    virtual void setIsNew( bool isNew ) = 0;

                    virtual uint fileSize() const = 0;
                    virtual void setFileSize( uint fileSize ) = 0;

                    virtual qint64 startTime() const = 0;
                    virtual void setStartTime( qint64 startTime ) = 0;
            };
        }
    }
}

#endif // IMEDIA_H
