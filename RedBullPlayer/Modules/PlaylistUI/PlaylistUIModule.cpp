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
#include "PlaylistUIModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>
#include <QDesktopServices>

// Project Includes
#include "../../Container/Error.h"
#include "../../Container/Constants.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Playlist/PlaylistContentTypeChangedEvent.h"
#include "../../Interfaces/ICoverViewPlaylist.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::Playlist::CoverViewPlaylist;
using namespace RedBullPlayer::Tools;

PlaylistUIModule::PlaylistUIModule() : IPlaylistUI() {
}

void PlaylistUIModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new PlaylistUI( _shell, _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
    Aggregator::instance()->registerListener( KeyPressed, this );
    Aggregator::instance()->registerListener( FocusedPlaylistItemChanged, this );
    Aggregator::instance()->registerListener( FunctionSelected, this );
    Aggregator::instance()->registerListener( CentralFocusChanged, this );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( EndOfPlaylist, this );
    Aggregator::instance()->registerListener( SearchPlaylistGenerationStarted, this );
    Aggregator::instance()->registerListener( SearchEnded, this );
}

void PlaylistUIModule:: SetupModuleConnections() {
    _ui->setupMediaLibraryConnection();
    _ui->resolveStyleFactory();
    _ui->setupStyle();
}

bool PlaylistUIModule::event ( QEvent * e ) {
    if( e->type() == KeyPressed ) {
        onKeyPressed( static_cast<KeyPressedEvent*>( e ) );
    }

    if( e->type() ==  FocusedPlaylistItemChanged ) {
        onFocusedPlaylistItemChanged( static_cast<FocusedPlayListItemChangedEvent*>( e ) );
    }

    if( e->type() ==  FunctionSelected ) {
        onFunctionSelected( static_cast<FunctionSelectedEvent*>( e ) );
    }

    if( e->type() == CentralFocusChanged ) {
        onCentralFocusChanged( static_cast<CentralFocusChangedEvent*>( e ) );
    }

    if( e->type() ==  MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
    }

    if( e->type() == EndOfPlaylist ) {
        onEndOfPlaylist( static_cast<EndOfPlaylistEvent*>( e ) );
    }

    if( e->type() == SearchPlaylistGenerationStarted ) {
        onSearchPlaylistGenerationStarted( static_cast<SearchPlaylistGenerationStartedEvent*>( e ) );
    }

    return false;
}


void PlaylistUIModule::onSearchPlaylistGenerationStarted( SearchPlaylistGenerationStartedEvent *e ) {
    _ui->clearTitle();
}

void PlaylistUIModule::onKeyPressed( KeyPressedEvent *event ) {
    if( !_ui->hasMainFocus() || !_ui->hasCentralFocus() ) {
        return;
    }

    if( event->keyEvent()->key() == Qt::Key_Left ) {
        _ui->onLeftNavigationClicked();
    }

    if( event->keyEvent()->key() == Qt::Key_Right ) {
        _ui->onRightNavigationClicked();
    }

    if( event->keyEvent()->key() == Qt::Key_Backspace ||
            event->keyEvent()->key() == Qt::Key_Delete ||
            event->keyEvent()->key() == Qt::Key_Minus ) {
        _ui->minusClicked();
    }
}

void PlaylistUIModule::onFocusedPlaylistItemChanged( FocusedPlayListItemChangedEvent *event ) {
    _ui->handleFocusedPlaylistItemChanged( event->newTitle(), event->mediaId() );
}

void PlaylistUIModule::onEndOfPlaylist( EndOfPlaylistEvent *event ) {
    _ui->setNavigationKeysState( event->endOfPlayListDirection() );
}

void PlaylistUIModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *event ) {
    _ui->changeMainUIRegionFocusGraphics( event->uiRegion() );
}

void PlaylistUIModule::onFunctionSelected( FunctionSelectedEvent* e ) {
    if( e->functionTitle() != Constants::FUNCTION_SELECT_SERVUSTVB2B_TITLE )
        _ui->clearTitle();
    else
        QDesktopServices::openUrl( ConfigManager::instance()->getServusTvB2BUrl() );

    if( e->functionTitle() == Constants::FUNCTION_SELECT_MEDIA_TITLE ) {
        _ui->SetPlaylistType( PlaylistUI::MEDIA );
        PlaylistContentTypeChangedEvent e( PlaylistContentTypeChangedEvent::MEDIA );
        Aggregator::instance()->sendEvent( &e );
    }

    if( e->functionTitle() == Constants::FUNCTION_SELECT_TV_TITLE ) {
        _ui->SetPlaylistType( PlaylistUI::TV );
        PlaylistContentTypeChangedEvent e( PlaylistContentTypeChangedEvent::TV );
        Aggregator::instance()->sendEvent( &e );
    }

    if( e->functionTitle() == Constants::FUNCTION_SELECT_NEWS_TITLE ) {
        _ui->SetPlaylistType( PlaylistUI::NEWS );
        PlaylistContentTypeChangedEvent e( PlaylistContentTypeChangedEvent::NEWS );
        Aggregator::instance()->sendEvent( &e );
    }

    if( e->functionTitle() == Constants::FUNCTION_SELECT_ABOUT_TITLE ) {
        _ui->SetPlaylistType( PlaylistUI::ABOUT );
        PlaylistContentTypeChangedEvent e( PlaylistContentTypeChangedEvent::ABOUT );
        Aggregator::instance()->sendEvent( &e );
    }

    if( e->functionTitle() == Constants::FUNCTION_SELECT_SETTINGS_TITLE ) {
        _ui->SetPlaylistType( PlaylistUI::SETTINGS );
        PlaylistContentTypeChangedEvent e( PlaylistContentTypeChangedEvent::SETTINGS );
        Aggregator::instance()->sendEvent( &e );
    }
}

void PlaylistUIModule::onCentralFocusChanged( CentralFocusChangedEvent *e ) {
    _ui->handleCentralFocusChanged( e->widgetType() );
}

Q_EXPORT_PLUGIN2( PlaylistUIModule, RedBullPlayer::Modules::Playlist::PlaylistUIModule )
