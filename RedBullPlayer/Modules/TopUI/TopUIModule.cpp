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
#include "TopUIModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>
#include <QApplication>

// Project Includes
#include "TopUICreator.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/Player/PlayerExitClickedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/EventTypes.h"

using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::UpdateInstaller;
using namespace RedBullPlayer::Modules::StyleFactory;

TopUIModule::TopUIModule() : ITopUI() {
}

void TopUIModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new TopUICreator( _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
    connect( _ui, SIGNAL( exitClicked() ), this, SLOT( exitClickedSlot() ) );
    connect( _ui, SIGNAL( dragging( QPoint ) ), this, SLOT( draggingSlot( QPoint ) ) );
    connect( _ui, SIGNAL( doubleClicked() ), this, SLOT( doubleClickedSlot() ) );
}

void TopUIModule::SetupModuleConnections() {
    resolveMediaLibrary();
    resolveUpdateInstallerLibrary();
    resolveStyleFacctory();
    _ui->initStateDisplay( _styleFactory, _mediaLibrary );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
}

void TopUIModule::resolveMediaLibrary() {
    _mediaLibrary = 0;
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}
void TopUIModule::resolveUpdateInstallerLibrary() {
    _updateInstaller = 0;
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "UpdateInstaller" );

    if ( plugin ) {
        IUpdateInstaller* retVal = qobject_cast<IUpdateInstaller*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast UpdateInstaller Plugin!" ), Error::Normal );
        }

        _updateInstaller = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find UpdateInstaller Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

void TopUIModule::resolveStyleFacctory() {
    _styleFactory = NULL;
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        _styleFactory = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find StyleFactory Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}


void TopUIModule::exitClickedSlot() {
    qDebug() << Q_FUNC_INFO;

    if ( _updateInstaller->updateAvailable() ) {
        if( _shell->isFullScreen() )
            _shell->showNormal();

        _shell->hide();
        _updateInstaller->installUpdate();
        qDebug() << "Abborting exit due to update!";
        return;
    }

    qDebug() << "Sending PlayerExitClickedEvent!";
    PlayerExitClickedEvent e;
    Aggregator::instance()->sendEvent( &e );
    _shell->hide();
    QTimer::singleShot( 300, this, SLOT( closeShell() ) );
}

void TopUIModule::closeShell() {
    _shell->close();
}

void TopUIModule::draggingSlot( QPoint distance ) {
    QPoint newPos = _shell->pos() + distance;
    _shell->move( newPos );
}

void TopUIModule::doubleClickedSlot() {
    _shell->toggleFullscreen();
}

bool TopUIModule::event ( QEvent * e ) {
    if( e->type() ==  MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
    }

    return false;
}


void TopUIModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *event ) {
    _ui->changeMainUIRegionFocusGraphics( event->uiRegion() );
}


Q_EXPORT_PLUGIN2( TopUIModule, RedBullPlayer::Modules::TopUI::TopUIModule )

