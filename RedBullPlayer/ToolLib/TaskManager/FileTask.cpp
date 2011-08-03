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
#include "FileTask.h"

#include <QFileInfo>
#include <QDir>
#include <QDataStream>
#include <QDebug>

using namespace RedBullPlayer::Modules::TaskManager;

FileTask::FileTask( QString fileName, QString targetDirectory, bool deleteAfterCopy, QObject *parent )
    : Task( TaskPurpose::IO, QFileInfo( fileName ).fileName(), parent ) {
    _fileName = fileName;
    _targetDirectory = targetDirectory;
    _deleteAfterCopy = deleteAfterCopy;
    _fileThread = 0;
}

void FileTask::prepare() {
    QFileInfo fileInfo( _fileName );
    QDir targetDir( _targetDirectory );
    _newFileName = QFileInfo( targetDir, fileInfo.fileName() ).absoluteFilePath();
    emit prepared( fileInfo.exists() && targetDir.exists() );
}

void FileTask::execute() {
    _fileThread = new FileThread( _fileName, _newFileName, _deleteAfterCopy, this );
    connect( _fileThread, SIGNAL( success() ), this, SLOT( thread_success() ), Qt::QueuedConnection );
    connect( _fileThread, SIGNAL( failed() ), this, SLOT( thread_failed() ), Qt::QueuedConnection );
    connect( _fileThread, SIGNAL( canceled() ), this, SLOT( thread_canceled() ), Qt::QueuedConnection );
    qDebug() << "FileTask => execute & start thread";
    _fileThread->start();
}

void FileTask::thread_success() {
    qDebug() << "FileTask => thread success";
    setResult( TaskResult::SUCCESSFUL );
    emit finished( true );
}

void FileTask::thread_failed() {
    qDebug() << "FileTask => thread failed";
    setResult( TaskResult::FAILED );
    emit finished( false );
}

void FileTask::thread_canceled() {
    qDebug() << "FileTask => thread canceled";
    setResult( TaskResult::FAILED );
    emit finished( false );
}

void FileTask::cancel() {
    if ( _fileThread != 0 ) {
        _fileThread->requestCancelation();
    }
}

void FileTask::clean() {
    if ( isCanceled() ) {
        QFile target( _newFileName );

        if ( target.exists() ) {
            target.remove();
        }
    }
}
