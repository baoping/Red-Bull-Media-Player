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
#include "SearchDrivenPlaylistCreator.h"


// Qt includes
#include <QtConcurrentRun>
#include <QFuture>


// Project includes
#include "../../Events/EventTypes.h"
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../Events/Playlist/SearchPlaylistGenerationStartedEvent.h"
#include "../../Events/Playlist/SearchEndedEvent.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"
#include "../../Modules/MediaLibrary/SearchEngine.h"

#include "DummyPlaylistItem.h"
#include "DummyPlaylist.h"


using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;

SearchDrivenPlaylistCreator::SearchDrivenPlaylistCreator( IMediaLibrary* mediaLibrary, QObject* parent )
    : QObject( parent ) {
    Q_ASSERT( mediaLibrary != 0 );
    _mediaLibrary = mediaLibrary;
    _futureWatcher = new QFutureWatcher<void>( this );
    Aggregator::instance()->registerListener( SearchChanged, this );
    _timer = new QTimer( this );
    connect( _timer, SIGNAL( timeout() ),
             this, SLOT( timerShot() ) );
    connect( _futureWatcher, SIGNAL( finished() ),
             this, SLOT( searchFinished() ) );
    _timer->setSingleShot( true );
    connect( _mediaLibrary->searchEngine(), SIGNAL( mediaFound( QString ) ),
             this, SLOT( mediaFound( QString ) ) );
}


bool SearchDrivenPlaylistCreator::event( QEvent* ev ) {
    if (  ev->type() == SearchChanged ) {
        onSearchChangedEvent( static_cast<SearchChangedEvent*>( ev ) );
    }

    return false;
}

void SearchDrivenPlaylistCreator::mediaFound( QString uuid ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, "Received mediaFound SIGNAL" );
    MediaDiscoveredEvent mde( QUuid( uuid ), MediaDiscoveredEvent::SEARCH );
    Aggregator::instance()->sendEvent( &mde );
}

void SearchDrivenPlaylistCreator::onSearchChangedEvent( SearchChangedEvent* ev ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, "onSearchChangedEvent: " + ev->query() );

    if ( _currentQuery == ev->query().simplified() ) {
        return;
    }

    if ( ev->query().isNull() || ev->query().simplified().isEmpty() ) {
        _currentQuery = "*";
    } else {
        _currentQuery = ev->query().simplified();
    }

    if ( _timer->isActive() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, "stop timer" );
        _timer->stop();
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, "Starting timer" );
    _timer->start( 300 );
}

void SearchDrivenPlaylistCreator::timerShot() {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, "Doing search" );

    if ( _futureWatcher->isRunning() ) {
        _futureWatcher->cancel();
    }

    if ( _currentQuery == "*" ) {
        SearchEndedEvent se;
        Aggregator::instance()->sendEvent( &se );
        return;
    }

    SearchPlaylistGenerationStartedEvent spge( new DummyPlaylist() );
    Aggregator::instance()->sendEvent( &spge );
    QFuture<void> future = QtConcurrent::run( _mediaLibrary->searchEngine(),
                           &ISearchEngine::queryForName,
                           _currentQuery );
    _futureWatcher->setFuture( future );
}

void SearchDrivenPlaylistCreator::searchFinished() {
}
