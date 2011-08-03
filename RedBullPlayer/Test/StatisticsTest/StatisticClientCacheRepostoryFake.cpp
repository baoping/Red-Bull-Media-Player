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
#include "StatisticClientCacheRepostoryFake.h"

#include "../../ToolLib/UuidCreator.h"
#include "../../Modules/MediaLibrary/StatisticEntry.h"

using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;


StatisticClientCacheRepostoryFake::StatisticClientCacheRepostoryFake( QObject *parent ) : QObject( parent ) {}
StatisticClientCacheRepostoryFake::~StatisticClientCacheRepostoryFake() {};

IStatisticEntry* StatisticClientCacheRepostoryFake::createEntry( QString entryType, QString data ) {
    QUuid id = UuidCreator::create();
    IStatisticEntry* e = new StatisticEntry( id, entryType,  data, QDateTime::currentDateTime() );
    _entries.append( e );
    return e;
}

IStatisticEntry* StatisticClientCacheRepostoryFake::createInMemoryEntry( QString entryType, QString data ) {
    QUuid id = UuidCreator::create();
    IStatisticEntry* e = new StatisticEntry( id, entryType,  data, QDateTime::currentDateTime() );
    return e;
}

bool StatisticClientCacheRepostoryFake::deleteEntry( QUuid guid, RedBullPlayer::IError* error ) {
    error->reset();
    int delIndex = -1;

    for( int i = 0; i < _entries.count() && delIndex == -1; i++ ) {
        if ( _entries.at( i )->guid() == guid ) {
            delIndex = i;
        }
    }

    if ( delIndex > -1 ) {
        qDebug() << "Current Item Count: " << _entries.count();
        qDebug() << "Remove entry at: " << delIndex;
        _entries.takeAt( delIndex );
        qDebug() << "Current Item Count: " << _entries.count();
        qDebug() << "Removed entry at: " << delIndex;
        return true;
    }

    return false;
}

QList<IStatisticEntry*>* StatisticClientCacheRepostoryFake::getAllEntries() {
    return &_entries;
}
