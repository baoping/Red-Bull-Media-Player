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
#include "SettingsPlaylistCreator.h"


#include <QDebug>

// Project Includes
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../UpdateServices/PlayerVersion.h"
#include "../MediaLibrary/Media.h"
#include "DummyPlaylist.h"


using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Tools;

SettingsPlaylistCreator::SettingsPlaylistCreator( QObject* parent ) : QObject( parent ) {
    createPlaylist();
}


void SettingsPlaylistCreator::createPlaylist() {
    QPointer<IPlaylist> playlist = new DummyPlaylist();
    QString generalSettingsTitle = ConfigManager::instance()->generalSettingsTitle();
    QString mediaSettingsTitle = ConfigManager::instance()->mediaSettingsTitle();
    QString generalSettingsPlaceHolder = Constants::PLAYLIST_CREATOR_IMAGE_DIR + Constants::PLAYLIST_CREATOR_GENERAL_SETTINGS_PLACEHOLDER;
    QString mediaSettingsPlaceHolder = Constants::PLAYLIST_CREATOR_IMAGE_DIR + Constants::PLAYLIST_CREATOR_MEDIA_SETTINGS_PLACEHOLDER;
    QString generalSettingsFileUrl = Constants::SETTINGS_DETAIL_VIEW_TEXT_DIR + Constants::SETTINGS_DETAIL_VIEW_GENERAL_SETUP_FILE;
    QString mediaSettinsFileUrl = Constants::SETTINGS_DETAIL_VIEW_TEXT_DIR + Constants::SETTINGS_DETAIL_VIEW_MEDIA_SETUP_FILE;
    QString generalSettingsIconUrl = Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR + Constants::SETTINGS_DETAIL_VIEW_GENERAL_BACKGROUND;
    QString mediaSettingsIconUrl = Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR + Constants::SETTINGS_DETAIL_VIEW_MEDIA_BACKGROUND;
    playlist->addItem( createItem( generalSettingsTitle, generalSettingsFileUrl,
                                   generalSettingsPlaceHolder, generalSettingsIconUrl ) );
    playlist->addItem( createItem( mediaSettingsTitle, mediaSettinsFileUrl,
                                   mediaSettingsPlaceHolder, mediaSettingsIconUrl ) );
    PlaylistCreatedEvent ea( playlist, PlaylistContentTypeChangedEvent::SETTINGS );
    Aggregator::instance()->sendEvent( &ea );
}

IPlaylistItem* SettingsPlaylistCreator::createItem( QString title, QString fileUrl, QString placeHolder,
        QString iconUrl ) {
    QImage coverImage( placeHolder );
    Media *media = new Media( QUuid(), "", MediaType( RedBullPlayer::Tools::UuidCreator::create(), "settings" ) );
    media->setTitle( title );
    media->setCoverImage( coverImage );
    media->setDescription( fileUrl );
    media->setArtworkUrl( iconUrl );
    return new DummyPlaylistItem( media );
}

