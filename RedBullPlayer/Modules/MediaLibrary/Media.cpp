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
#include "Media.h"

#include "../../Container/Error.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/FileSystem/PathResolver.h"
#include "../../Container/Constants.h"

#include "Tag.h"


using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

Media::Media(
    const QUuid& guid,
    const QString& fileName,
    const IMediaType& mediaType,
    qint64 length/* = 0*/,
    uint trackNumber/* = 0*/,
    const QString& title/* = QString::null*/,
    const QString& artist/* = QString::null*/,
    const QString& genre /*= QString::null*/,
    const QString& copyright /*= QString::null*/,
    const QString& album/* = QString::null*/,
    const QString& description /*= QString::null*/,
    const QString& rating /*= QString::null*/,
    const QDateTime& date /*= QString::null*/,
    const QString& setting /*= QString::null*/,
    const QString& url /*= QString::null*/,
    const QString& language /*= QString::null*/,
    const QString& publisher /*= QString::null*/,
    const QString& encodedBy /*= QString::null*/,
    const QString& artworkUrl /*= QString::null*/,
    const QString& trackID /*= QString::null*/,
    bool isNew /*= false */,
    uint fileSize /*= 0 */,
    qint64 startTime, /*= 0 */
    const QDateTime& createdAt,
    const QString& startAnimation,
    int animationFrameCount
) : _mediaType( mediaType ) {
    _createdAt = createdAt;
    _guid = guid;
    setFileName( fileName );
    _length = length;
    _trackNumber = trackNumber;
    _artist = artist;
    _title = title;
    _genre = genre;
    _copyright = copyright;
    _album = album;
    _description = description;
    _rating = rating;
    _date = date;
    _setting = setting;
    _url = url;
    _language = language;
    _publisher = publisher;
    _encodedBy = encodedBy;
    _artworkUrl = artworkUrl;
    _trackID = trackID;
    _isNew = isNew;
    _fileSize = fileSize;
    _startTime = startTime;
    _startAnimationFile = startAnimation;
    _animationFrameCount = animationFrameCount;
}

Media::Media( const IMedia& media ) : _mediaType( media.mediaType() ) {
    _createdAt = media.createdAt();
    _guid = media.guid();
    setFileName( media.fileName() );
    _length = media.length();
    _trackNumber = media.trackNumber();
    _artist = media.artist();
    _title = media.title();
    _genre = media.genre();
    _copyright = media.copyright();
    _album = media.album();
    _description = media.description();
    _rating = media.rating();
    _date = media.date();
    _setting = media.setting();
    _url = media.url();
    _language = media.language();
    _publisher = media.publisher();
    _encodedBy = media.encodedBy();
    _artworkUrl = media.artworkUrl();
    _trackID = media.trackID();
    _isNew = media.isNew();
    _fileSize = media.fileSize();
    _startTime = media.startTime();
    _startAnimationFile = media.startAnimationFile();
    _animationFrameCount = media.animationFrameCount();
}

QString Media::fileName() const  {
    return PathResolver::resolvePath( _fileName,
                                      ConfigManager::instance()->getMediaDirectory(),
                                      Constants::MEDIADIRECTORY_PATH_PLACEHOLDER );
}
void Media::setFileName( QString fileName ) {
    _fileName = PathResolver::createResolvablePath( fileName,
                ConfigManager::instance()->getMediaDirectory(),
                Constants::MEDIADIRECTORY_PATH_PLACEHOLDER );
}

QString Media::playListTitle() {
    if ( _mediaType.name() == Constants::MEDIA_TYPE_AUDIO_NAME ) {
        if ( !_artist.isNull() && !_artist.isEmpty() ) {
            return QString( "%1 (%2)" )
                   .arg( _title )
                   .arg( _artist );
        }
    }

    return _title;
}

Media::~Media() {}


