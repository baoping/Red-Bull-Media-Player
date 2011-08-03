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
#include "MediaTest.h"

#include <QString>

#include "MediaLibrary/IMediaRepository.h"
#include "MediaLibrary/IMedia.h"
#include "MediaLibrary/ITagRepository.h"
#include "MediaLibrary/ITag.h"
#include "MediaLibrary/IMediaDB.h"
#include "Shell.h"
#include "Error.h"
#include "TagRepository.h"
#include "MediaRepository.h"
#include "Media.h"
#include "Tag.h"
#include "MediaDB.h"
#include "FileSystem/PathResolver.h"
#include "Constants.h"
#include "ConfigManager.h"
#include "UuidCreator.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

void MediaTest::should_use_PathResolver() {
    MediaDB* db = new MediaDB( 0, ":memory:" );
    db->ensureSchema();
    db->ensureDefaultData();
    Error err;
    IMediaRepository* rep = db->mediaRepository();
    IMediaType* mediaType = db->mediaTypes().at( 0 );
    QVERIFY( rep != 0 );
    QUuid id = RedBullPlayer::Tools::UuidCreator::create();
    QDateTime date = QDateTime::currentDateTime();
    Media media( id,
                 Constants::MEDIADIRECTORY_PATH_PLACEHOLDER + "/fileName",
                 *mediaType,
                 1,
                 2,
                 "title",
                 "artist",
                 "genre",
                 "copyright",
                 "album",
                 "description",
                 "rating",
                 date,
                 "setting",
                 "url",
                 "language",
                 "publisher",
                 "encodedBy",
                 "artworkUrl",
                 "trackID",
                 true,
                 5,
                 1
               );
    QString expected = QDir::cleanPath( ConfigManager::instance()->getMediaDirectory() + "/fileName" );
    qDebug() <<  media.fileName() << "expected: " << expected;
    QVERIFY2( media.fileName() == expected, "Media fileName must use the PathResolver" );
}
