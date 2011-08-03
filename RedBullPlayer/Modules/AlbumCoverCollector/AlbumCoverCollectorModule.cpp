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
#include "AlbumCoverCollectorModule.h"

//Qt includes
#include <QtCore/QUrl>
#include <QtPlugin>
#include <QDebug>

//Project includes
#include "../../Container/Constants.h"
#include "../../Container/Error.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/MediaLibrary/CoverImageChangedEvent.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::AlbumCoverCollector;
using namespace RedBullPlayer::Modules::MediaLibrary;

AlbumCoverCollectorModule::AlbumCoverCollectorModule() : IAlbumCoverCollector() {}

AlbumCoverCollectorModule::~AlbumCoverCollectorModule() {
    while ( !_collectors->empty() ) {
        delete _collectors->takeFirst();
    }
}

void AlbumCoverCollectorModule::Setup( QObject* shell, QDomElement config ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
    _collectors = new QList<RedBullPlayer::Modules::AlbumCoverCollector::Collector*>();
    _allowCollect = false;
    _internetAvailable = false;
    Aggregator::instance()->registerListener( RequestAlbumCoverImage, this );
    Aggregator::instance()->registerListener( MetaDataChanged, this );
    Aggregator::instance()->registerListener( InternetConnectionStatusChanged, this );
    Aggregator::instance()->registerListener( UserSettingsChanged, this );
}

void AlbumCoverCollectorModule::SetupModuleConnections() {
    resolveMediaLibrary();
    _allowCollect = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
}

void AlbumCoverCollectorModule::resolveMediaLibrary() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

void AlbumCoverCollectorModule::collectAlbumCover( QUuid mediaGuid, bool allowRequestMetaData ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    Collector* collector = new Collector( this );
    connect( collector, SIGNAL( signalImageFound( QUuid ) ),
             SLOT( slotImageFound( QUuid ) ) );
    connect( collector, SIGNAL( signalNoImageFound( QUuid ) ),
             SLOT( slotNoImageFound( QUuid ) ) );
    _collectors->append( collector );
    collector->searchAlbumCover( mediaGuid, allowRequestMetaData );
}

void AlbumCoverCollectorModule::slotImageFound( QUuid mediaGuid ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    //emit event
    CoverImageChangedEvent evCoverImage( mediaGuid );
    Aggregator::instance()->sendEvent( &evCoverImage );
    removeCollector( mediaGuid );
}

void AlbumCoverCollectorModule::slotNoImageFound( QUuid mediaGuid ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_ALBUMCOVERCOLLECTOR, Q_FUNC_INFO );
    removeCollector( mediaGuid );
}

void AlbumCoverCollectorModule::removeCollector( QUuid mediaGuid ) {
    int i;

    for( i = 0; i < _collectors->count(); i++ ) {
        Collector* collector = _collectors->at( i );

        if ( collector->getCurrentRequestGuid() == mediaGuid ) {
            _collectors->removeAt( i );
            collector->deleteLater();
            break;
        }
    }
}

bool AlbumCoverCollectorModule::event ( QEvent * e ) {
    if( e->type() == RequestAlbumCoverImage ) {
        onRequestAlbumCoverImage( static_cast<RequestAlbumCoverImageEvent*>( e ) );
    }

    if( e->type() == MetaDataChanged ) {
        onMetaDataChanged( static_cast<MetaDataChangedEvent*>( e ) );
    }

    if ( e->type() == InternetConnectionStatusChanged ) {
        onInternetConnectionStatusChanged( static_cast<InternetConnectionStatusChangedEvent*>( e ) );
    }

    if( e->type() == UserSettingsChanged ) {
        onUserSettingsChanged( static_cast<UserSettingsChangedEvent*>( e ) );
    }

    return false;
}

void AlbumCoverCollectorModule::onUserSettingsChanged( UserSettingsChangedEvent *event ) {
    _allowCollect = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
    resumeAlbumCoverCollection();
}

void AlbumCoverCollectorModule::onInternetConnectionStatusChanged( InternetConnectionStatusChangedEvent* event ) {
    _internetAvailable = event->internetAvailable();
    resumeAlbumCoverCollection();
}

void AlbumCoverCollectorModule::onRequestAlbumCoverImage( RequestAlbumCoverImageEvent *event ) {
    if ( _allowCollect && _internetAvailable ) {
        collectAlbumCover( event->requestId(), true );
    } else {
        bufferRequest( event->requestId(), true );
    }
}

void AlbumCoverCollectorModule::onMetaDataChanged( MetaDataChangedEvent *event ) {
    if ( _allowCollect  && _internetAvailable ) {
        collectAlbumCover( event->mediaId(), false );
    } else {
        bufferRequest( event->mediaId(), false );
    }
}

void AlbumCoverCollectorModule::resumeAlbumCoverCollection() {
    if ( _allowCollect && _internetAvailable ) {
        while ( _requestBuffer.count() > 0 ) {
            QPair<QUuid, bool>* bufferEntry = _requestBuffer.takeFirst();
            collectAlbumCover( bufferEntry->first, bufferEntry->second );
            delete bufferEntry;
        }
    }
}

void AlbumCoverCollectorModule::bufferRequest( QUuid requestId, bool metaData ) {
    QPair<QUuid, bool>* bufferEntry = new QPair<QUuid, bool>();
    bufferEntry->first = requestId;
    bufferEntry->second = metaData;
    _requestBuffer.append( bufferEntry );
}

Q_EXPORT_PLUGIN2( AlbumCoverCollectorModule, RedBullPlayer::Modules::AlbumCoverCollector::AlbumCoverCollectorModule );


