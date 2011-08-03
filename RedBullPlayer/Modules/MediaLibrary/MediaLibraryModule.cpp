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
#include "MediaLibraryModule.h"

// Qt includes
#include <QtPlugin>
#include <QDebug>

// Project includes
#include "../../PlayerShell/Shell.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/SnapshotCreator/SnapshotCreationRequestedEvent.h"
#include "../../Events/SnapshotCreator/SnapshotCreationRequestData.h"
#include "../../Events/MediaLibrary/CoverImageChangedEvent.h"

//logging
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;

MediaLibraryModule::MediaLibraryModule() {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    _mediaDB = 0;
    _metadataHandler = 0;
    _searchEngine = 0;
}

void MediaLibraryModule::Setup( QObject* shell, QDomElement config ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
    _mediaDB = new MediaDB( this );
    _mediaDB->ensureSchema();
    _mediaDB->ensureDefaultData();
    _metadataHandler = new MediaMetadataHandler( this, this );
    _searchEngine = new SearchEngine( this, this );
}

Q_EXPORT_PLUGIN2( MediaLibraryModule, RedBullPlayer::Modules::MediaLibrary::MediaLibraryModule )
