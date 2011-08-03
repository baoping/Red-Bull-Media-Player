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
#include "MediaWatcherEventBridge.h"

#include <QDebug>

using namespace RedBullPlayer::Modules::MediaWatcher;

MediaWatcherEventBridge::MediaWatcherEventBridge( MediaWatcher* mediaWatcher,
        MediaWatcherExclusionListHandler* exclusionListHandler,
        QObject *parent ) : QObject( parent ) {
    _mediaWatcher = mediaWatcher;
    _exclusionListHandler = exclusionListHandler;
    connect( _mediaWatcher, SIGNAL( fileAdded( QString ) ), this, SLOT( fileAdded( QString ) ) );
    connect( _mediaWatcher, SIGNAL( fileRemoved( QString ) ), this, SLOT( fileRemoved( QString ) ) );
}

void MediaWatcherEventBridge::fileAdded( QString fileName ) {
    //handle file added to directory
}

void MediaWatcherEventBridge::fileRemoved( QString fileName ) {
    //handle file removed from directory
}
