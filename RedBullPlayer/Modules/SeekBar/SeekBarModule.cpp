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
#include "SeekBarModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>

// Project Includes
#include "../../Container/Error.h"
#include "../../Container/Container.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"


using namespace RedBullPlayer::Modules::SeekBar;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;


SeekBarModule::SeekBarModule() : ISeekBar() {
}

void SeekBarModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<RedBullPlayer::PlayerShell::Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new SeekBarUI( _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
}

void SeekBarModule::SetupModuleConnections() {
    resolveMediaLibrary();
    _model = new SeekBarModel( _mediaLibrary, this );
    _controller = new SeekBarController( _model, _ui, this );
}


void SeekBarModule::resolveMediaLibrary() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast PlaylistCreator Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find PlaylistCreator Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

Q_EXPORT_PLUGIN2( SeekBarModule, RedBullPlayer::Modules::SeekBar::SeekBarModule )
