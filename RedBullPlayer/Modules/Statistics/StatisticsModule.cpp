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
#include "StatisticsModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>

#include "../../Container/Constants.h"
#include "../../Container/Error.h"
#include "../../Container/Container.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Interfaces/MediaLibrary/ISystemMetaDataRepository.h"



using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;

using namespace RedBullPlayer::Modules::Statistics;
using namespace RedBullPlayer::Modules::MediaLibrary;

StatisticsModule::StatisticsModule() : IStatistics() {
    _internetAvailable = false;
    _statisticClientCacheRepository = 0;
    _senderQueue = 0;
    _senderQueue = new SenderQueue( this, this );
    connect( _senderQueue, SIGNAL( entrySentToServer( QString ) ),
             this, SLOT( statisticRequestFinished( QString ) ) );
}

void StatisticsModule::Setup( QObject* shell, QDomElement config ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATISTICS, "Setup Module." );
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
}

void StatisticsModule::SetupModuleConnections() {
    IMediaLibrary* mediaLibrary = 0;
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;

    if ( ! mediaLibrary ) {
        qFatal( "Could not find MediaLibrary module" );
        return;
    }

    _playerUuid =  mediaLibrary-> mediaDB()->systemMetaDataRepository()->getPlayerUUID();
    setStatisticClientCacheRepository( mediaLibrary-> mediaDB()->statisticClientCacheRepository() );
    setSystemMetaDataRepository( mediaLibrary->mediaDB()->systemMetaDataRepository() );
    _senderQueue->setPlayerUuid( _playerUuid );
    _senderQueue->setApiKey( ConfigManager::instance()->getApiKey() );
    EventAggregator::Aggregator::instance()->registerListener( InternetConnectionStatusChanged, this );
    EventAggregator::Aggregator::instance()->registerListener( CreateStatisticsEntry, this );

    if ( mediaLibrary->mediaDB()->systemMetaDataRepository()->allowCollectStatistic() ) {
        CreateStatisticEntryEvent ev( Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED, QString::null );
        onCreateStatisticsEntry( &ev );
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_STATISTICS, QString( "SetupModuleConnections done" ) );
}

void StatisticsModule::setStatisticClientCacheRepository( RedBullPlayer::Modules::MediaLibrary::IStatisticClientCacheRepository*  repository ) {
    _statisticClientCacheRepository = repository;
}

void StatisticsModule::setSystemMetaDataRepository( RedBullPlayer::Modules::MediaLibrary::ISystemMetaDataRepository* repository ) {
    _systemMetaDataRepository = repository;
}


void StatisticsModule::queueCachedRequests() {
    if ( ! statisticClientCacheRepository() ) {
        qWarning() << "No statisticClientCacheRepository available!";
        return;
    }

    if ( ! systemMetaDataRepository() ) {
        qWarning() << "No SystemMetaDataRepository available!";
        return;
    }

    QList<IStatisticEntry*>* cached = statisticClientCacheRepository()->getAllEntries();
    IStatisticEntry* curr;
    foreach( curr, *cached ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_STATISTICS, QString( "Queue Cached Request: %1 %2" )
                                         .arg( curr->entryType() )
                                         .arg( curr->date().toString() ) );
        senderQueue()->queueEntry( curr );
    }
}

void StatisticsModule::statisticRequestFinished( QString entryUuid ) {
    if ( ! statisticClientCacheRepository() ) {
        qWarning() << "No statisticClientCacheRepository available!";
        return;
    }

    Error err;
    statisticClientCacheRepository()->deleteEntry( QUuid( entryUuid ), &err );
}

bool StatisticsModule::event( QEvent * e ) {
    if ( e->type() == InternetConnectionStatusChanged ) {
        InternetConnectionStatusChangedEvent* ev = static_cast<InternetConnectionStatusChangedEvent*>( e );
        onInternetStatusChange( ev );
    }

    if ( e->type() == CreateStatisticsEntry ) {
        if ( systemMetaDataRepository() && systemMetaDataRepository()->allowCollectStatistic() ) {
            CreateStatisticEntryEvent* ev = static_cast<CreateStatisticEntryEvent*>( e );
            onCreateStatisticsEntry( ev );
        }
    }

    return false;
}

void StatisticsModule::onInternetStatusChange( RedBullPlayer::Events::InternetConnectionStatusChangedEvent* ev ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATISTICS, QString( "onInternetStatusChange: %1" )
                                     .arg( ev->internetAvailable() ) );
    _internetAvailable = ev->internetAvailable();
    _senderQueue->setInternetAvailable( internetAvailable() );

    if ( _internetAvailable ) {
        queueCachedRequests();
    }
}

void StatisticsModule::onCreateStatisticsEntry( RedBullPlayer::Events::CreateStatisticEntryEvent* ev ) {
    Q_ASSERT( ev );
    SimpleLogger::instance()->debug( Constants::LOGGER_STATISTICS, QString( "onCreateStatisticsEntry: %1" )
                                     .arg( ev->eventType() ) );

    if ( ! _statisticClientCacheRepository ) {
        SimpleLogger::instance()->error( Constants::LOGGER_STATISTICS, "No client Cahce repository set." );
        qFatal( "No client Cahce repository set." );
        return;
    }

    if ( _internetAvailable ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_STATISTICS, QString( "Enqueue: %1" )
                                         .arg( ev->eventType() ) );
        _senderQueue->queueEntry( _statisticClientCacheRepository->createEntry( ev->eventType(), ev->data() ) );
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_STATISTICS, QString( "Cache: %1" )
                                         .arg( ev->eventType() ) );
        _statisticClientCacheRepository->createEntry( ev->eventType(), ev->data() );
    }
}



Q_EXPORT_PLUGIN2( StatisticsModule, RedBullPlayer::Modules::Statistics::StatisticsModule );
