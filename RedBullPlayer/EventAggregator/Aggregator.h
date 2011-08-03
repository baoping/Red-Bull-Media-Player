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
#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include "EventAggregator_global.h"

// QT Includes
#include <QObject>
#include <QUuid>
#include <QList>
#include <QMap>
#include <QPair>
#include <QEvent>

namespace RedBullPlayer {
    namespace EventAggregator {
        class EVENTAGGREGATORSHARED_EXPORT Aggregator : public QObject {
                Q_OBJECT

            public:
                static Aggregator* instance();

            public slots:
                void registerListener( QEvent::Type eventType, QObject* listener );
                void unregisterListener( QEvent::Type eventType, QObject* listener );
                void unregisterListener( QObject* listener );
                void sendEvent( QEvent* event );
                void postEvent( QEvent* event );
                void postEvent( QObject* target, QEvent* event );
                const QList<QObject*>& getListenersForEvent( QEvent::Type eventType );

            protected:
                static Aggregator* _instance;

                Aggregator( QObject* parent = 0 );

                QList<QObject*>* getListeners( QEvent::Type eventType );

                QMap<QEvent::Type, QList<QObject*>* > _eventListener;
        };
    }
}
#endif // AGGREGATOR_H
