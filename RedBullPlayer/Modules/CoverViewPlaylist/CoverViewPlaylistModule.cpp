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
#include "CoverViewPlaylistModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>
#include <QThread>

// Project Includes
#include "../../Container/Error.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Playlist/PlaylistNavigationKeyPressedEvent.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::Playlist::CoverViewPlaylist;
using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;

CoverViewPlaylistModule::CoverViewPlaylistModule() : ICoverViewPlaylist() {
    _currentContentType = PlaylistContentTypeChangedEvent::MEDIA;
}

void CoverViewPlaylistModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    setInstanceName( config.attribute( "instanceName" ) );
    _ui = NULL;
    _contentType = PlaylistContentTypeChangedEvent::NONE;
    createInstance( "CoverViewPlaylistRegion", "MediaCoverViewPlaylist", PlaylistContentTypeChangedEvent::MEDIA );
    createInstance( "TVPlaylistRegion", "TVCoverViewPlaylist", PlaylistContentTypeChangedEvent::TV );
    createInstance( "NewsPlaylistRegion", "NewsCoverViewPlaylist", PlaylistContentTypeChangedEvent::NEWS );
    createInstance( "AboutPlaylistRegion", "AboutCoverViewPlaylist", PlaylistContentTypeChangedEvent::ABOUT );
    createInstance( "SettingsPlaylistRegion", "SettingsCoverViewPlaylist", PlaylistContentTypeChangedEvent::SETTINGS );
}

void CoverViewPlaylistModule::createInstance( QString region, QString instanceName, PlaylistContentTypeChangedEvent::ContentType contentType ) {
    CoverViewPlaylistModule *c = new CoverViewPlaylistModule();
    c->setInstanceName( instanceName );
    c->setRegion( _shell->findRegion( region ) );
    Q_ASSERT( c->_region != 0 );
    c->_ui = new CoverViewPlaylistUI( _shell, contentType, c->_region->widget() );
    c->_contentType = contentType;
    connect( c->_ui, SIGNAL( itemsRearranged() ), c, SLOT( emitFocusedItemChangedEvent() ) );

    if( c->_region->widget()->layout() == NULL ) {
        new QHBoxLayout( c->_region->widget() );
    }

    c->_region->widget()->layout()->setMargin( 0 );
    c->_region->widget()->layout()->setSpacing( 0 );
    c->_region->widget()->layout()->addWidget( c->_ui );
    Aggregator::instance()->registerListener( PlaylistNavigationKeyPressed, c );
    Aggregator::instance()->registerListener( CentralUIRegionResized, c );
    Aggregator::instance()->registerListener( PlaylistContentTypeChanged, c );
    Aggregator::instance()->registerListener( CentralFocusChanged, c );
    Aggregator::instance()->registerListener( PlaylistNextItemSelected, c );
    Aggregator::instance()->registerListener( PlaylistPreviousItemSelected, c );
    Aggregator::instance()->registerListener( KeyPressed, c );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, c );

    if( c->_contentType == PlaylistContentTypeChangedEvent::MEDIA ) {
        Aggregator::instance()->registerListener( CoverImageChanged, c );
        Aggregator::instance()->registerListener( FocusedPlaylistItemChanged, c );
        Aggregator::instance()->registerListener( MediaItemContentChanged, c );
        Aggregator::instance()->registerListener( AddedItemToPlaylist, c );
        Aggregator::instance()->registerListener( StartupPlaylistGenerationStarted, c );
        Aggregator::instance()->registerListener( SearchPlaylistGenerationStarted, c );
        Aggregator::instance()->registerListener( CachedPlaylistRestored, c );
        Aggregator::instance()->registerListener( RemovedItemFromPlaylist, c );
        Aggregator::instance()->registerListener( DocumentOpened, c );
    } else {
        Aggregator::instance()->registerListener( PlaylistCreated, c );
    }

    Container::Container::instance()->addModule( c );
}

void CoverViewPlaylistModule::SetupModuleConnections() {
    if( _ui != NULL )
        _ui->setupMediaLibraryConnection();
}

