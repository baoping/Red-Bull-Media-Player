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
#include "ChapterSelectModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>
#include <QBitmap>
#include <QPainter>
#include <QDir>
#include <QLabel>
#include <QUuid>

// Project Includes
#include "../../Container/Constants.h"
#include "../../Container/Container.h"
#include "../../Container/Error.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Modules::MediaLibrary;

ChapterSelectModule::ChapterSelectModule() : IChapterSelect() {
}

void ChapterSelectModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new ChapterSelectUI( _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
}

IMediaLibrary* ChapterSelectModule::resolveMediaLibrary() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast IMediaLibrary Plugin!" ), Error::Normal );
        }

        return retVal;
    } else {
        error->setErrorMessage( tr( "Could not find IMediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
    return 0;
}

void ChapterSelectModule::SetupModuleConnections() {
    _mediaLibrary = resolveMediaLibrary();
    _model = new ChapterSelectModel( _mediaLibrary, this );
    _controller = new ChapterSelectController( _shell,
            _region,
            _model,
            _ui,
            this );
}

Q_EXPORT_PLUGIN2( ChapterSelectModule, RedBullPlayer::Modules::ChapterSelect::ChapterSelectModule )
