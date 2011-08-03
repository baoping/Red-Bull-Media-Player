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
#include "EventAggregatorTest.h"

// QT includes
#include <QFileInfo>
#include <QList>
#include <QEvent>

// Project includes
#include "Aggregator.h"
#include "TestEventAggregator.h"
#include "EventSpy.h"

using namespace RedBullPlayer::EventAggregator;


void EventAggregatorTest::checkSingletonInstance() {
    Aggregator* a1 = Aggregator::instance();
    Aggregator* a2 = Aggregator::instance();
    QVERIFY( a1 == a2 );
}

void EventAggregatorTest::registerListener() {
    TestEventAggregator* a = new TestEventAggregator();
    QObject* o1 = new QObject();
    a->registerListener( QEvent::ActionAdded, o1 );
    a->registerListener( QEvent::AcceptDropsChange, o1 );
    QObject* o2 = new QObject();
    a->registerListener( QEvent::ActionAdded, o2 );
    const QList<QObject*>& list = a->getListenersForEvent( QEvent::ActionAdded );
    QVERIFY( list.count() == 2 );
    QVERIFY( list.contains( o1 ) );
    QVERIFY( list.contains( o2 ) );
    const QList<QObject*>& list2 = a->getListenersForEvent( QEvent::AcceptDropsChange );
    QVERIFY( list2.count() == 1 );
    QVERIFY( list2.contains( o1 ) );
}

void EventAggregatorTest::removeListener() {
    TestEventAggregator* a = new TestEventAggregator();
    QObject* o1 = new QObject();
    QObject* o2 = new QObject();
    a->registerListener( QEvent::ActionAdded, o1 );
    a->registerListener( QEvent::AcceptDropsChange, o1 );
    a->registerListener( QEvent::ActionAdded, o2 );
    a->registerListener( QEvent::AcceptDropsChange, o2 );
    a->unregisterListener( QEvent::ActionAdded, o1 );
    const QList<QObject*>& list = a->getListenersForEvent( QEvent::ActionAdded );
    QVERIFY( list.count() == 1 );
    QVERIFY( list.contains( o2 ) );
    const QList<QObject*>& list2 = a->getListenersForEvent( QEvent::AcceptDropsChange );
    QVERIFY( list2.count() == 2 );
    QVERIFY( list2.contains( o1 ) );
    QVERIFY( list2.contains( o2 ) );
    a->unregisterListener( o2 );
    const QList<QObject*>& list3 = a->getListenersForEvent( QEvent::ActionAdded );
    QVERIFY( list3.count() == 0 );
    const QList<QObject*>& list4 = a->getListenersForEvent( QEvent::AcceptDropsChange );
    QVERIFY( list4.count() == 1 );
    QVERIFY( ! list4.contains( o2 ) );
    QVERIFY( list4.contains( o1 ) );
}

void EventAggregatorTest::sendEvent() {
    TestEventAggregator* a = new TestEventAggregator();
    EventSpy* s1 = new EventSpy();
    EventSpy* s2 = new EventSpy();
    a->registerListener( QEvent::AcceptDropsChange, s1 );
    a->registerListener( QEvent::AcceptDropsChange, s2 );
    a->registerListener( QEvent::Enter, s1 );
    QEvent e( QEvent::AcceptDropsChange );
    a->sendEvent( &e );
    e = QEvent( QEvent::Enter );
    a->sendEvent( &e );
    QVERIFY( s1->recievedEvents().contains( QEvent::AcceptDropsChange ) );
    QVERIFY( s2->recievedEvents().contains( QEvent::AcceptDropsChange ) );
    QVERIFY( s1->recievedEvents().contains( QEvent::Enter ) );
    QVERIFY( ! s2->recievedEvents().contains( QEvent::Enter ) );
}

void EventAggregatorTest::duplicateRegisterListener() {
    TestEventAggregator* a = new TestEventAggregator();
    EventSpy* s1 = new EventSpy();
    a->registerListener( QEvent::AcceptDropsChange, s1 );
    a->registerListener( QEvent::AcceptDropsChange, s1 );
    QEvent e( QEvent::AcceptDropsChange );
    a->sendEvent( &e );
    QVERIFY( s1->recievedEvents().count() == 1 );
    QVERIFY( s1->recievedEvents().contains( QEvent::AcceptDropsChange ) );
}
