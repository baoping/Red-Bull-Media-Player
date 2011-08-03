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
#include "MediaWatcherModule.h"

#include <QtPlugin>
#include <QDebug>

#include "../../ToolLib/ConfigManager.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::MediaWatcher;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

MediaWatcherModule::MediaWatcherModule() : IMediaWatcher() {
}

MediaWatcherModule::~MediaWatcherModule() {
}

void MediaWatcherModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
}

void MediaWatcherModule::SetupModuleConnections() {
    _mediaWatcher = new MediaWatcher( ConfigManager::instance()->getMediaDirectory(), this );
    _exclusionListHandler = new MediaWatcherExclusionListHandler( this );

    //add some internal files to built-in exclusion list
    QFileInfo mediaDb( ConfigManager::instance()->getMediaLibraryFileName() );
    _exclusionListHandler->addFile( mediaDb.absoluteFilePath() );
    _mediaWatcherEventBridge = new MediaWatcherEventBridge( _mediaWatcher, _exclusionListHandler, this );
}

Q_EXPORT_PLUGIN2( MediaWatcherModule, RedBullPlayer::Modules::MediaWatcher::MediaWatcherModule );
