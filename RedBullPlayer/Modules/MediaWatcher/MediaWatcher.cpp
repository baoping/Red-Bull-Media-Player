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
#include "MediaWatcher.h"

#include <QDebug>

#include <QDir>

using namespace RedBullPlayer::Modules::MediaWatcher;

MediaWatcher::MediaWatcher( QString path, QObject *parent ) :
    QObject( parent ) {
    _path = path;
    QDir dir( _path );
    _files = dir.entryInfoList( QDir::Files );
    _fileSystemWatcher = new QFileSystemWatcher( this );
    connect( _fileSystemWatcher, SIGNAL( directoryChanged( QString ) ), this, SLOT( directoryChanged( QString ) ) );
    _isWatching = false;
}

void MediaWatcher::startWatching() {
    QDir dir( _path );
    _files = dir.entryInfoList( QDir::Files );
    _fileSystemWatcher->addPath( dir.absolutePath() );
    _isWatching = true;
}

void MediaWatcher::stopWatching() {
    QDir dir( _path );
    _fileSystemWatcher->removePath( dir.absolutePath() );
    _isWatching = false;
}

void MediaWatcher::directoryChanged( const QString& path ) {
    QDir dir( _path );
    QFileInfoList newList = dir.entryInfoList( QDir::Files );

    //check for removed files
    for( int i = 0; i < _files.count(); i++ ) {
        QFileInfo file = _files.at( i );

        if ( !newList.contains( file ) ) {
            emit fileRemoved( file.absoluteFilePath() );
        }
    }

    //check for new files
    for( int i = 0; i < newList.count(); i++ ) {
        QFileInfo file = newList.at( i );

        if ( !_files.contains( file ) ) {
            emit fileAdded( file.absoluteFilePath() );
        }
    }

    _files = newList;
}
