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
#include "FileThread.h"

#include <QFile>
#include <QMutexLocker>
#include <QFileInfo>
#include <QDataStream>
#include <QDebug>

using namespace RedBullPlayer::Modules::TaskManager;

FileThread::FileThread( QString sourceFileName,
                        QString targetFileName,
                        bool deleteAfterCopy,
                        QObject *parent ) :
    QThread( parent ) {
    _sourceFileName = sourceFileName;
    _targetFileName = targetFileName;
    _deleteAfterCopy = deleteAfterCopy;
    _cancelationRequested = false;
}

void FileThread::run() {
    moveToThread( this );
    //QThread::run();
    qDebug() << "FileThread => run";

    //Fix if source & target are the same
    if ( QString::compare( _sourceFileName, _targetFileName, Qt::CaseInsensitive ) == 0 ) {
        qDebug() << "FileThread => emit failed & quit";
        emit failed();
        return;
    }

    QFile source( _sourceFileName );
    QFile target( _targetFileName );

    //Delete existing file
    if ( target.exists() ) {
        target.remove();
    }

    bool ok;

    if ( _deleteAfterCopy ) {
        ok = source.rename( QFileInfo( target ).absoluteFilePath() );

        if ( ok ) {
            qDebug() << "FileThread => (DeleteAfterCopy) emit success & quit";

            if ( source.isOpen() ) source.close();

            if ( target.isOpen() ) target.close();

            emit success();
            return;
        }
    }

    ok = source.open( QFile::ReadOnly ) && target.open( QFile::WriteOnly );

    if ( !ok ) {
        qDebug() << "FileThread => emit failed & quit";

        if ( source.isOpen() ) source.close();

        if ( target.isOpen() ) target.close();

        emit failed();
        return;
    }

    QDataStream in( &source );
    QDataStream out( &target );
    qint64 bytesLeft = 0;
    bytesLeft = source.bytesAvailable();

    while ( bytesLeft > 0 ) {
        if ( _cancelationRequested ) {
            qDebug() << "FileThread => emit canceled & quit";

            if ( source.isOpen() ) source.close();

            if ( target.isOpen() ) {
                target.remove();
                target.close();
            }

            emit canceled();
            return;
        }

        try {
            int bytesWritten = 0;
            char* rawData = 0;

            if ( bytesLeft <= 5242880 ) {
                rawData = new char[bytesLeft];
                in.readRawData( rawData, bytesLeft );
                bytesWritten = bytesLeft;
            } else {
                rawData = new char[5242880];
                in.readRawData( rawData, 5242880 );
                bytesWritten = 5242880;
            }

            if ( ( bytesWritten > 0 ) && ( rawData != 0 ) ) {
                out.writeRawData( rawData, bytesWritten );
                bytesLeft -= bytesWritten;
            }
        } catch( ... ) {
            qDebug() << "FileThread => emit failed & quit";

            if ( source.isOpen() ) source.close();

            if ( target.isOpen() ) {
                target.remove();
                target.close();
            }

            emit failed();
            return;
        }
    }

    if ( source.isOpen() ) {
        if ( _deleteAfterCopy ) source.remove();

        source.close();
    }

    if ( target.isOpen() ) target.close();

    qDebug() << "FileThread => emit success";
    emit success();
}

void FileThread::requestCancelation() {
    QMutexLocker locker( &_cancelMutex );
    _cancelationRequested = true;
}

