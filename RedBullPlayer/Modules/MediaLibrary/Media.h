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
#ifndef MEDIA_H
#define MEDIA_H

// Qt includes
#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QPixmap>

// Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Interfaces/MediaLibrary/IMediaType.h"
#include "../../Interfaces/MediaLibrary/ITag.h"
#include "MediaType.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {

            class Media : public IMedia {
                public:
                    Media(
                        const QUuid& guid,
                        const QString& fileName,
                        const IMediaType& mediaType,
                        qint64 length = 0,
                        uint trackNumber = 0,
                        const QString& title = QString::null,
                        const QString& artist = QString::null,
                        const QString& genre = QString::null,
                        const QString& copyright = QString::null,
                        const QString& album = QString::null,
                        const QString& description = QString::null,
                        const QString& rating = QString::null,
                        const QDateTime& date = QDateTime(),
                        const QString& setting = QString::null,
                        const QString& url = QString::null,
                        const QString& language = QString::null,
                        const QString& publisher = QString::null,
                        const QString& encodedBy = QString::null,
                        const QString& artworkUrl = QString::null,
                        const QString& trackID = QString::null,
                        bool isNew = false,
                        uint fileSize = 0,
                        qint64 startTime = 0,
                        const QDateTime& createdAt = QDateTime(),
                        const QString& startAnimation = QString::null,
                        int animationFrameCount = 0

                    );
                    Media( const IMedia& media );

                    virtual ~Media();

                    virtual const QUuid& guid() const {
                        return _guid;
                    }

                    virtual const IMediaType& mediaType() const {
                        return _mediaType;
                    }

                    virtual QDateTime createdAt() const {
                        return _createdAt;
                    }

                    virtual qint64 length() const {
                        return _length;
                    }

                    virtual uint trackNumber() const  {
                        return _trackNumber;
                    }

                    virtual QString resolvableFileName() const {
                        return _fileName;
                    }

                    virtual QString fileName() const;

                    virtual QString title() const  {
                        return _title;
                    }

                    virtual QString playListTitle();

                    virtual QString artist() const  {
                        return _artist;
                    }

                    virtual QString genre() const  {
                        return _genre;
                    }

                    virtual QString copyright() const  {
                        return _copyright;
                    }

                    virtual QString album() const  {
                        return _album;
                    }

                    virtual QString description() const  {
                        return _description;
                    }

                    virtual QString rating() const  {
                        return _rating;
                    }

                    virtual QDateTime date() const  {
                        return _date;
                    }

                    virtual QString setting() const  {
                        return _setting;
                    }

                    virtual QString url() const  {
                        return _url;
                    }

                    virtual QString language() const  {
                        return _language;
                    }

                    virtual QString publisher() const  {
                        return _publisher;
                    }

                    virtual QString encodedBy() const  {
                        return _encodedBy;
                    }

                    virtual QString artworkUrl() const  {
                        return _artworkUrl;
                    }

                    virtual QString trackID() const  {
                        return _trackID;
                    }

                    virtual bool isNew() const {
                        return _isNew;
                    }

                    virtual uint fileSize() const {
                        return _fileSize;
                    }

                    virtual qint64 startTime() const {
                        return _startTime;
                    }

                    virtual void setFileName( QString fileName );

                    virtual void setLength( qint64 length ) {
                        _length = length;
                    }

                    virtual void setTrackNumber( uint trackNumber ) {
                        _trackNumber = trackNumber;
                    }

                    virtual void setTitle( QString title ) {
                        _title = title;
                    }

                    virtual void setArtist( QString artist ) {
                        _artist = artist;
                    }

                    virtual void setGenre( QString genre ) {
                        _genre = genre;
                    }

                    virtual void setCopyright( QString copyright ) {
                        _copyright = copyright;
                    }

                    virtual void setAlbum( QString album ) {
                        _album = album;
                    }

                    virtual void setDescription( QString description ) {
                        _description = description;
                    }

                    virtual void setRating( QString rating ) {
                        _rating = rating;
                    }

                    virtual void setDate( QDateTime date ) {
                        _date = date;
                    }

                    virtual void setSetting( QString setting ) {
                        _setting = setting;
                    }

                    virtual void setUrl( QString url ) {
                        _url = url;
                    }

                    virtual void setLanguage( QString language ) {
                        _language = language;
                    }

                    virtual void setPublisher( QString publisher ) {
                        _publisher = publisher;
                    }

                    virtual void setEncodedBy( QString encodedBy ) {
                        _encodedBy = encodedBy;
                    }

                    virtual void setArtworkUrl( QString artworkUrl ) {
                        _artworkUrl = artworkUrl;
                    }

                    virtual void setTrackID( QString trackID ) {
                        _trackID = trackID;
                    }

                    virtual QImage coverImage() const {
                        return _coverImage;
                    }
                    virtual void setCoverImage( QImage coverImage ) {
                        _coverImage = coverImage;
                    }

                    virtual QString startAnimationFile() const {
                        return _startAnimationFile;
                    }

                    virtual void setStartAnimationFile( QString fileName ) {
                        _startAnimationFile = fileName;
                    }

                    virtual int animationFrameCount() const {
                        return _animationFrameCount;
                    }

                    virtual void setAnimationFrameCount( int frameCount ) {
                        _animationFrameCount = frameCount;
                    }

                    virtual void setIsNew( bool isNew ) {
                        _isNew = isNew;
                    }

                    virtual void setFileSize( uint fileSize ) {
                        _fileSize = fileSize;
                    }

                    virtual void setStartTime( qint64 startTime ) {
                        _startTime = startTime;
                    }

                private: // Methods
                    Media();

                private:
                    QUuid _guid;
                    MediaType _mediaType;
                    qint64 _length;
                    int _trackNumber;
                    uint _fileSize;
                    qint64 _startTime;
                    bool _isNew;
                    QDateTime _createdAt;
                    QString _fileName;
                    QString _title;
                    QString _artist;
                    QString _genre;
                    QString _copyright;
                    QString _album;
                    QString _description;
                    QString _rating;
                    QDateTime _date;
                    QString _setting;
                    QString _url;
                    QString _language;
                    QString _publisher;
                    QString _encodedBy;
                    QString _artworkUrl;
                    QString _trackID;
                    QImage _coverImage;
                    QString _startAnimationFile;
                    int _animationFrameCount;

            };
        }
    }
}
#endif // MEDIA_H
