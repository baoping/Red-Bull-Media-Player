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
#include "TVPlaylistCreator.h"

#include <QDebug>
#include <QDir>
#include <QImage>

// Project Includes
#include "../../ToolLib/Configuration/ConfigurationProvider.h"
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../MediaLibrary/Media.h"
#include "../MediaLibrary/MediaType.h"
#include "DummyPlaylist.h"
#include "DummyPlaylistItem.h"
#include "DummyChapter.h"

using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;

using namespace RedBullPlayer::Tools::Configuration;

TVPlaylistCreator::TVPlaylistCreator(  RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent ) : QObject( parent ) {
    Q_ASSERT( mediaLibrary != 0 );
    _mediaLibrary = mediaLibrary;
    createTVPlaylist();
}

TVPlaylistCreator::~TVPlaylistCreator() {
}


void TVPlaylistCreator::createTVPlaylist() {
    QPointer<IPlaylist> playlist = new DummyPlaylist();
    int i;
    int count = ConfigurationProvider::instance()->tv()->count();

    for( i = 0; i < count; i++ ) {
        QString name = ConfigurationProvider::instance()->tv()->nameAt( i );
        QUrl url = ConfigurationProvider::instance()->tv()->urlAt( i );
        QImage image = ConfigurationProvider::instance()->tv()->imageAt( i );
        QString animationFile = ConfigurationProvider::instance()->tv()->startAnimationFileAt( i );
        int frameCount = ConfigurationProvider::instance()->tv()->animationFrameCountAt( i );
        Media *media = new Media(
            QUuid(),
            url.toString(),
            *_mediaLibrary->mediaDB()->tvChannelType(),
            100,
            100,
            name,
            "" );

        if ( !image.isNull() ) {
            media->setCoverImage( image );
        }

        media->setStartAnimationFile( animationFile );
        media->setAnimationFrameCount( frameCount );
        DummyPlaylistItem *item = new DummyPlaylistItem( media );
        playlist->addItem( item );
    }

    PlaylistCreatedEvent ea( playlist, PlaylistContentTypeChangedEvent::TV );
    Aggregator::instance()->sendEvent( &ea );
}
