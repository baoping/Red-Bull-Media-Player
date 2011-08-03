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
#include "UpdateManager.h"

// Qt include
#include <QDebug>

using namespace RedBullPlayer::Updater;

UpdateManager::UpdateManager( QObject* parent ) : QObject( parent ) {
    // qDebug()<< Q_FUNC_INFO;
}

void UpdateManager::addProcess( UpdateProcess* process ) {
    //  qDebug()<< Q_FUNC_INFO;
    _prcesses.append( process );
    qSort( _prcesses.begin(), _prcesses.end(), UpdateProcess::lessThen );
    connect( process, SIGNAL( taskFailed( QString, QString ) ),
             this, SIGNAL( taskFailed( QString, QString ) ) );
    connect( process, SIGNAL( taskFinished( QString ) ),
             this, SIGNAL( taskFinished( QString ) ) );
    connect( process, SIGNAL( taskStarted( QString ) ),
             this, SIGNAL( taskStarted( QString ) ) );
}

void UpdateManager::execute( PlayerVersion* fromVersion, PlayerVersion* toVersion ) {
    UpdateProcess* p = findMatchingProcess( fromVersion, toVersion );

    if ( p ) {
        qApp->processEvents();
        p->execute();
        qApp->processEvents();
    }
}

UpdateProcess* UpdateManager::findMatchingProcess( PlayerVersion* fromVersion, PlayerVersion* toVersion ) {
    qDebug() << Q_FUNC_INFO;
    qDebug() << "Search for Updater from: " << fromVersion->internalVersionString() << " to: " << toVersion->internalVersionString();

    for ( int i = 0; i < _prcesses.size(); ++i ) {
        qDebug() << "Found updater from: " << _prcesses.at( i )->fromVersion()->internalVersionString() << " to: " << _prcesses.at( i )->toVersion()->internalVersionString();

        if ( *fromVersion == *_prcesses.at( i )->fromVersion() && *toVersion == *_prcesses.at( i )->toVersion() ) {
            qDebug() << "Updater Matched!";
            return _prcesses.at( i );
        }
    }

    qDebug() << "No matching Updater found";
    return 0;
}
