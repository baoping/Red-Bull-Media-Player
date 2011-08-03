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
#include "UpdateHistoryUpdateTask.h"

// QT includes
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>

// Project includes
#include "../Package/Package.h"


using namespace RedBullPlayer::Updater;

UpdateHistoryUpdateTask::UpdateHistoryUpdateTask( QString fromVersion, QString toVersion, QString udateDataFile, QString taskName /* = "Write Update Log Task" */ )
    : UpdateTask( taskName ) {
    _fromVersion = fromVersion;
    _toVersion = toVersion;
    _udateDataFile = udateDataFile;
}


bool UpdateHistoryUpdateTask::doWork() {
#if ! defined(Q_OS_MAC)

    if ( ! QFileInfo( _udateDataFile ).exists() )
        return false;

#endif
#if defined(Q_OS_MAC)

    if  ( QFileInfo( _udateDataFile + "/Contents/MacOS/update_history.txt" ).exists() ) {
        QFile::copy( _udateDataFile + "/Contents/MacOS/update_history.txt", "update_history.txt" );
    }

#endif
    QFile updateHistory( "update_history.txt" );

    if ( ! updateHistory.open( QFile::Append ) ) {
        qWarning() << "Could not open update_history.txt for appending";
        return false;
    }

    QTextStream ts( &updateHistory );
    ts << "Installed update";
    ts << " date: " << QDateTime::currentDateTime().toString( Qt::TextDate );
    ts << " from: " << _fromVersion;
    ts << " to: " << _toVersion << "\r\n";
    updateHistory.flush();
    updateHistory.close();
#if ! defined(Q_OS_MAC)
    Package *p = Package::openEdit( _udateDataFile );
    p->addFile( "update_history.txt", "update_history.txt" );
    p->save( _udateDataFile );
    delete p;
#endif
    return true;
}
