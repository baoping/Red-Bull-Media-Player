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
#include "FunctionSelectModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>

// Project Includes
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/FunctionSelectedEvent.h"
#include "../../Events/EventTypes.h"
#include "../../Container/Constants.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"

using namespace RedBullPlayer::Modules::FunctionSelect;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;

FunctionSelectModule::FunctionSelectModule() : IFunctionSelect() {
}

void FunctionSelectModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new FunctionSelectUI( _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
    connect( _ui, SIGNAL( functionChanged( QString ) ), this, SLOT( functionChangedSlot( QString ) ) );
}

void FunctionSelectModule::SetupModuleConnections() {
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
    Aggregator::instance()->registerListener( KeyPressed, this );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( InternetConnectionStatusChanged, this );
}

void FunctionSelectModule::functionChangedSlot( QString newFunctionTitle ) {
    FunctionSelectedEvent e( newFunctionTitle );
    Aggregator::instance()->sendEvent( &e );
}

bool FunctionSelectModule::event ( QEvent * e ) {
    if( e->type() == KeyPressed ) {
        onKeyPressed( static_cast<KeyPressedEvent*>( e ) );
    }

    if( e->type() ==  MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
    }

    if( e->type() ==  InternetConnectionStatusChanged ) {
        onInternetConnectionStatusChanged( static_cast<InternetConnectionStatusChangedEvent*>( e ) );
    }

    return false;
}

void FunctionSelectModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *event ) {
    _ui->changeMainUIRegionFocusGraphics( event->uiRegion() );
}

void FunctionSelectModule::onKeyPressed( KeyPressedEvent *event ) {
    if( !_ui->hasMainFocus() ) {
        return;
    }

    if( event->keyEvent()->key() == Qt::Key_Enter || event->keyEvent()->key() == Qt::Key_Return ) {
        _ui->selectItemInTheMiddle();
    }

    if( event->keyEvent()->key() == Qt::Key_Left ) {
        _ui->onLeftNavigationClicked();
    }

    if( event->keyEvent()->key() == Qt::Key_Right ) {
        _ui->onRightNavigationClicked();
    }
}

void FunctionSelectModule::onInternetConnectionStatusChanged( InternetConnectionStatusChangedEvent *event ) {
    _ui->changeInternetConnectionStatus( event->internetAvailable() );
}

Q_EXPORT_PLUGIN2( FunctionSelectModule, RedBullPlayer::Modules::FunctionSelect::FunctionSelectModule )
