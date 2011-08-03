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
#include "SettingsDetailViewModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>

// Project Includes
#include "../../Container/Error.h"
#include "../../Container/Container.h"
#include "../../Container/Constants.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Playlist/PlaylistNextItemSelectedEvent.h"
#include "../../Events/Playlist/PlaylistPreviousItemSelectedEvent.h"
#include "../../Events/UserSettings/UserSettingsChangedEvent.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

SettingsDetailViewModule::SettingsDetailViewModule() : ISettingsDetailView() {
}

void SettingsDetailViewModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<RedBullPlayer::PlayerShell::Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new SettingsDetailViewUI( _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
    _region->widget()->setStyleSheet( "background-color: transparent" );
    connect( _ui, SIGNAL( emitCentralFocusChanged( bool ) ), this, SLOT( emitCentralFocusChangedSlot( bool ) ) );
    connect( _ui, SIGNAL( emitPlaylistNextItemSelected() ), this, SLOT( emitNextPlaylistItemSelectedSlot() ) );
    connect( _ui, SIGNAL( emitPlaylistPreviousItemSelected() ), this, SLOT( emitPreviousPlaylistItemSelectedSlot() ) );
    connect( _ui, SIGNAL( emitMainRegionFocusChanged() ), this, SLOT( emitMainRegionFocusChangedSlot() ) );
    connect( _ui, SIGNAL( userSettingChanged() ), this, SLOT( emitUserSettingChangedSlot() ) );
    Aggregator::instance()->registerListener( PlaylistItemSelected, this );
    Aggregator::instance()->registerListener( FunctionSelected, this );
    Aggregator::instance()->registerListener( KeyPressed, this );
    Aggregator::instance()->registerListener( CentralFocusChanged, this );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( CentralUIRegionResized, this );
}

void SettingsDetailViewModule::SetupModuleConnections() {
    resolvePlaylistRegion();
    resolveStyleFactory();
    setupUiStyle();
    resolveMediaLibrary();
}

bool SettingsDetailViewModule::event ( QEvent * e ) {
    if( e->type() == PlaylistItemSelected ) {
        onPlaylistItemSelected( static_cast<PlaylistItemSelectedEvent*>( e ) );
    }

    if( e->type() == FunctionSelected ) {
        onFunctionSelected( static_cast<FunctionSelectedEvent*>( e ) );
    }

    if( e->type() == KeyPressed ) {
        onKeyPressed( static_cast<KeyPressedEvent*>( e ) );
    }

    if( e->type() == CentralFocusChanged ) {
        onCentralFocusChanged( static_cast<CentralFocusChangedEvent*>( e ) );
    }

    if( e->type() == MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
    }

    if( e->type() == CentralUIRegionResized ) {
        onCentralUIRegiontResized( static_cast<CentralUIRegionResizedEvent*>( e ) );
    }

    return false;
}

void SettingsDetailViewModule::onCentralFocusChanged( CentralFocusChangedEvent *event ) {
    _ui->handleCentralFocusChanged( event->widgetType() );
}

void SettingsDetailViewModule::onCentralUIRegiontResized( CentralUIRegionResizedEvent *event ) {
    _ui->handleCentralUiRegionResized();
}

void SettingsDetailViewModule::onFunctionSelected( FunctionSelectedEvent *event ) {
    _ui->handleFunctionSelected( event->functionTitle() );
}

void SettingsDetailViewModule::onKeyPressed( KeyPressedEvent *event ) {
    _ui->handleKeyPressed( event->keyEvent()->key() );
}

void SettingsDetailViewModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *event ) {
    _ui->handleMainUIRegionFocusChanged( event->uiRegion() );
}

void SettingsDetailViewModule::onPlaylistItemSelected( PlaylistItemSelectedEvent *event ) {
    if( event->playlistItem() == NULL
            || event->playlistItem()->getMedia() == NULL )
        return;

    if( event->playlistItem()->getMedia()->mediaType().name() == "settings" ) {
        _ui->handlePlaylistItemSelected( event->playlistItem()->getMedia() );
    }
}

void SettingsDetailViewModule::emitCentralFocusChangedSlot( bool hasCentralFocus ) {
    CentralFocusChangedEvent::CentralWidgetType widgetType;

    if( hasCentralFocus )
        widgetType = CentralFocusChangedEvent::SETTINGS_DETAIL;
    else
        widgetType = CentralFocusChangedEvent::SETTINGS_PLAYLIST;

    CentralFocusChangedEvent e( widgetType );
    Aggregator::instance()->sendEvent( &e );
}

void SettingsDetailViewModule::emitNextPlaylistItemSelectedSlot() {
    PlaylistNextItemSelectedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void SettingsDetailViewModule::emitPreviousPlaylistItemSelectedSlot() {
    PlaylistPreviousItemSelectedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void SettingsDetailViewModule::emitMainRegionFocusChangedSlot() {
    MainUIRegionFocusChangedEvent e( MainUIRegionFocusChangedEvent::CENTRAL );
    Aggregator::instance()->sendEvent( &e );
}

void SettingsDetailViewModule::emitUserSettingChangedSlot() {
    UserSettingsChangedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void SettingsDetailViewModule::resolvePlaylistRegion() {
    QWidget *playlistRegion = _shell->findRegion( "PlaylistRegion" )->widget();

    if( playlistRegion != NULL )
        _ui->setPlaylistRegionWidget( playlistRegion );
}

void SettingsDetailViewModule::resolveStyleFactory() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        _styleFactory = retVal;
        _ui->setStyleFactory( _styleFactory );
    } else {
        error->setErrorMessage( tr( "Could not find StyleFactory Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

void SettingsDetailViewModule::setupUiStyle() {
    _ui->setupStyle();
}

void SettingsDetailViewModule::resolveMediaLibrary() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
        _ui->setMediaLibrary( _mediaLibrary );
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

Q_EXPORT_PLUGIN2( SettingsDetailViewModule, RedBullPlayer::Modules::SettingsDetailView::SettingsDetailViewModule )
