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
#include "GridViewPlaylistModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>

// Project Includes
#include "../../Container/Error.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Playlist/PlaylistNavigationKeyPressedEvent.h"
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../Events/FocusedPlayListItemChangedEvent.h"


using namespace RedBullPlayer::Modules::Playlist::GridViewPlaylist;
using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;

GridViewPlaylistModule::GridViewPlaylistModule() : IGridViewPlaylist() {
}

void GridViewPlaylistModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new GridViewPlaylistUI( _shell, _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );

    _contentType = PlaylistContentTypeChangedEvent::MEDIA;
    _playlistManager = new PlaylistManager();
    Aggregator::instance()->registerListener( CentralUIRegionResized, this );
    Aggregator::instance()->registerListener( CentralFocusChanged, this );
    Aggregator::instance()->registerListener( PlaylistNavigationKeyPressed, this );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( CoverImageChanged, this );
    Aggregator::instance()->registerListener( PlayerExitClicked, this );
    Aggregator::instance()->registerListener( FocusedPlaylistItemChanged, this );
    Aggregator::instance()->registerListener( PlayerTrackStarted, this );
    Aggregator::instance()->registerListener( MediaItemContentChanged, this );
    Aggregator::instance()->registerListener( KeyPressed, this );
    Aggregator::instance()->registerListener( MediaDiscovered, this );
    Aggregator::instance()->registerListener( StartupPlaylistGenerationStarted, this );
    Aggregator::instance()->registerListener( StartupPlaylistGenerationFinished, this );
    Aggregator::instance()->registerListener( SearchPlaylistGenerationStarted, this );
    Aggregator::instance()->registerListener( SearchEnded, this );
    Aggregator::instance()->registerListener( CachedPlaylistRestored, this );
    Aggregator::instance()->registerListener( AddedItemToPlaylist, this );
    Aggregator::instance()->registerListener( MediaRemoved, this );
    Aggregator::instance()->registerListener( MediaVanished, this );
    Aggregator::instance()->registerListener( RemovedItemFromPlaylist, this );
    Aggregator::instance()->registerListener( DocumentOpened, this );
}

void GridViewPlaylistModule::SetupModuleConnections() {
    _playlistManager->setupMediaLibraryConnection();
    _ui->setupMediaLibraryConnection();
    _ui->resolveStyleFactory();
    _ui->setupStyle();
}

bool GridViewPlaylistModule::event ( QEvent * e ) {
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

    if( e->type() == CoverImageChanged ) {
        onCoverImageChanged( static_cast<CoverImageChangedEvent*>( e ) );
    }

    if( e->type() == PlayerExitClicked ) {
        onPlayerExitClicked( static_cast<PlayerExitClickedEvent*>( e ) );
    }

    if( e->type() == FocusedPlaylistItemChanged ) {
        onFocusedPlaylistItemChanged( static_cast<FocusedPlayListItemChangedEvent*>( e ) );
    }

    if( e->type() == PlayerTrackStarted ) {
        onPlayerTrackStarted( static_cast<PlayerTrackStartedEvent*>( e ) );
    }

    if( e->type() == MediaItemContentChanged ) {
        onMediaItemContentChanged( static_cast<MediaItemContentChangedEvent*>( e ) );
    }

    if( e->type() == MediaDiscovered ) {
        onMediaDiscovered( static_cast<MediaDiscoveredEvent*>( e ) );
    }

    if( e->type() == KeyPressed ) {
        onKeyPressed( static_cast<KeyPressedEvent*>( e ) );
    }

    if( e->type() == StartupPlaylistGenerationStarted ) {
        onStartupPlaylistGenerationStarted( static_cast<StartupPlaylistGenerationStartedEvent*>( e ) );
    }

    if( e->type() == StartupPlaylistGenerationFinished ) {
        onStartupPlaylistGenerationFinished( static_cast<StartupPlaylistGenerationFinishedEvent*>( e ) );
    }

    if( e->type() == SearchPlaylistGenerationStarted ) {
        onSearchPlaylistGenerationStarted( static_cast<SearchPlaylistGenerationStartedEvent*>( e ) );
    }

    if( e->type() == SearchEnded ) {
        onSearchEnded( static_cast<SearchEndedEvent*>( e ) );
    }

    if( e->type() == CachedPlaylistRestored ) {
        onCachedPlaylistRestored( static_cast<CachedPlaylistRestoredEvent*>( e ) );
    }

    if( e->type() == AddedItemToPlaylist ) {
        onAddedItemToPlaylist( static_cast<AddedItemToPlaylistEvent*>( e ) );
    }

    if( e->type() == MediaRemoved ) {
        onMediaRemoved( static_cast<MediaRemovedEvent*>( e ) );
    }

    if( e->type() == MediaVanished ) {
        onMediaVanished( static_cast<MediaVanishedEvent*>( e ) );
    }

    if( e->type() == RemovedItemFromPlaylist ) {
        onRemovedItemFromPlaylist( static_cast<RemovedItemFromPlaylistEvent*>( e ) );
    }

    if( e->type() == DocumentOpened ) {
        onDocumentOpened( static_cast<DocumentOpenedEvent*>( e ) );
    }

    return false;
}