bool CoverViewPlaylistModule::event ( QEvent * e ) {
    if( e->type() == PlaylistNavigationKeyPressed ) {
        onNavigationKeyPressed( static_cast<PlaylistNavigationKeyPressedEvent*>( e ) );
    }

    if( e->type() == CentralUIRegionResized ) {
        onCentralUIRegionResized( static_cast<CentralUIRegionResizedEvent*>( e ) );
    }

    if( e->type() == PlaylistCreated ) {
        onPlaylistCreatedEvent( static_cast<PlaylistCreatedEvent*>( e ) );
    }

    if( e->type() == PlaylistContentTypeChanged ) {
        onPlaylistContentTypeChanged( static_cast<PlaylistContentTypeChangedEvent*>( e ) );
    }

    if( e->type() == CentralFocusChanged ) {
        onCentralFocusChangedEvent( static_cast<CentralFocusChangedEvent*>( e ) );
    }

    if( e->type() == CoverImageChanged ) {
        onCoverImageChanged( static_cast<CoverImageChangedEvent*>( e ) );
    }

    if( e->type() == FocusedPlaylistItemChanged ) {
        onFocusedPlaylistItemChanged( static_cast<FocusedPlayListItemChangedEvent*>( e ) );
    }

    if( e->type() == PlaylistNextItemSelected ) {
        onPlaylistNextItemSelected( static_cast<PlaylistNextItemSelectedEvent*>( e ) );
    }

    if( e->type() == PlaylistPreviousItemSelected ) {
        onPlaylistPreviousItemSelected( static_cast<PlaylistPreviousItemSelectedEvent*>( e ) );
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

    if( e->type() == MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
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

void CoverViewPlaylistModule::onDocumentOpened( DocumentOpenedEvent *e ) {
    _ui->handleDocumentOpened( e->mediaId() );
}

void CoverViewPlaylistModule::onRemovedItemFromPlaylist( RemovedItemFromPlaylistEvent *e ) {
    _ui->handleRemovedItemFromPlaylist( e->mediaId() );
}

void CoverViewPlaylistModule::onCachedPlaylistRestored( CachedPlaylistRestoredEvent *e ) {
    _ui->handleCachedPlaylistRestored( e->playlist() );
}

void CoverViewPlaylistModule::onSearchPlaylistGenerationStarted( SearchPlaylistGenerationStartedEvent *e ) {
    _ui->handleSearchPlaylistGenerationStarted( e->playlist() );
}

void CoverViewPlaylistModule::onStartupPlaylistGenerationStarted( StartupPlaylistGenerationStartedEvent *e ) {
    _ui->setupPlaylist( e->playlist() );
}

void CoverViewPlaylistModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *e ) {
    _ui->changeMainUIRegionFocusGraphics( e->uiRegion() );
}

void CoverViewPlaylistModule::onKeyPressed( KeyPressedEvent *e ) {
    if( _ui->hasMainFocus() && _ui->hasCentralFocus() ) {
        if( e->keyEvent()->key() == Qt::Key_Enter || e->keyEvent()->key() == Qt::Key_Return ) {
            _ui->playCurrentItem();
        }

        if( e->keyEvent()->key() == Qt::Key_Backtab ) {
            _ui->scrollRight();
        }

        if( e->keyEvent()->key() == Qt::Key_Tab ) {
            _ui->scrollLeft();
        }
    }
}

void CoverViewPlaylistModule::onAddedItemToPlaylist( AddedItemToPlaylistEvent *e ) {
    _ui->addMediaItem( e->mediaId(), e->mediaSource() );
}

void CoverViewPlaylistModule::onMediaItemContentChanged( MediaItemContentChangedEvent *e ) {
    _ui->handleMediaItemContentChangedEvent( e->mediaId() );
}

void CoverViewPlaylistModule::onFocusedPlaylistItemChanged( FocusedPlayListItemChangedEvent *e ) {
    if( e->playlistType() == FocusedPlayListItemChangedEvent::LISTVIEW ||
            e->playlistType() == FocusedPlayListItemChangedEvent::GRIDVIEW )
        _ui->handleFocusedPlaylistItemChanged( e->focusedItemIndex(), e->selectedByUser() );
}

void CoverViewPlaylistModule::onCoverImageChanged( CoverImageChangedEvent *e ) {
    _ui->handleCoverImageChanged( e->mediaId() );
}

void CoverViewPlaylistModule::onNavigationKeyPressed( PlaylistNavigationKeyPressedEvent *e ) {
    if( _currentContentType != _contentType ) {
        return;
    }

    _ui->handlePlaylistNavigationKeyPressed( e->direction() );
}

void CoverViewPlaylistModule::onCentralUIRegionResized( CentralUIRegionResizedEvent */*e*/ ) {
    _ui->updateItems();
}


void CoverViewPlaylistModule::onPlaylistCreatedEvent( PlaylistCreatedEvent *e ) {
    if( e->contentType() == _contentType ) {
        _ui->setupPlaylist( e->playlist() );
    }
}

void CoverViewPlaylistModule::onPlaylistContentTypeChanged( PlaylistContentTypeChangedEvent *e ) {
    _currentContentType = e->newType();

    if( _ui->hasCentralFocus() )
        emitFocusedItemChangedEvent();
}

void CoverViewPlaylistModule::onCentralFocusChangedEvent( CentralFocusChangedEvent *e ) {
    _ui->handleCentralFocusChanged( e->widgetType() );
}

void CoverViewPlaylistModule::setRegion( RedBullPlayer::PlayerShell::Region *region ) {
    _region = region;
}

void CoverViewPlaylistModule::emitFocusedItemChangedEvent() {
    IPlaylistItem *currentItem = _ui->getCurrentItem();

    if( _currentContentType != _contentType )
        return;

    FocusedPlayListItemChangedEvent::PlaylistType playlistType = FocusedPlayListItemChangedEvent::COVERVIEW;
    QString title = "";
    int currentIndex = 0;
    QUuid currentMediaId = QUuid();

    if( currentItem != NULL && currentItem->getMedia() != NULL ) {
        title = currentItem->getMedia()->playListTitle();
        currentIndex = _ui->getCurrentIndex();
        currentMediaId = _ui->getCurrentItem()->getMedia()->guid();
    }

    switch( _contentType ) {
        case PlaylistContentTypeChangedEvent::MEDIA:
            playlistType = FocusedPlayListItemChangedEvent::COVERVIEW;
            break;

        case PlaylistContentTypeChangedEvent::TV:
            playlistType = FocusedPlayListItemChangedEvent::TV;
            break;

        case PlaylistContentTypeChangedEvent::NEWS:
            playlistType = FocusedPlayListItemChangedEvent::NEWS;
            break;

        case PlaylistContentTypeChangedEvent::ABOUT:
            playlistType = FocusedPlayListItemChangedEvent::ABOUT;
            break;

        case PlaylistContentTypeChangedEvent::NONE:
            playlistType = FocusedPlayListItemChangedEvent::NONE;
            break;

        case PlaylistContentTypeChangedEvent::SETTINGS:
            break;

    }

    FocusedPlayListItemChangedEvent e( title, playlistType, currentIndex, _ui->hasUserSelectedItem(), currentMediaId );
    Aggregator::instance()->sendEvent( &e );
    emitEndOfPlaylist();
}

void CoverViewPlaylistModule::emitEndOfPlaylist() {
    _ui->emitEndOfPlaylist();
}

void CoverViewPlaylistModule::onPlaylistNextItemSelected( PlaylistNextItemSelectedEvent */*e*/ ) {
    if( _contentType != _currentContentType )
        return;

    if( _ui->isNewsAboutDetailOpen() ) {
        if( !_ui->isEnd() )
            _ui->scrollLeft();
    } else {
        _ui->setCurrentIndex( _ui->getCurrentIndex() + 1 );
        emitFocusedItemChangedEvent();
    }

    _ui->playCurrentItem( true, false );
}

void CoverViewPlaylistModule::onPlaylistPreviousItemSelected( PlaylistPreviousItemSelectedEvent */*e*/ ) {
    if( _contentType != _currentContentType )
        return;

    if( _ui->isNewsAboutDetailOpen() ) {
        if( !_ui->isBeginning() )
            _ui->scrollRight();
    }  else {
        _ui->setCurrentIndex( _ui->getCurrentIndex() - 1 );
        emitFocusedItemChangedEvent();
    }

    _ui->playCurrentItem( true, true );
}

RedBullPlayer::Modules::Playlist::IPlaylistItem *CoverViewPlaylistModule::getCurrentItem() {
    return _ui->getCurrentItem();
}

Q_EXPORT_PLUGIN2( CoverViewPlaylistModule, RedBullPlayer::Modules::Playlist::CoverViewPlaylist::CoverViewPlaylistModule )


