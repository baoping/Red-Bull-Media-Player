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
#include "SettingsEventHandler.h"

#include <QList>
#include <QDebug>

#include "../../EventAggregator/Aggregator.h"
#include "../../Events/Playlist/MediaRemovedEvent.h"
#include "../../Container/Error.h"

using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;

SettingsEventHandler::SettingsEventHandler( IMediaLibrary *mediaLibrary, QObject *parent )
    : QObject( parent ) {
    _mediaLibrary = mediaLibrary;
    Aggregator::instance()->registerListener( ScanedDirectoriesSettingsChanged, this );
    Aggregator::instance()->registerListener( FileExtensionsSettingsChanged, this );
}

SettingsEventHandler::~SettingsEventHandler() {}

bool SettingsEventHandler::event( QEvent *event ) {
    if( event->type() == ScanedDirectoriesSettingsChanged ) {
        onScanedDirectoriesSettingsChanged( static_cast<ScanedDirectoriesSettingsChangedEvent*>( event ) );
    }

    if( event->type() == FileExtensionsSettingsChanged ) {
        onFileExtensionsSettingsChanged( static_cast<FileExtensionsSettingsChangedEvent*>( event ) );
    }

    return false;
}

void SettingsEventHandler::onScanedDirectoriesSettingsChanged( ScanedDirectoriesSettingsChangedEvent *e ) {
    QThread *scanedDirectoriesProcessor = new ScanedDirectoriesPlaylistProcessorThread( _mediaLibrary,
            e->addedDirectories(),
            e->removedDirectories(),
            this );
    enqueueThread( scanedDirectoriesProcessor );
}

void SettingsEventHandler::onFileExtensionsSettingsChanged( FileExtensionsSettingsChangedEvent *e ) {
    QThread *fileExtensionsProcessor = new DynamicFileExtensionsPlaylistProcessorThread( _mediaLibrary,
            e->addedExtensions(),
            e->removedExtensions(), this );
    enqueueThread( fileExtensionsProcessor );
}

void SettingsEventHandler::enqueueThread( QThread *processor ) {
    connect( processor, SIGNAL( finished() ), this, SLOT( playlistProcessorThreadFinished() ) );

    if( _playlistProcessorThreads.isEmpty() )
        processor->start( QThread::NormalPriority );
    else
        _playlistProcessorThreads.enqueue( processor );
}

void SettingsEventHandler::playlistProcessorThreadFinished() {
    if( _playlistProcessorThreads.isEmpty() )
        return;

    QThread *processor = _playlistProcessorThreads.dequeue();
    processor->start( QThread::NormalPriority );
}


