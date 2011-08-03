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
#include "DiskSpaceModel.h"

#include "../../../ToolLib/FileSystem/StorageFactory.h"

using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::TaskManager;

DiskSpaceModel::DiskSpaceModel( HoverBoxChangeVisibilityEvent::HoverBoxDockPosition hoverBoxPosition, QObject *parent ) :
    StateIconModel( TaskPurpose::DISK_SPACE, hoverBoxPosition, parent ) {
    _lastFreeBytes = 0;
    _fileSystemWatcher = new QFileSystemWatcher( this );
    connect( _fileSystemWatcher, SIGNAL( directoryChanged( QString ) ), this, SLOT( directoryChanged( QString ) ) );
}

void DiskSpaceModel::addPath( QString path ) {
    _fileSystemWatcher->addPath( path );
}

DiskSpaceModel::~DiskSpaceModel() {
    _fileSystemWatcher->deleteLater();
}

void DiskSpaceModel::directoryChanged( const QString & path  ) {
    doCheck();
}

void DiskSpaceModel::doCheck() {
    if ( _lastFreeBytes != StorageFactory::defaultStorage()->freeBytes() ) {
        emit freeDiskSpaceChanged( StorageFactory::defaultStorage()->freeBytes(),
                                   StorageFactory::defaultStorage()->usedBytes(),
                                   StorageFactory::defaultStorage()->capacity() );
    }
}
