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
#include "MediaWatcherExclusionListHandler.h"

#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"

using namespace RedBullPlayer::Modules::MediaWatcher;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

MediaWatcherExclusionListHandler::MediaWatcherExclusionListHandler( QObject *parent ) :
    QObject( parent ) {
    Aggregator::instance()->registerListener( MediaWatcherAddExclusion, this );
    Aggregator::instance()->registerListener( MediaWatcherRemoveExclusion, this );
}

bool MediaWatcherExclusionListHandler::isExcluded( QString fileName ) {
    return _exclusionList.contains( fileName );
}

void MediaWatcherExclusionListHandler::addFile( QString fileName ) {
    if ( !_exclusionList.contains( fileName ) )
        _exclusionList.append( fileName );
}

void MediaWatcherExclusionListHandler::removeFile( QString fileName ) {
    if ( _exclusionList.contains( fileName ) )
        _exclusionList.removeAll( fileName );
}

bool MediaWatcherExclusionListHandler::event( QEvent* event ) {
    if ( event->type() == MediaWatcherAddExclusion ) {
        onMediaWatcherAddExclusion( static_cast<MediaWatcherAddExclusionEvent*>( event ) );
    } else if ( event->type() == MediaWatcherRemoveExclusion ) {
        onMediaWatcherRemoveExclusion( static_cast<MediaWatcherRemoveExclusionEvent*>( event ) );
    }

    return false;
}

void MediaWatcherExclusionListHandler::onMediaWatcherAddExclusion( RedBullPlayer::Events::MediaWatcherAddExclusionEvent* e ) {
    addFile( e->fileName() );
}

void MediaWatcherExclusionListHandler::onMediaWatcherRemoveExclusion( RedBullPlayer::Events::MediaWatcherRemoveExclusionEvent* e ) {
    removeFile( e->fileName() );
}
