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
#include "SenderQueue.h"

// QT includes
#include <QDebug>


// Project includes
#include "UrlGeneratorFactory.h"

#include "../../Container/Constants.h"
#include "../../Container/Error.h"
#include "../../Container/Container.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Interfaces/Statistics/IUrlGenerator.h"

using namespace RedBullPlayer::Modules::Statistics;
using namespace RedBullPlayer::Tools::Network;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

SenderQueue::SenderQueue( IStatistics* statisticsModule, QObject *parent ) : QObject( parent ) {
    Q_ASSERT( statisticsModule );
    _statisticsModule = statisticsModule;
    _currentEntry = 0;
    _networkAccessManager = 0;
    _networkAccessManager = new NetworkAccessManager( this );
    _timer = new QTimer( this );
    _timer->setInterval( 1000 );
    connect( _timer, SIGNAL( timeout() ),
             this, SLOT( timerShot() ) );
    _timer->setSingleShot( false );
    _timer->start( 1000 );
    connect( _networkAccessManager, SIGNAL( finished( QUrl, QByteArray, QString, QString ) ),
             this, SLOT( requestFinished( QUrl, QByteArray, QString, QString ) ) );
    connect( _networkAccessManager, SIGNAL( failed( QUrl, int, QString ) ),
             this, SLOT( requestFailed( QUrl, int, QString ) ) );
}


bool SenderQueue::queueEntry( RedBullPlayer::Modules::MediaLibrary::IStatisticEntry* entry ) {
    if ( ! internetAvailable() ) {
        return false;
    }

    _queue.enqueue( entry );
    return true;
}

void SenderQueue::setPlayerUuid( QUuid playerUuid ) {
    _playerUuid = playerUuid;
}

void SenderQueue::setApiKey( QString apiKey ) {
    _apiKey = apiKey;
}

void SenderQueue::setInternetAvailable( bool available ) {
    _internetAvailable = available;
}

void SenderQueue::timerShot() {
    if ( _queue.empty() ) {
        return;
    }

    if ( ! internetAvailable() ) {
        return;
    }

    if ( _currentEntry ) {
        return;
    }

    _currentEntry = _queue.dequeue();
    QUrl url = generateRequestUrl( *_currentEntry );
    SimpleLogger::instance()->info( Constants::LOGGER_STATISTICS,
                                    QString( "Request Statistics URL: %1" )
                                    .arg( url.toString() ) );
    _networkAccessManager->get( url );
}

QUrl SenderQueue::generateRequestUrl( const RedBullPlayer::Modules::MediaLibrary::IStatisticEntry& entry ) {
    IUrlGenerator* g =  UrlGeneratorFactory::getGeratorForRequest( entry );

    if ( ! g ) {
        qWarning() << "Unknown entry type: " << entry.entryType();
        qFatal( "Invalid entry type!" );
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_STATISTICS, QString( "APIKey: %1" ).arg( _apiKey ) );
    g->setPlayerUuid( _playerUuid );
    g->setApiKey( _apiKey );
    QUrl url = g->createRequestUrl( entry );
    delete g;
    return url;
}

void SenderQueue::requestFinished( QUrl requestUrl, QByteArray data, QString contentType, QString contentLength ) {
    SimpleLogger::instance()->info( Constants::LOGGER_STATISTICS,
                                    QString( "Finished Statistic request url: %1" )
                                    .arg( requestUrl.toString() ) );
    emit entrySentToServer( _currentEntry->guid().toString() );
    delete _currentEntry;
    _currentEntry = 0;
}

void SenderQueue::requestFailed( QUrl requestUrl, int httpStatusCode, QString httpReasonPhrase ) {
    SimpleLogger::instance()->error( Constants::LOGGER_STATISTICS,
                                     QString( "Error at Statistic request url: %1 httpStatusCode: %2 httpReasonPhrase: %3" )
                                     .arg( requestUrl.toString() )
                                     .arg( httpStatusCode )
                                     .arg( httpReasonPhrase ) );
    emit entrySentToServerFailed( _currentEntry->guid().toString() );
    delete _currentEntry;
    _currentEntry = 0;
}
