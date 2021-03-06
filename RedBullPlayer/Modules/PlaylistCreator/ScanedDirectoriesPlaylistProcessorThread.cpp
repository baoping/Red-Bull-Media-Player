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
#include "ScanedDirectoriesPlaylistProcessorThread.h"

#include "../../EventAggregator/Aggregator.h"
#include "../../Events/Playlist/MediaRemovedEvent.h"

using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

ScanedDirectoriesPlaylistProcessorThread::ScanedDirectoriesPlaylistProcessorThread( IMediaLibrary *mediaLibrary,
        QStringList addedScanDirectories, QStringList removedScanDirectories,
        QObject* parent )
    : QThread( parent ) {
    _mediaLibrary = mediaLibrary;
    _addedScanDirectories = addedScanDirectories;
    _removedScanDirectories = removedScanDirectories;
}

void ScanedDirectoriesPlaylistProcessorThread::run() {
    _worker = new ScanedDirectoriesPlaylistProcessor( _mediaLibrary,
            _addedScanDirectories,
            _removedScanDirectories, this );
    connect( _worker, SIGNAL( mediaRemoved( QString ) ), this, SLOT( mediaRemovedSlot( QString ) ) );
    _worker->processPlaylist();
    exec();
}


/**
  * Send Events from Thread class connected using SIGNALS/SLOTS. So that the UI receives the events
  * on the correct thread.
  * Can be done here as the Thread class it self stays on the main thread, funny ya!
  */
void ScanedDirectoriesPlaylistProcessorThread::mediaRemovedSlot( QString mediaId ) {
    QUuid mediaGuid = QUuid( mediaId );
    MediaRemovedEvent e( mediaGuid );
    Aggregator::instance()->sendEvent( &e );
}
