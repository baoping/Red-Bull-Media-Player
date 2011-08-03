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
#include "NewsPlaylistCreator.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QDir>
#include <QList>

// Project Includes
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Interfaces/MediaLibrary/INews.h"
#include "../../Interfaces/MediaLibrary/INewsImage.h"
#include "../MediaLibrary/Media.h"
#include "../MediaLibrary/MediaType.h"

#include "DummyPlaylist.h"
#include "DummyPlaylistItem.h"
#include "DummyChapter.h"


using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

NewsPlaylistCreator::NewsPlaylistCreator( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent ) : QObject( parent ) {
    _mediaLibrary = mediaLibrary;
    Aggregator::instance()->registerListener( NewsItemChanged, this );
    Aggregator::instance()->registerListener( RssFeedPlaylistCreationRequest, this );
}

bool NewsPlaylistCreator::event ( QEvent * e ) {
    if ( e->type() == RssFeedPlaylistCreationRequest ) {
        onRssFeedPlaylistCreationRequest( static_cast<RssFeedPlaylistCreationRequestEvent*>( e ) );
    }

    return false;
}

void NewsPlaylistCreator::onRssFeedPlaylistCreationRequest( RedBullPlayer::Events::RssFeedPlaylistCreationRequestEvent* e ) {
    QPointer<IPlaylist> playlist = new DummyPlaylist();
    QList<INews*>* allNews = _mediaLibrary->mediaDB()->newsRepository()->allNews( true );
    QImage placeholder( Constants::PLAYLIST_CREATOR_IMAGE_DIR + Constants::PLAYLIST_CREATOR_NEWS_PLACEHOLDER );

    while ( !allNews->isEmpty() ) {
        INews* news = allNews->takeFirst();
        Media *media = new Media( news->guid(),
                                  "",
                                  MediaType( RedBullPlayer::Tools::UuidCreator::create(), "news" ) );
        media->setUrl( news->link() );
        media->setDescription( news->description() );
        media->setCopyright( news->content() );
        media->setTitle( news->title() );

        if ( _mediaLibrary->mediaDB()->newsRepository()->anyNewsImageExists( news->guid() ) ) {
            QList<INewsImage*>* allImages = _mediaLibrary->mediaDB()->newsRepository()->allNewsImages( news->guid() );
            INewsImage* firstImage = allImages->takeFirst();
            media->setCoverImage( firstImage->image() );
        } else {
            media->setCoverImage( placeholder );
        }

        media->setArtist( news->pubDate().toString() );
        DummyPlaylistItem *dummyItem = new DummyPlaylistItem( media );
        playlist->addItem( dummyItem );
    }

    PlaylistCreatedEvent event( playlist, PlaylistContentTypeChangedEvent::NEWS );
    Aggregator::instance()->sendEvent( &event );
}
