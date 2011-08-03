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
#include "ListViewPlaylistModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>

// Project Includes
#include "../../Container/Error.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"

using namespace RedBullPlayer::Modules::Playlist::ListViewPlaylist;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;

ListViewPlaylistModule::ListViewPlaylistModule() : IListViewPlaylist() {}

void ListViewPlaylistModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new ListViewPlaylistUI( _shell, _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );

    _contentType = PlaylistContentTypeChangedEvent::MEDIA;

    Aggregator::instance()->registerListener( CentralUIRegionResized, this );
    Aggregator::instance()->registerListener( CentralFocusChanged, this );
    Aggregator::instance()->registerListener( PlaylistNavigationKeyPressed, this );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( FocusedPlaylistItemChanged, this );
    Aggregator::instance()->registerListener( MediaItemContentChanged, this );
    Aggregator::instance()->registerListener( KeyPressed, this );
    Aggregator::instance()->registerListener( AddedItemToPlaylist, this );
    Aggregator::instance()->registerListener( StartupPlaylistGenerationStarted, this );
    Aggregator::instance()->registerListener( SearchPlaylistGenerationStarted, this );
    Aggregator::instance()->registerListener( CachedPlaylistRestored, this );
    Aggregator::instance()->registerListener( RemovedItemFromPlaylist, this );
    Aggregator::instance()->registerListener( DocumentOpened, this );
}

void ListViewPlaylistModule::SetupModuleConnections() {
    _ui->setupMediaLibraryConnection();
    _ui->resolveStyleFactory();
    _ui->setupStyle();
}

bool ListViewPlaylistModule::event ( QEvent * e ) {
    if( e->type() == CentralUIRegionResized ) {
        onCentralUIRegionResized( static_cast<CentralUIRegionResizedEvent*>( e ) );
    }

    if( e->type() == CentralFocusChanged ) {
        onCentralFocusChanged( static_cast<CentralFocusChangedEvent*>( e ) );
    }

    if( e->type() == PlaylistNavigationKeyPressed ) {
        onNavigationKeyPressed( static_cast<PlaylistNavigationKeyPressedEvent*>( e ) );
    }

    if( e->type() == MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
    }

    if( e->type() == FocusedPlaylistItemChanged ) {
        onFocusedPlaylistItemChanged( static_cast<FocusedPlayListItemChangedEvent*>( e ) );
    }

    if( e->type() == MediaItemContentChanged ) {
        onMediaItemContentChanged( static_cast<MediaItemContentChangedEvent*>( e ) );
    }

    if( e->type() == AddedItemToPlaylist ) {
        onAddedItemToPlaylist( static_cast<AddedItemToPlaylistEvent*>( e ) );
    }

    if( e->type() == KeyPressed ) {
        onKeyPressed( static_cast<KeyPressedEvent*>( e ) );
    }

    if( e->type() == StartupPlaylistGenerationStarted ) {
        onStartupPlaylistGenerationStarted( static_cast<StartupPlaylistGenerationStartedEvent*>( e ) );
    }

    if( e->type() == SearchPlaylistGenerationStarted ) {
        onSearchPlaylistGenerationStarted( static_cast<SearchPlaylistGenerationStartedEvent*>( e ) );
    }

    if( e->type() == CachedPlaylistRestored ) {
        onCachedPlaylistRestored( static_cast<CachedPlaylistRestoredEvent*>( e ) );
    }

    if( e->type() == RemovedItemFromPlaylist ) {
        onRemovedItemFromPlaylist( static_cast<RemovedItemFromPlaylistEvent*>( e ) );
    }

    if( e->type() == DocumentOpened ) {
        onDocumentOpened( static_cast<DocumentOpenedEvent*>( e ) );
    }

    return false;
}

void ListViewPlaylistModule::onDocumentOpened( DocumentOpenedEvent *e ) {
    _ui->handleDocumentOpened( e->mediaId() );
}

void ListViewPlaylistModule::onRemovedItemFromPlaylist( RemovedItemFromPlaylistEvent *e ) {
    _ui->handleRemovedItemFromPlaylist( e->mediaId() );
}

void ListViewPlaylistModule::onCachedPlaylistRestored( CachedPlaylistRestoredEvent *e ) {
    _ui->handleCachedPlaylistRestored( e->playlist() );
}

void ListViewPlaylistModule::onSearchPlaylistGenerationStarted( SearchPlaylistGenerationStartedEvent *e ) {
    _ui->handleSearchPlaylistGenerationStarted( e->playlist() );
}

void ListViewPlaylistModule::onStartupPlaylistGenerationStarted( StartupPlaylistGenerationStartedEvent *e ) {
    _ui->setupPlaylist( e->playlist() );
}

void ListViewPlaylistModule::onKeyPressed( KeyPressedEvent *e ) {
    if( _ui->hasCentralFocus() && _ui->hasMainFocus() ) {
        if( e->keyEvent()->key() == Qt::Key_Enter || e->keyEvent()->key() == Qt::Key_Return ) {
            _ui->playCurrentItem();
        }

        if( e->keyEvent()->key() == Qt::Key_Backtab || e->keyEvent()->key() == Qt::Key_Up ) {
            _ui->selectPreviousItemAnimated();
        }

        if( e->keyEvent()->key() == Qt::Key_Tab || e->keyEvent()->key() == Qt::Key_Down ) {
            _ui->selectNextItemAnimated();
        }
    }
}

void ListViewPlaylistModule::onMediaItemContentChanged( MediaItemContentChangedEvent *e ) {
    _ui->handleMediaItemContentChangedEvent( e->mediaId() );
}

void ListViewPlaylistModule::onFocusedPlaylistItemChanged( FocusedPlayListItemChangedEvent *e ) {
    if( e->playlistType() == FocusedPlayListItemChangedEvent::GRIDVIEW ||
        e->playlistType() == FocusedPlayListItemChangedEvent::LISTVIEW ||
        e->playlistType() == FocusedPlayListItemChangedEvent::COVERVIEW )
        _ui->handleFocusedPlaylistItemChanged( e->focusedItemIndex(), e->selectedByUser() );
}

void ListViewPlaylistModule::onNavigationKeyPressed( PlaylistNavigationKeyPressedEvent *e ) {
    switch( e->direction() ) {
    case PlaylistNavigationKeyPressedEvent::LEFT : {
            _ui->scrollRight();
            break;
        }

    case PlaylistNavigationKeyPressedEvent::RIGHT : {
            _ui->scrollLeft();
            break;
        }
    }
}

void ListViewPlaylistModule::onCentralFocusChanged( CentralFocusChangedEvent *e ) {
    _ui->handleCentralFocusChanged( e->widgetType() );
}

void ListViewPlaylistModule::onAddedItemToPlaylist( AddedItemToPlaylistEvent *e ) {
    _ui->addMediaItem( e->mediaId(), e->mediaSource() );
}

void ListViewPlaylistModule::onCentralUIRegionResized( CentralUIRegionResizedEvent *e ) {
    _ui->updateItems();
}

void ListViewPlaylistModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *e ) {
    _ui->changeMainUIRegionFocusGraphics( e->uiRegion() );
}

Q_EXPORT_PLUGIN2( ListViewPlaylistModule, RedBullPlayer::Modules::Playlist::ListViewPlaylist::ListViewPlaylistModule )
