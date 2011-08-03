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
#include "SearchEngine.h"

// Qt includes
#include <QUuid>
#include <QString>
#include <QImage>
#include <QList>
#include <QMessageBox>
#include <QMap>

// Projectincludes
#include "MediaDB.h"
#include "../../Interfaces/MediaLibrary/ISnapshot.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Interfaces/MediaLibrary/IMediaRepository.h"
#include "../../Interfaces/MediaLibrary/ITag.h"
#include "../../Events/Playlist/SearchPlaylistGenerationStartedEvent.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../Container/Error.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;

SearchEngine::SearchEngine( IMediaLibrary* mediaLibrary,  QObject* parent ) : ISearchEngine( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    Q_ASSERT( mediaLibrary != 0 );
    _mediaLibrary = mediaLibrary;
}

void SearchEngine::queryForName( QString query ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QList<QUuid>* quickFoundItems = doQuickDBSearch( query );
    doSlowgQuery( query, quickFoundItems );
    delete quickFoundItems;
}

QList<QUuid>* SearchEngine::doQuickDBSearch( QString query ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QList<QUuid>* quckResult = _mediaLibrary->mediaDB()->mediaRepository()->getMediaUsingDBSearch( query );
    QUuid mediaID;
    foreach( mediaID, *quckResult ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Emitting mediaFound SIGNAL" );
        MediaDiscoveredEvent* mde = new MediaDiscoveredEvent( mediaID, MediaDiscoveredEvent::SEARCH );
        Aggregator::instance()->postEvent( mde );
    }
    return quckResult;
}

void SearchEngine::doSlowgQuery( QString query, QList<QUuid>* allreadyFoundItems  ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QStringList queryTerms = query.split( " ", QString::SkipEmptyParts );
    queryTerms.removeDuplicates();
    QList<QPair<QUuid, QString>*>* allMedia = _mediaLibrary->mediaDB()->mediaRepository()->allMediaInUIOrder();

    while ( !allMedia->empty() ) {
        QPair<QUuid, QString>* p = allMedia->takeFirst();

        if ( ! QFile::exists( p->second ) ) {
            continue;
        }

        IMedia* media = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( p->first );

        if ( ! allreadyFoundItems->contains( media->guid() ) && isNameMatch( media, queryTerms ) ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Emitting mediaFound SIGNAL" );
            MediaDiscoveredEvent* mde = new MediaDiscoveredEvent( media->guid(), MediaDiscoveredEvent::SEARCH );
            Aggregator::instance()->postEvent( mde );
        }

        delete media;
    }

    delete allMedia;
}



bool SearchEngine::isNameMatch( IMedia* media, QStringList searchTerms ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QFileInfo fi(  media->fileName() );
    QStringList searchFields;
    searchFields
            << media->title()
            << fi.baseName()
            << media->album()
            << media->artist()
            // << media->copyright()
            // << media->description()
            // << media->genre()
            // << media->publisher()
            ;
    QStringList parts;
    QString searchField;
    foreach( searchField, searchFields ) {
        parts << searchField.split( " ", QString::SkipEmptyParts );
        parts << searchField.split( "_", QString::SkipEmptyParts );
        parts << searchField.split( "-", QString::SkipEmptyParts );
        parts << searchField.split( ".", QString::SkipEmptyParts );
    }
    parts.removeDuplicates();
    bool * foundAll = new bool[ searchTerms.count() ];

    for( int i = 0; i < searchTerms.count(); i++ ) {
        foundAll[i] = false;
    }

    QString word;

    for( int i = 0; i < searchTerms.count(); i++ ) {
        foreach( word, parts ) {
            if ( word.toLower().startsWith( searchTerms.at( i ).toLower() ) ) {
                foundAll[i] = true;
            }
        }
    }

    for( int i = 0; i < searchTerms.count(); i++ ) {
        if ( ! foundAll[i] ) {
            delete [] foundAll;
            return false;
        }
    }

    return true;
}
