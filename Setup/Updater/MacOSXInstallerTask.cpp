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
#include "MacOSXInstallerTask.h"
#if defined(Q_OS_MAC)

#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
using namespace RedBullPlayer::Updater;

MacOSXInstallerTask::MacOSXInstallerTask( QString oldAppPath, QString taskName )
    : UpdateTask( taskName ) {
    _oldAppPath = oldAppPath;
}


bool MacOSXInstallerTask::doWork() {
    QProcess rm;
    QStringList rmArgs;
    QString relOldPath = _oldAppPath;
    rmArgs << "-r" << "-v" << "-f" << relOldPath;
//    QMessageBox::information(0, "Info", relOldPath );
    rm.start( "/bin/rm", rmArgs );
    rm.waitForFinished(-1);

    if ( rm.exitCode() != 0 ) {
	QMessageBox::warning(0, "Error", "Could not delete old app bundle!" );
	return false;
    }

    QFileInfo newBundleInf( "../../" );
    QString newBulkdeAbsoluteName = newBundleInf.absolutePath();
    qDebug() << "New App Bulde: " << newBulkdeAbsoluteName;

    QFileInfo inf( "../../../" );
    qDebug() << "Set workingdir: " << inf.absolutePath();
    QDir::setCurrent( inf.absolutePath() );

//    QMessageBox::information(0, "Info", "Move from: " + newBulkdeAbsoluteName + "\nTo: " + _oldAppPath );
    if( ! QDir().rename( newBulkdeAbsoluteName, _oldAppPath ) ) {
	QMessageBox::warning(0, "Error", "Could not move app bundle!" );
	return false;
    }

    QProcess rm2;
    QStringList rmArgs2;
    QString rmArg2 =  inf.absolutePath().toLatin1();
//    QMessageBox::information(0, "Info", "rm arg: " + rmArg2 );
    rmArgs2 << "-r" << "-f" << rmArg2;
    rm2.start( "/bin/rm", rmArgs2 );
    rm2.waitForFinished(-1);

    if ( rm2.exitCode() != 0 ) {
	QMessageBox::warning(0, "Error", "Could not delete old app bundle!" );
	return false;
    }
    return true;
}

#endif //defined(Q_OS_MAC)

