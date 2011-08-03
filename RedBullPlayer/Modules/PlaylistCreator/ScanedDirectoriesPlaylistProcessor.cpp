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
#include "ScanedDirectoriesPlaylistProcessor.h"

#include <QDir>
#include <QFileInfo>
#include <QDirIterator>

#include "../../Container/Error.h"
#include "PlaylistItemEventHandler.h"

using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;

ScanedDirectoriesPlaylistProcessor::ScanedDirectoriesPlaylistProcessor( IMediaLibrary *mediaLibrary,
        QStringList addedScanDirectories, QStringList removedScanDirectories,
        QObject* parent )
    : QObject( parent ) {
    _mediaLibrary = mediaLibrary;
    _addedScanDirectories = addedScanDirectories;
    _removedScanDirectories = removedScanDirectories;
}

void ScanedDirectoriesPlaylistProcessor::processPlaylist() {
    processRemovedDirectories();
    processAddedDirectories();
}

void ScanedDirectoriesPlaylistProcessor::processAddedDirectories() {
    for( int i = 0; i < _addedScanDirectories.size(); i++ )
        processDirectory( _addedScanDirectories.at( i ) );
}

void ScanedDirectoriesPlaylistProcessor::processDirectory( QString directoryAbsolutePath ) {
    QList<QString> directories;
    QDir dir = QDir( directoryAbsolutePath );

    if( !dir.exists() )
        return;

    directories.append( dir.absolutePath() );
    QDirIterator it( directoryAbsolutePath, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories );

    while ( it.hasNext() ) {
        QDir subDir = QDir( it.next() );
        directories.append( subDir.absolutePath() );
    }

    for( int i = 0; i < directories.count(); i++ ) {
        QDir directory( directories.at( i ) );

        if( directory.exists() ) {
            QFileInfoList list = directory.entryInfoList( QDir::NoFilter, QDir::Time | QDir::Reversed );

            for( int fileInFsIndex = 0; fileInFsIndex < list.length(); fileInFsIndex++ ) {
                QFileInfo file = list.at( fileInFsIndex );

                if( !file.isFile() )
                    continue;

                processNewMedia( file, false );
            }
        }
    }
}

void ScanedDirectoriesPlaylistProcessor::processRemovedDirectories() {
    if( _removedScanDirectories.count() == 0 )
        return;

    QList<IMedia*>* allMedia = _mediaLibrary->mediaDB()->mediaRepository()->allMedia();

    if( allMedia == NULL )
        return;

    IMedia *currentMedia;

    for( int i = 0; i < allMedia->count(); i++ ) {
        currentMedia = allMedia->at( i );

        if( currentMedia ) {
            QFileInfo fileInfo = QFileInfo( currentMedia->fileName() );

            for( int j = 0; j < _removedScanDirectories.size(); j++ ) {
                if( fileInfo.absolutePath().indexOf( _removedScanDirectories.at( j ) ) == 0 ) {
                    Error err;

                    if ( _mediaLibrary->mediaDB()->mediaRepository()->deleteMedia( *currentMedia, &err ) ) {
                        emit mediaRemoved( currentMedia->guid().toString() );
                    } else {
                        Error::raise( &err );
                    }

                    delete currentMedia;
                    break;
                }
            }
        }
    }

    delete allMedia;
}


void ScanedDirectoriesPlaylistProcessor::processNewMedia( QFileInfo file, bool userAdded ) {
    PlaylistItemEventHandler::processNewMediaFile( _mediaLibrary, file, userAdded );
}
