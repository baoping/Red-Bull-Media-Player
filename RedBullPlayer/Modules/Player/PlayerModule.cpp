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
#include "PlayerModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QMovie>

// Project Includes
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"

//MVC
#include "PlayerView.h"

using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::Modules::StyleFactory;

PlayerModule::PlayerModule() :
    IPlayer() {
}

void PlayerModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<RedBullPlayer::PlayerShell::Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _audioAnimEngine = new AudioAnimationEngine( _region->widget(), this );
}

void PlayerModule::SetupModuleConnections() {
    resolveMediaLibrary();
    _styleFactory = resolveStyleFactory();
    QString styleSheet = "";

    if ( _styleFactory != 0 ) {
        bool ok;
        ITextStyle* style = _styleFactory->getTextStyle( "UserSettingsDialog_Text", &ok );

        if ( ok ) {
            styleSheet = style->styleSheet();
        }
    }

    _model = new PlayerModel( _mediaLibrary, this );
    _view = new PlayerView( _region->widget() );
    _region->widget()->layout()->addWidget( _view );
    _controller = new PlayerController( _shell,
                                        _region,
                                        _model,
                                        _view,
                                        this );
    _controller->setDialogStyleSheet( styleSheet );
}

void PlayerModule::cleanUp() {
}

void PlayerModule::resolveMediaLibrary() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast IMediaLibrary Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find IMediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

IStyleFactory* PlayerModule::resolveStyleFactory() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        return retVal;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
    return 0;
}

Q_EXPORT_PLUGIN2( PlayerModule, RedBullPlayer::Modules::Player::PlayerModule )
