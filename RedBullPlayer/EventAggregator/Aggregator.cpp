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
#include "Aggregator.h"

// Qt includes
#include <QString>
#include <QDebug>
#include <QCoreApplication>

using namespace RedBullPlayer::EventAggregator;

Aggregator* Aggregator::_instance = 0;

Aggregator::Aggregator( QObject* parent ) : QObject( parent ) {}

Aggregator* Aggregator::instance() {
    if ( _instance == 0 ) {
        _instance = new Aggregator();
    }

    return _instance;
}

void Aggregator::registerListener( QEvent::Type eventType, QObject* listener ) {
    if ( ! _eventListener.contains( eventType ) ) {
        _eventListener[ eventType ] = new QList<QObject*>();
    }

    if ( ! getListeners( eventType )->contains( listener ) ) {
        getListeners( eventType )->append( listener );
    } else {
        qWarning() << QString( "Try to register object %1 twice as listener for eventId %2" )
                   .arg( listener->metaObject()->className() )
                   .arg( eventType );
    }
}

void Aggregator::unregisterListener( QEvent::Type eventType, QObject* listener ) {
    if ( ! _eventListener.contains( eventType ) ) {
        return;
    }

    QList<QObject*>* listeners = _eventListener[ eventType ];

    if ( listeners->contains( listener ) ) {
        listeners->removeOne( listener );
    }
}

void Aggregator::unregisterListener( QObject* listener ) {
    QEvent::Type eventType;
    foreach( eventType, _eventListener.keys() ) {
        if ( ! _eventListener.contains( eventType ) ) {
            return;
        }

        QList<QObject*>* listeners = _eventListener[ eventType ];

        if ( listeners->contains( listener ) ) {
            listeners->removeOne( listener );
        }
    }
}

void Aggregator::sendEvent( QEvent* event ) {
    Q_ASSERT( event != 0 );
    QListIterator<QObject*> i( *getListeners( event->type() ) );

    while ( i.hasNext() ) {
        QObject* currObj = i.next();
        QCoreApplication::sendEvent( currObj, event );
    }
}

void Aggregator::postEvent( QEvent* event ) {
    Q_ASSERT( event != 0 );
    QListIterator<QObject*> i( *getListeners( event->type() ) );

    while ( i.hasNext() ) {
        QObject* currObj = i.next();
        QCoreApplication::postEvent( currObj, event );
    }
}

void Aggregator::postEvent( QObject* target, QEvent* event ) {
    QCoreApplication::postEvent( target, event );
}

const QList<QObject*>& Aggregator::getListenersForEvent( QEvent::Type eventType ) {
    return *getListeners( eventType );
}

QList<QObject*>* Aggregator::getListeners( QEvent::Type eventType ) {
    if ( ! _eventListener.contains( eventType ) ) {
        _eventListener[ eventType ] =  new QList<QObject*>();
    }

    return _eventListener[ eventType ];
}