void GridViewPlaylistModule::onDocumentOpened( DocumentOpenedEvent *e ) {
    _ui->handleDocumentOpened( e->mediaId() );
}

void GridViewPlaylistModule::onRemovedItemFromPlaylist( RemovedItemFromPlaylistEvent *e ) {
    _ui->handleRemovedItemFromPlaylist( e->mediaId() );
}

void GridViewPlaylistModule::onMediaRemoved( MediaRemovedEvent *e ) {
    _playlistManager->handleMediaRemoved( e->mediaId() );
}

void GridViewPlaylistModule::onMediaVanished( RedBullPlayer::Events::MediaVanishedEvent *e ) {
    _playlistManager->handleMediaVanished( e->mediaId() );
}

void GridViewPlaylistModule::onAddedItemToPlaylist( AddedItemToPlaylistEvent *e ) {
    _ui->addMediaItem( e->mediaId(), e->mediaSource() );
}

void GridViewPlaylistModule::onMediaDiscovered( MediaDiscoveredEvent *e ) {
    _playlistManager->handleMediaDisovered( e->mediaId(), e->source() );
}

void GridViewPlaylistModule::onCachedPlaylistRestored( CachedPlaylistRestoredEvent *e ) {
    _ui->handleCachedPlaylistRestored( e->playlist() );
}

void GridViewPlaylistModule::onSearchEnded( SearchEndedEvent *e ) {
    _playlistManager->handleSearchEnded();
}

void GridViewPlaylistModule::onSearchPlaylistGenerationStarted( SearchPlaylistGenerationStartedEvent *e ) {
    _playlistManager->handleSearchPlaylistGenerationStarted( e->playlist() );
    _ui->handleSearchPlaylistGenerationStarted( e->playlist() );
}

void GridViewPlaylistModule::onStartupPlaylistGenerationFinished( StartupPlaylistGenerationFinishedEvent *e ) {
    _playlistManager->handleStartupPlaylistGenerationFinished();
}

void GridViewPlaylistModule::onStartupPlaylistGenerationStarted( StartupPlaylistGenerationStartedEvent *e ) {
    _playlistManager->handleStartupPlaylistGenerationStarted( e->playlist() );
    _ui->setupPlaylist( e->playlist() );
}

void GridViewPlaylistModule::onKeyPressed( KeyPressedEvent *e ) {
    if( _ui->hasCentralFocus() && _ui->hasMainFocus() ) {
        if( e->keyEvent()->key() == Qt::Key_Enter || e->keyEvent()->key() == Qt::Key_Return ) {
            _ui->playCurrentItem();
        }

        if( e->keyEvent()->key() == Qt::Key_Backtab ) {
            _ui->selectPreviousItemAnimated();
        }

        if( e->keyEvent()->key() == Qt::Key_Tab ) {
            _ui->selectNextItemAnimated();
        }
    }
}

void GridViewPlaylistModule::onMediaItemContentChanged( MediaItemContentChangedEvent *e ) {
    _ui->handleMediaItemContentChangedEvent( e->mediaId() );
}

void GridViewPlaylistModule::onPlayerTrackStarted( PlayerTrackStartedEvent *e ) {
    _ui->handlePlayerTrackStarted( e->mediaId() );
}

void GridViewPlaylistModule::onFocusedPlaylistItemChanged( FocusedPlayListItemChangedEvent *e ) {
    if( e->playlistType() == FocusedPlayListItemChangedEvent::GRIDVIEW ||
            e->playlistType() == FocusedPlayListItemChangedEvent::LISTVIEW ||
            e->playlistType() == FocusedPlayListItemChangedEvent::COVERVIEW )
        _ui->handleFocusedPlaylistItemChanged( e->focusedItemIndex(), e->selectedByUser() );
}

void GridViewPlaylistModule::onPlayerExitClicked( PlayerExitClickedEvent *e ) {
    _ui->cancelAllCoverImageRequsts();
}

void GridViewPlaylistModule::onCoverImageChanged( CoverImageChangedEvent *e ) {
    _ui->handleCoverImageChanged( e->mediaId() );
}

void GridViewPlaylistModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *e ) {
    _ui->changeMainUIRegionFocusGraphics( e->uiRegion() );
}

void GridViewPlaylistModule::onNavigationKeyPressed( PlaylistNavigationKeyPressedEvent *e ) {
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

void GridViewPlaylistModule::onCentralUIRegionResized( CentralUIRegionResizedEvent *e ) {
    _ui->updateItems();
}

void GridViewPlaylistModule::onCentralFocusChanged( CentralFocusChangedEvent *e ) {
    _ui->handleCentralFocusChanged( e->widgetType() );
}

Q_EXPORT_PLUGIN2( GridViewPlaylistModule, RedBullPlayer::Modules::Playlist::GridViewPlaylist::GridViewPlaylistModule )


