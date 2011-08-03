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
#include "DynamicFileExtensionsPlaylistProcessor.h"

#include "../../Container/Error.h"
#include "PlaylistItemEventHandler.h"

using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;

DynamicFileExtensionsPlaylistProcessor::DynamicFileExtensionsPlaylistProcessor( IMediaLibrary *mediaLibrary,
        QStringList addedFileExtensions, QStringList removedFileExtensions,
        QObject* parent )
    : QObject( parent ) {
    _mediaLibrary = mediaLibrary;
    _addedFileExtensions = addedFileExtensions;
    _removedFileExtensions = removedFileExtensions;
}


void DynamicFileExtensionsPlaylistProcessor::processPlaylist() {
    if( _removedFileExtensions.count() == 0 )
        return;

    QList<IMedia*>* allMedia = _mediaLibrary->mediaDB()->mediaRepository()->allMedia();

    if( allMedia == NULL )
        return;

    IMedia *currentMedia;

    for( int i = 0; i < allMedia->count(); i++ ) {
        currentMedia = allMedia->at( i );

        if( currentMedia ) {
            QString fileName = currentMedia->fileName();
            QUuid mediaId = currentMedia->guid();

            for( int j = 0; j < _removedFileExtensions.size(); j++ )
                if( fileName.endsWith( _removedFileExtensions.at( j ) ) ) {
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

    delete allMedia;
}

