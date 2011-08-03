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
#include "SnapshotCreatorModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>
#include <QMessageBox>
#include <QUuid>
#include <QString>
#include <QMutexLocker>

// Project Includes
#include "../../Interfaces/ISnapshotCreationRequest.h"
#include "../../Container/Error.h"
#include "../../Container/Constants.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/SnapshotCreator/SnapshotCreatedEvent.h"
#include "../../Events/SnapshotCreator/SnapshotCreationRequestData.h"
#include "../../Events/SnapshotCreator/SnapshotCreatedResponseData.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Modules::SnapshotCreator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;

SnapshotCreatorModule::SnapshotCreatorModule() {
    _requestHandler = 0;
}


void SnapshotCreatorModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
}

void SnapshotCreatorModule::SetupModuleConnections() {
    _checkTimer = new QTimer( this );
    _checkTimer->setInterval( 500 );
    connect( _checkTimer, SIGNAL( timeout() ), this, SLOT( onCheckTimer() ) );
    Aggregator::instance()->registerListener( PlayerExitClicked, this );
    Aggregator::instance()->registerListener( SnapshotCreationRequested, this );
    Aggregator::instance()->registerListener( CancelSnapshotCreation, this );
    _checkTimer->start();
}

SnapshotCreatorModule::~SnapshotCreatorModule() {
    clearQueue();

    if ( _requestHandler != 0 ) {
        delete _requestHandler;
        _requestHandler = 0;
    }
}

void SnapshotCreatorModule::cleanUp() {
    Aggregator::instance()->unregisterListener( this );
    _checkTimer->stop();
}

bool SnapshotCreatorModule::event( QEvent* ev ) {
    if ( ev->type() == PlayerExitClicked ) {
        onCloseButtonClicked( static_cast<RedBullPlayer::Events::PlayerExitClickedEvent*>( ev ) );
    }

    if ( ev->type() == SnapshotCreationRequested ) {
        onSnapshotCreationRequested( static_cast<RedBullPlayer::Events::SnapshotCreationRequestedEvent*>( ev ) );
    }

    if ( ev->type() == CancelSnapshotCreation ) {
        onCancelSnapshotCreation( static_cast<RedBullPlayer::Events::CancelSnapshotCreationEvent*>( ev ) );
    }

    return false;
}

void SnapshotCreatorModule::clearQueue() {
    QMutexLocker locker( &_clearMutex );

    while ( !_requestQueue.isEmpty() ) {
        delete _requestQueue.takeFirst();
    }
}

void SnapshotCreatorModule::onCloseButtonClicked( RedBullPlayer::Events::PlayerExitClickedEvent* ev ) {
    clearQueue();
}

void SnapshotCreatorModule::onSnapshotCreationRequested( RedBullPlayer::Events::SnapshotCreationRequestedEvent* ev ) {
    QMutexLocker locker( &_requestMutex );
    SimpleLogger::instance()->debug( Constants::LOGGER_SNAPSHOTCREATOR, "onSnapshotCreationRequested" );
    RequestMetaData* metaData = new RequestMetaData( ev->requestData()->id(),
            ev->requestData()->movieFileName(),
            ev->requestData()->position(),
            ev->requestData()->second() );
    _requestQueue.enqueue( metaData );
}

void SnapshotCreatorModule::onCancelSnapshotCreation( RedBullPlayer::Events::CancelSnapshotCreationEvent* ev ) {
    if( ev == NULL )
        return;

    QMutexLocker locker( &_cancelMutex );

    for( int i = 0; i < _requestQueue.count(); i++ ) {
        if( _requestQueue.at( i ) != NULL )
            if ( _requestQueue.at( i )->id() == ev->requestId() ) {
                if( _requestQueue.at( i ) != NULL )
                    delete _requestQueue.takeAt( i );

                break;
            }
    }
}

void SnapshotCreatorModule::onCheckTimer() {
    QMutexLocker locker( &_checkMutex );

    if ( _requestQueue.count() > 0 ) {
        _checkTimer->stop();
        RequestMetaData* metaData = _requestQueue.dequeue();
        _requestHandler = new SnapshotRequestHandler( metaData, this );
        connect( _requestHandler, SIGNAL( snapshotFinished( QUuid, QString ) ), this, SLOT( snapshotFinished( QUuid, QString ) ) );
        connect( _requestHandler, SIGNAL( snapshotFailed( QUuid ) ), this, SLOT( snapshotFailed( QUuid ) ) );
        _requestHandler->requestSnapshot( );
    }
}

void SnapshotCreatorModule::snapshotFinished( QUuid id, QString fileName ) {
    QMutexLocker locker( &_finishedMutex );
    delete _requestHandler;
    _requestHandler = 0;
    SnapshotCreatedResponseData data( id, fileName );
    SnapshotCreatedEvent ev( &data );
    Aggregator::instance()->sendEvent( &ev );

    if ( !_checkTimer->isActive() ) {
        _checkTimer->start();
    }
}

void SnapshotCreatorModule::snapshotFailed( QUuid id ) {
    QMutexLocker locker( &_failedMutex );
    delete _requestHandler;
    _requestHandler = 0;

    if ( !_checkTimer->isActive() ) {
        _checkTimer->start();
    }
}



Q_EXPORT_PLUGIN2( SnapshotCreatorModule, RedBullPlayer::Modules::SnapshotCreator::SnapshotCreatorModule )
