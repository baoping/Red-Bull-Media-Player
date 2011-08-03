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
#ifndef Q_OS_WIN
#ifndef Q_OS_MAC

#include "LinuxVolumeFinder.h"

#include <QProcess>
#include <QStringList>
#include <QDebug>
#include <QFileInfo>

using namespace RedBullPlayer::Tools;

LinuxVolumeFinder::LinuxVolumeFinder() {}

QString LinuxVolumeFinder::runMountCmd() {
    QProcess queryMount;
    queryMount.setProcessChannelMode( QProcess::MergedChannels );
    queryMount.start( "mount" , QStringList() << "-l" );
    QString out;

    if ( ! queryMount.waitForFinished() ) {
        qWarning() << queryMount.readAll() + " " + queryMount.errorString();
        return QString::null;
    }

    return queryMount.readAll();
}

QString LinuxVolumeFinder::findDriveByLabel( QString label ) {
    QString out = runMountCmd();

    if ( out.isNull() ) {
        return QString::null;
    }

    QStringList list = out.split( '\n', QString::SkipEmptyParts );
    QString searchstring = QString( "[%1]" ).arg( label );
    qDebug() << "Search for: " << searchstring;
    QString curr;
    foreach ( curr, list ) {
        if ( ! curr.contains( QString( "[%1]" ).arg( label ) ) ) {
            qDebug() << "Ignoring line: " << curr;
            continue;
        }

        qDebug() << "Parse line: " << curr;
        QStringList lineParts = curr.split( ' ' , QString::SkipEmptyParts );
        QString linePart;
        foreach( linePart, lineParts ) {
            qDebug() << "curr linePart: " << linePart;

            if ( linePart.startsWith( "/" ) && ! linePart.startsWith( "/dev" ) ) {
                return linePart;
            }
        }
    }
    return QString::null;
}

QString  LinuxVolumeFinder::getVolumeLabelForPath( QString path ) {
    qDebug() << Q_FUNC_INFO << " " << path;
    QFileInfo inf( path );

    if ( ! inf.exists() ) {
        qWarning() << path << "File or Directoy does not exists";
        return QString::null;
    }

    QString out = runMountCmd();

    if ( out.isNull() ) {
        return QString::null;
    }

    QStringList list = out.split( '\n', QString::SkipEmptyParts );
    QStringList possiibleMatchingLabels;
    QStringList possiibleMatchingMountPoints;
    QString curr;
    foreach ( curr, list ) {
        QStringList lineParts = curr.split( ' ' , QString::SkipEmptyParts );
        QString linePart;
        foreach( linePart, lineParts ) {
            if ( linePart.startsWith( "/" ) && ! linePart.startsWith( "/dev" ) &&  path.startsWith( linePart ) ) {
                possiibleMatchingMountPoints << linePart;
            }

            if ( linePart.startsWith( "[" )  && linePart.endsWith( "]" ) && linePart != "[]" ) {
                QString lp = linePart.remove( "[" ).remove( "]" );
                possiibleMatchingLabels << lp;
            }
        }

        if ( possiibleMatchingMountPoints.count() < possiibleMatchingLabels.count() ) {
            possiibleMatchingMountPoints << "";
        } else if ( possiibleMatchingMountPoints.count() > possiibleMatchingLabels.count() ) {
            possiibleMatchingLabels << "";
        }
    }
    QString bestMatching = "";
    int bestMatchinIndex = -1;

    for ( int i = 0; i < possiibleMatchingLabels.count(); i++ ) {
        QString la = possiibleMatchingLabels.at( i );
        QString ma = possiibleMatchingMountPoints.at( i );

        if ( ! la.isEmpty() && /* path.startsWith( ma ) && */ ma.length() > bestMatching.length() ) {
            bestMatching = ma;
            bestMatchinIndex = i;
        }
    }

    if ( bestMatchinIndex > -1 ) {
        qDebug() << "Return label: " << possiibleMatchingLabels.at( bestMatchinIndex );
        return  possiibleMatchingLabels.at( bestMatchinIndex );
    }

    qDebug() << "No matching label found";
    return QString::null;
}




#endif
#endif
