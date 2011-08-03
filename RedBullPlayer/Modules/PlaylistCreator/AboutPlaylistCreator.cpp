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
#include "AboutPlaylistCreator.h"

#include <QDebug>

// Project Includes
#include "../MediaLibrary/Media.h"
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Container/Constants.h"
#include "../../Interfaces/IChapter.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../UpdateServices/PlayerVersion.h"

#include "DummyPlaylist.h"


using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Tools;

AboutPlaylistCreator::AboutPlaylistCreator( QObject* parent ) : QObject( parent ) {
    createPlaylist();
}

void AboutPlaylistCreator::createPlaylist() {
    SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTPLAYLISTCREATOR, Q_FUNC_INFO );
    QPointer<IPlaylist> playlist = new DummyPlaylist();
    QString aboutTitle = ConfigManager::instance()->aboutTitle();
    QString aboutFile = ConfigManager::instance()->aboutFile();
    QString licenceTitle = ConfigManager::instance()->licenceTitle();
    QString licenceFile = ConfigManager::instance()->licenceFile();
    QString aboutPlaceHolder = Constants::PLAYLIST_CREATOR_IMAGE_DIR + Constants::PLAYLIST_CREATOR_ABOUT_PLACEHOLDER;
    QString licensePlaceHolder = Constants::PLAYLIST_CREATOR_IMAGE_DIR + Constants::PLAYLIST_CREATOR_LICENSE_PLACEHOLDER;
    playlist->addItem( createItem( aboutTitle, aboutFile, aboutPlaceHolder ) );
    playlist->addItem( createItem( licenceTitle, licenceFile, licensePlaceHolder ) );
    PlaylistCreatedEvent ea( playlist, PlaylistContentTypeChangedEvent::ABOUT );
    Aggregator::instance()->sendEvent( &ea );
}

IPlaylistItem *AboutPlaylistCreator::createItem( QString title, QString fileUrl, QString placeHolder ) {
    QImage coverImage( placeHolder );
    Media *media = new Media( QUuid(), "", MediaType( RedBullPlayer::Tools::UuidCreator::create(), "about" ) );
    media->setTitle( title );
    media->setCoverImage( coverImage );
    media->setDescription( getFileContent( fileUrl ) );
    return new DummyPlaylistItem( media );
}

QString AboutPlaylistCreator::getFileContent( QString fileName ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTPLAYLISTCREATOR, Q_FUNC_INFO );
    QString retVal = "";
    QFile file( fileName );

    if( QFile( fileName ).exists() && file.open( QFile::ReadOnly ) ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTPLAYLISTCREATOR, "getFileContent:true" );
        retVal = QString( file.readAll() );
        PlayerVersion::instance()->parseString( &retVal );
        file.close();
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTPLAYLISTCREATOR, "getFileContent:false" );
    }

    return retVal;
}
