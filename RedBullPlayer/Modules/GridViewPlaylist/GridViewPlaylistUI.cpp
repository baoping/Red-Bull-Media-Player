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
#include "GridViewPlaylistUI.h"
#include "ui_GridViewPlaylistUI.h"
#include <math.h>

// Qt Includes
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

// Animation
#include "qtpropertyanimation.h"
#include "qtsequentialanimationgroup.h"

// Project Includes
#include "../../Container/Constants.h"
#include "../../Container/Container.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/Playlist/PlaylistItemSelectedEvent.h"
#include "../../Events/FocusedPlayListItemChangedEvent.h"
#include "../../Events/MediaLibrary/CoverImageRequestedEvent.h"
#include "../../Events/MediaLibrary/CancelCoverImageCreationEvent.h"
#include "../../Events/RequestAlbumCoverImageEvent.h"
#include "../../Events/Playlist/MediaItemContentChangedEvent.h"
#include "../../Events/Playlist/AddedItemToPlaylistEvent.h"
#include "../../Interfaces/IPlaylistItem.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"


using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;

GridViewPlaylistUI::GridViewPlaylistUI( Shell *shell, QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::GridViewPlaylistUI ) {
    _shell = shell;
    _parentWidget = parent;
    _hasCentralFocus = false;
    _widgetType = CentralFocusChangedEvent::GRIDVIEW_PLAYLIST;
    _hasMainUIRegionFocus = false;
    _mainUIRegionGroup = MainUIRegionFocusChangedEvent::CENTRAL;
    _currentPageIndex = 0;
    _currentItemIndex = 0;
    _curentItemMediaId = QUuid();
    _selectedItemInPreviousPlaylistId = QUuid();
    EASING_CURVE = QtEasingCurve::InOutCubic;
    _animationOn = false;
    _userSelectedItem = false;
    _mediaDB = 0;
    _isNewIcon = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_NEW_ICON );
    _numberColumnsFormat4To3 = ConfigManager::instance()->numberColumnsFormat4To3();
    _numberRowsFormat4To3 = ConfigManager::instance()->numberRowsFormat4To3();
    _numberColumnsFormat16To9 = ConfigManager::instance()->numberColumnsFormat16To9();
    _numberRowsFormat16To9 = ConfigManager::instance()->numberRowsFormat16To9();
    _ui->setupUi( this );
}

GridViewPlaylistUI::~GridViewPlaylistUI() {
    if( _playlistItems )
        delete _playlistItems;

    delete _ui;
}

void GridViewPlaylistUI::setupMediaLibraryConnection() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary *mediaLibrary = qobject_cast<IMediaLibrary*>( plugin );

        if ( mediaLibrary ) {
            _mediaDB = mediaLibrary->mediaDB();
        } else {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }
}

void GridViewPlaylistUI::resolveStyleFactory() {
    Error *error = new Error();
    Plugin* plugin = Container::instance()->findModule( "StyleFactory" );

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

void GridViewPlaylistUI::setupStyle() {
    if( _styleFactory == NULL )
        return;

    bool ok;
    _displayItemTextStyle = _styleFactory->getTextStyle( "GridView_FilenameText", &ok );
}

void GridViewPlaylistUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void GridViewPlaylistUI::resizeEvent( QResizeEvent *e ) {
    if( _playlistItems == NULL )
        return;

    _currentPageIndex = getPageIndexOfItem( _currentItemIndex );
    displayItems();
    emitEndOfPlaylist();
}

void GridViewPlaylistUI::handleCachedPlaylistRestored( QPointer<IPlaylist> cachedPlaylist ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );
    storeSelectedItemId();
    setupPlaylist( cachedPlaylist );
    setCurrentIndexToSelectedInPreviousPlaylistOrFirst();
}

void GridViewPlaylistUI::handleSearchPlaylistGenerationStarted( QPointer<IPlaylist> newPlaylist ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );
    storeSelectedItemId();
    setupPlaylist( newPlaylist );
}

void GridViewPlaylistUI::handleRemovedItemFromPlaylist( QUuid removedItemId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "remove Item: " + removedItemId.toString() );
    GridViewTransformWidget *widgetToRemove = 0;
    int removedItemIndex = -1;

    for( int i = 0; i < _playlistItemsWidgets.count() && widgetToRemove == 0; i++ ) {
        if ( _playlistItemsWidgets.at( i )->mediaGuid() == removedItemId ) {
            widgetToRemove = _playlistItemsWidgets.at( i );
            removedItemIndex = i;
        }
    }

    if ( ! widgetToRemove ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_COVERVIEWPLAYLIST, "No Widget foound to Remove for id: " + removedItemId.toString() );
        return;
    }

    if( removedItemIndex < 0 )
        return;

    if( removedItemIndex < _playlistItemsWidgets.count() && removedItemIndex >= 0 ) {
        if( _currentItemIndex == removedItemIndex ) {
            _playlistItemsWidgets.at( removedItemIndex )->unSelect();

            if( removedItemIndex == 0 && _playlistItemsWidgets.count() == 1 ) {
                _currentItemIndex = 0;
                _currentItemIndex = 0;
                _curentItemMediaId = QUuid();
            } else {
                if( removedItemIndex == _playlistItemsWidgets.count() - 1 ) {
                    _playlistItemsWidgets.at( removedItemIndex - 1 )->select();
                    _currentItemIndex = removedItemIndex - 1;
                } else {
                    _playlistItemsWidgets.at( removedItemIndex + 1 )->select();
                    _currentItemIndex = removedItemIndex;
                }

                _curentItemMediaId = _playlistItems->at( _currentItemIndex )->getMedia()->guid();
                _currentPageIndex =  getPageIndexOfItem( _currentItemIndex );
            }
        }

        if( _hasCentralFocus )
            emitFocusedItemChangedEvent( _currentItemIndex, false );

        _playlistItemsWidgets.removeAt( removedItemIndex );
        widgetToRemove->deleteLater();
        updateDisplayWidgetsId();

        if( _hasCentralFocus )
            updateItems();
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "_currentMediaId : " + _curentItemMediaId.toString() );
}

void GridViewPlaylistUI::addMediaItem( QUuid mediaId, MediaDiscoveredEvent::MediaDiscoveredSource source ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );
    IMedia *newMediaItem = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

    if( newMediaItem == NULL || _playlistItems == NULL )
        return;

    //append DisplayWidget inits
    refreshItemSize();
    int insertedItemIndex = _playlistItems->getItemIndexByGuid( newMediaItem->guid() );
    QUuid currentMediaTypeUid = newMediaItem->mediaType().guid();
    QString mediaTypeName = getMediaTypeName( currentMediaTypeUid );
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "HERE" + mediaId.toString() );

    //CoverImage Requests
    if( mediaTypeName == Constants::MEDIA_TYPE_VIDEO_NAME && newMediaItem->coverImage().isNull() ) {
        emitCoverImageRequestEvent( newMediaItem->guid() );
        _waitingItemsForCoverImage.insert( newMediaItem->guid(), 0 );
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "emit VIDEO Cover ImageRequest" );
    }

    if( mediaTypeName == Constants::MEDIA_TYPE_AUDIO_NAME && newMediaItem->coverImage().isNull() ) {
        _waitingItemsForCoverImage.insert( newMediaItem->guid(), 0 );
        emitRequestAlbumCover( newMediaItem->guid() );
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "emit AUDIO Cover ImageRequest" );
    }

    //add displayWidget
    bool isPodcast = _mediaDB->mediaRepository()->isOnlineContent( newMediaItem->guid() );
    QImage coverImage;

    if ( mediaTypeName == Constants::MEDIA_TYPE_DOCUMENT_NAME ) {
        if ( newMediaItem->coverImage().isNull() ) {
            QString fileName = newMediaItem->fileName();

            if ( ConfigManager::instance()->isPdfDocument( fileName ) ) {
                coverImage = QImage( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_PDF_THUMBNAIL );
            } else if ( ConfigManager::instance()->isPowerpointDocument( fileName ) ) {
                coverImage = QImage( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_POWERPOINT_THUMBNAIL );
            } else if ( ConfigManager::instance()->isWordDocument( fileName ) ) {
                coverImage = QImage( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_WORD_THUMBNAIL );
            } else if ( ConfigManager::instance()->isExcelDocument( fileName ) ) {
                coverImage = QImage( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_EXCEL_THUMBNAIL );
            } else if ( ConfigManager::instance()->isOtherDocument( fileName ) ) {
                coverImage = QImage( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_OTHER_THUMBNAIL );
            }
        } else
            coverImage = newMediaItem->coverImage();
    } else {
        coverImage = newMediaItem->coverImage();
    }

    GridViewTransformWidget *newDisplayWidget = new GridViewTransformWidget( _parentWidget,
            coverImage,
            mediaTypeName,
            newMediaItem,
            isPodcast,
            _hasMainUIRegionFocus,
            false,
            insertedItemIndex,
            _displayItemTextStyle );
    connect( newDisplayWidget, SIGNAL( doubleClicked( int ) ), this, SLOT( playlistItemDoubleClicked( int ) ) );
    connect( newDisplayWidget, SIGNAL( clicked( int ) ), this, SLOT( playlistItemClicked( int ) ) );
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "DisplayWidget Created" );

    if( source == MediaDiscoveredEvent::STARTUP_MEDIALIBRARY ||
            source == MediaDiscoveredEvent::SEARCH )
        _playlistItemsWidgets.append( newDisplayWidget );

    if( source == MediaDiscoveredEvent::STARTUP_FOLDER ||
            source == MediaDiscoveredEvent::POD_CATCHER ||
            source == MediaDiscoveredEvent::USER_ADD ) {
        _playlistItemsWidgets.insert( 0, newDisplayWidget );
        _currentItemIndex++;
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "DisplayWidget Added" );
    updateDisplayWidgetsId();
    setCurrentIndexAfterAddItem( source );

    if( !_animationOn && _hasCentralFocus )
        displayItems();

    if( _hasCentralFocus )
        emitEndOfPlaylist();

}

void GridViewPlaylistUI::setCurrentIndexAfterAddItem( MediaDiscoveredEvent::MediaDiscoveredSource source ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );

    if( _playlistItems == NULL )
        return;

    switch( source ) {
        case MediaDiscoveredEvent::SEARCH : {
                setCurrentIndexToSelectedInPreviousPlaylistOrFirst();
                break;
            }

        case MediaDiscoveredEvent::POD_CATCHER : {
                setCurrentIndexToUserSelectOrFirst();
                break;
            }

        case MediaDiscoveredEvent::STARTUP_MEDIALIBRARY : {
                setCurrentIndexToUserSelectOrFirst();
                break;
            }

        case MediaDiscoveredEvent::STARTUP_FOLDER : {
                setCurrentIndexToUserSelectOrFirst();
                break;
            }

        case MediaDiscoveredEvent::USER_ADD : {
                setCurrentIndexToUsedAddedItem();
                break;
            }
    }
}

void GridViewPlaylistUI::setCurrentIndexToUsedAddedItem() {
    selectItem( 0 );

    if( _hasCentralFocus )
        emitFocusedItemChangedEvent( 0, false ); //for title display ONLY
}

void GridViewPlaylistUI::setCurrentIndexToUserSelectOrFirst() {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );

    if( _userSelectedItem ) {
        _currentItemIndex = _playlistItems->getItemIndexByGuid( _curentItemMediaId );
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, QString( "userSelected : %1" ).arg( _currentItemIndex ) );
        _currentPageIndex = getPageIndexOfItem( _currentItemIndex );
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, QString( "defaultSelected : %1" ).arg( _currentItemIndex ) );
        selectItem( 0 );

        if( _hasCentralFocus )
            emitFocusedItemChangedEvent( 0, false ); //for title display ONLY
    }
}

void GridViewPlaylistUI::setCurrentIndexToSelectedInPreviousPlaylistOrFirst() {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "begin setCurrentIndexToSelectedInPreviousPlaylistOrFirst" );
    int indexOfItem = _playlistItems->getItemIndexByGuid( _selectedItemInPreviousPlaylistId );
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, QString( "indexOfCahedItem %1" ).arg( indexOfItem ) );

    if( indexOfItem < 0 )
        indexOfItem = 0; //selects first

    selectItem( indexOfItem );

    if( _hasCentralFocus )
        emitFocusedItemChangedEvent( indexOfItem, false );

    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "end setCurrentIndexToSelectedInPreviousPlaylistOrFirst" );
}


void GridViewPlaylistUI::storeSelectedItemId() {
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, QString( "Store Selected uid :" ) + _curentItemMediaId.toString() );
    _selectedItemInPreviousPlaylistId = _curentItemMediaId;
}

void GridViewPlaylistUI::updateDisplayWidgetsId() {
    for( int i = 0; i < _playlistItemsWidgets.count(); i++ )
        _playlistItemsWidgets.at( i )->setId( i );
}

void GridViewPlaylistUI::updateItems() {
    displayItems();
}

void GridViewPlaylistUI::setupPlaylist( QPointer<IPlaylist> playlist ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "begin Setup Playlist" );
    setPlaylist( playlist );

    if( _hasCentralFocus )
        emitEndOfPlaylist();

    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "end Setup Playlist" );
}

void GridViewPlaylistUI::setPlaylist( QPointer<IPlaylist> newPlaylist ) {
    clearItems();
    _playlistItems = newPlaylist;
    initItems();
    displayItems();
}

void GridViewPlaylistUI::clearItems() {
    for( int i = _playlistItemsWidgets.count() - 1; i >= 0; i-- ) {
        _playlistItemsWidgets.at( i )->deleteLater();
    }

    _playlistItemsWidgets.clear();
    _currentItemIndex = 0;
    _currentPageIndex = 0;
    _userSelectedItem = false;
}

void GridViewPlaylistUI::cancelAllCoverImageRequsts() {
    QHashIterator<QUuid, int> i( _waitingItemsForCoverImage );

    while ( i.hasNext() ) {
        i.next();
        emitCancelCoverImageCreationEvent( i.key() );
    }

    _waitingItemsForCoverImage.clear();
}

void GridViewPlaylistUI::initItems() {
    refreshItemSize();
    Q_ASSERT( _playlistItemsWidgets.count() == 0 );

    for( int i = 0; i < _playlistItems->count(); i++ ) {
        QUuid currentMediaTypeUid = _playlistItems->at( i )->getMedia()->mediaType().guid();
        QString mediaTypeName = getMediaTypeName( currentMediaTypeUid );

        if( mediaTypeName == Constants::MEDIA_TYPE_VIDEO_NAME && _playlistItems->at( i )->getMedia()->coverImage().isNull() ) {
            emitCoverImageRequestEvent( _playlistItems->at( i )->getMedia()->guid() );
            _waitingItemsForCoverImage.insert( _playlistItems->at( i )->getMedia()->guid(), i );
        }

        if( mediaTypeName == Constants::MEDIA_TYPE_AUDIO_NAME && _playlistItems->at( i )->getMedia()->coverImage().isNull() ) {
            _waitingItemsForCoverImage.insert( _playlistItems->at( i )->getMedia()->guid(), i );
            emitRequestAlbumCover( _playlistItems->at( i )->getMedia()->guid() );
        }

        bool isPodcast = _mediaDB->mediaRepository()->isOnlineContent( _playlistItems->at( i )->getMedia()->guid() );
        QImage coverImage;

        if ( mediaTypeName == Constants::MEDIA_TYPE_DOCUMENT_NAME ) {
            if ( _playlistItems->at( i )->getMedia()->coverImage().isNull() ) {
                QString fileName = _playlistItems->at( i )->getMedia()->fileName();

                if ( ConfigManager::instance()->isPdfDocument( fileName ) ) {
                    coverImage = QImage( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_PDF_THUMBNAIL );
                } else if ( ConfigManager::instance()->isPowerpointDocument( fileName ) ) {
                    coverImage = QImage( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_POWERPOINT_THUMBNAIL );
                } else if ( ConfigManager::instance()->isWordDocument( fileName ) ) {
                    coverImage = QImage( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_WORD_THUMBNAIL );
                } else if ( ConfigManager::instance()->isExcelDocument( fileName ) ) {
                    coverImage = QImage( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_EXCEL_THUMBNAIL );
                }
            }
        } else {
            coverImage = _playlistItems->at( i )->getMedia()->coverImage();
        }

        _playlistItemsWidgets.append( new GridViewTransformWidget( _parentWidget,
                                      coverImage,
                                      mediaTypeName,
                                      _playlistItems->at( i )->getMedia(),
                                      isPodcast,
                                      _hasMainUIRegionFocus,
                                      false,
                                      i,
                                      _displayItemTextStyle ) );
        connect( _playlistItemsWidgets.at( i ), SIGNAL( doubleClicked( int ) ), this, SLOT( playlistItemDoubleClicked( int ) ) );
        connect( _playlistItemsWidgets.at( i ), SIGNAL( clicked( int ) ), this, SLOT( playlistItemClicked( int ) ) );
    }
}

void GridViewPlaylistUI::emitAddedItemToPlaylist( QUuid mediaId, MediaDiscoveredEvent::MediaDiscoveredSource source ) {
    AddedItemToPlaylistEvent e( mediaId, source );
    Aggregator::instance()->sendEvent( &e );
}

void GridViewPlaylistUI::emitCoverImageRequestEvent( QUuid mediaId ) {
    CoverImageRequestedEvent e( mediaId );
    Aggregator::instance()->sendEvent( &e );
}

void GridViewPlaylistUI::emitRequestAlbumCover( QUuid mediaId ) {
    RequestAlbumCoverImageEvent ev( mediaId );
    Aggregator::instance()->sendEvent( &ev );
}

void GridViewPlaylistUI::emitCancelCoverImageCreationEvent( QUuid mediaId ) {
    CancelCoverImageCreationEvent e( mediaId );
    Aggregator::instance()->sendEvent( &e );
}

void  GridViewPlaylistUI::handleCoverImageChanged( QUuid mediaId ) {
    if( _playlistItems == NULL )
        return;

    int indexOfPlaylistItem = -1;

    if( _waitingItemsForCoverImage.contains( mediaId ) )
        indexOfPlaylistItem = _playlistItems->getItemIndexByGuid( mediaId );

    if( indexOfPlaylistItem >= 0 ) {
        IMedia *media = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

        if( media ) {
            _playlistItems->at( indexOfPlaylistItem )->setMedia( media );
            _playlistItemsWidgets.at( indexOfPlaylistItem )->changeCoverImage( media->coverImage() );
            _waitingItemsForCoverImage.remove( mediaId );
        }
    }
}

void GridViewPlaylistUI::handlePlayerTrackStarted( QUuid mediaId ) {
    if( _playlistItems == NULL )
        return;

    int itemIndex = _playlistItems->getItemIndexByGuid( mediaId );

    if( itemIndex >= 0 ) {
        IPlaylistItem *currentPlayingItem = _playlistItems->at( itemIndex );

        if( currentPlayingItem->getMedia()->isNew() ) {
            currentPlayingItem->getMedia()->setIsNew( false );
        }
    }
}

void GridViewPlaylistUI::handleDocumentOpened( QUuid mediaId ) {
    this->handleMediaItemContentChangedEvent( mediaId );
}

void GridViewPlaylistUI::emitMediaItemContentChanged( QUuid mediaId ) {
    MediaItemContentChangedEvent e( mediaId );
    Aggregator::instance()->sendEvent( &e );
}

void GridViewPlaylistUI::handleMediaItemContentChangedEvent( QUuid mediaId ) {
    if( _playlistItems == NULL )
        return;

    int indexOfPlaylistItem = _playlistItems->getItemIndexByGuid( mediaId );

    if( indexOfPlaylistItem >= 0 ) {
        IMedia *media = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

        if( media ) {
            _playlistItems->at( indexOfPlaylistItem )->setMedia( media );
            _playlistItemsWidgets.at( indexOfPlaylistItem )->refreshMediaItemMembers( media, true );
        }
    }
}

void GridViewPlaylistUI::displayItems() {
    if( _animationOn )
        return;

    refreshItemSize();

    for( int i = 0; i < _playlistItemsWidgets.count(); i++ ) {
        _playlistItemsWidgets.at( i )->hide();
        displayItem( _playlistItemsWidgets.at( i ), i );
    }
}

void GridViewPlaylistUI::displayItem( GridViewTransformWidget *item, int indexInList ) {
    int navigationMargin = Constants::PLAYLISTUI_ARROW_MARGIN + Constants::PLAYLISTUI_ARROW_WIDTH + Constants::GRIDVIEW_PLAYLIST_DISTANCE_FROM_NAVIGATION_KEY;
    int offsetXAxis = ( getPageIndexOfItem( indexInList ) - _currentPageIndex ) * ( Constants::GRIDVIEW_PLAYLIST_DISTANCE_BETWEEN_PAGES + _parentWidget->width() );
    int columnIndex = ( indexInList % _numberColumnsCurrent );
    int rowIndex = ( ( indexInList - ( getPageIndexOfItem( indexInList ) * _numberColumnsCurrent * _numberRowsCurrent ) ) / _numberColumnsCurrent );
    item->setGeometry( navigationMargin + offsetXAxis + columnIndex * _itemWidth,
                       rowIndex * ( _itemHeight + _spaceBetweenRows ),
                       _itemWidth, _itemHeight );
    int pageIndexOfItem = getPageIndexOfItem( indexInList );

    if( pageIndexOfItem >= _currentPageIndex - 1 && pageIndexOfItem <= _currentPageIndex + 1 )
        item->show();
}

void GridViewPlaylistUI::refreshItemSize() {
    int playlistMarginLeft =  Constants::PLAYLISTUI_ARROW_MARGIN +
                              Constants::PLAYLISTUI_ARROW_WIDTH +
                              Constants::GRIDVIEW_PLAYLIST_DISTANCE_FROM_NAVIGATION_KEY;
    int currentWidth = _parentWidget->width();
    int currentHeight = _parentWidget->height();
    int isNewIconHangingUpHeight = 0;
    int isNewIconWidth = 0;
    refreshDisplayFormat( currentWidth, currentHeight );
    _itemWidth = ( int )( ( currentWidth  - ( 2 * playlistMarginLeft ) ) / _numberColumnsCurrent );
    isNewIconWidth = ( int )( Constants::GRIDVIEW_PLAYLIST_ITEM_ISNEW_ICON_WIDTH_PROCENT_FROM_THUMBNAIL_WIDTH * ( _itemWidth - 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING ) / 100 );
    isNewIconHangingUpHeight = ( int )( _isNewIcon.scaledToWidth( isNewIconWidth, Qt::FastTransformation ).height() / 2 );
    _itemHeight = ( int )( ( 1 + ( 0.01 * Constants::GRIDVIEW_PLAYLIST_ITEM_TITLE_HEIGHT_PROCENT ) ) * ( _itemWidth - 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING ) +  2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING );
    _itemHeight += isNewIconHangingUpHeight;
    _spaceBetweenRows = ( int )( ( currentHeight - ( _numberRowsCurrent * _itemHeight ) ) / _numberRowsCurrent );
}

void GridViewPlaylistUI::refreshDisplayFormat( int width, int height ) {
    if( ( width / height ) < (16/9) ) {
        _numberRowsCurrent = _numberRowsFormat4To3;
        _numberColumnsCurrent = _numberColumnsFormat4To3;
        _currentDisplayFormatType = GridViewPlaylistUI::FOUR_TO_THREE;
    } else {
        _numberRowsCurrent = _numberRowsFormat16To9;
        _numberColumnsCurrent = _numberColumnsFormat16To9;
        _currentDisplayFormatType = GridViewPlaylistUI::SIXTEEN_TO_NINE;
    }
}

void GridViewPlaylistUI::playlistItemDoubleClicked( int id ) {
    if( _playlistItems == NULL )
        return;

    PlaylistItemSelectedEvent e( _playlistItems->at( id ) );
    Aggregator::instance()->sendEvent( &e );
}

void GridViewPlaylistUI::playlistItemClicked( int id ) {
    emitFocusedItemChangedEvent( id, true );
}

void GridViewPlaylistUI::handleFocusedPlaylistItemChanged( int newItemIndex, bool selectedByUser ) {
    if( _playlistItems == NULL )
        return;

    if( _playlistItemsWidgets.count() <= 0 )
        return;

    if( _playlistItems->count() <= 0 )
        return;

    if( !selectedByUser )
        return;

    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, Q_FUNC_INFO );

    if( _currentItemIndex < _playlistItemsWidgets.count() )
        _playlistItemsWidgets.at( _currentItemIndex )->unSelect();

    _playlistItemsWidgets.at( newItemIndex )->select();
    _currentItemIndex = newItemIndex;
    _currentPageIndex =  getPageIndexOfItem( _currentItemIndex );
    _curentItemMediaId = _playlistItems->at( newItemIndex )->getMedia()->guid();
    SimpleLogger::instance()->debug( Constants::LOGGER_GRIDVIEWPLAYLIST, "current item Id : " + _curentItemMediaId.toString() );

    if( _hasCentralFocus )
        displayItems();

    _userSelectedItem = selectedByUser;
}

void GridViewPlaylistUI::emitFocusedItemChangedEvent( int newItemIndex, bool selectedByUser ) {
    QString title = "";
    IPlaylistItem *currentItem = _playlistItems->at( newItemIndex );
    QUuid mediaId = QUuid();

    if( currentItem != NULL && currentItem->getMedia() != NULL ) {
        title = currentItem->getMedia()->playListTitle();
        mediaId = currentItem->getMedia()->guid();
    }

    FocusedPlayListItemChangedEvent e( title, FocusedPlayListItemChangedEvent::GRIDVIEW, newItemIndex, selectedByUser, mediaId );
    Aggregator::instance()->sendEvent( &e );
}

void GridViewPlaylistUI::selectNextItemAnimated() {
    if( _currentItemIndex == ( _playlistItemsWidgets.count() - 1 ) )
        return;

    int newPageIndedx = getPageIndexOfItem( _currentItemIndex + 1 );

    if( newPageIndedx > _currentPageIndex )
        scrollLeft();

    emitFocusedItemChangedEvent( _currentItemIndex + 1, true );
}

void GridViewPlaylistUI::selectPreviousItemAnimated() {
    if( _currentItemIndex == 0 )
        return;

    int newPageIndedx = getPageIndexOfItem( _currentItemIndex - 1 );

    if( newPageIndedx < _currentPageIndex )
        scrollRight();

    emitFocusedItemChangedEvent( _currentItemIndex - 1, true );
}

void GridViewPlaylistUI::selectItem( int itemIndex ) {
    if( _playlistItemsWidgets.count() == 0 )
        return;

    if( _currentItemIndex < _playlistItemsWidgets.count() )
        _playlistItemsWidgets.at( _currentItemIndex )->unSelect();

    _playlistItemsWidgets.at( itemIndex )->select();
    _currentItemIndex = itemIndex;
    _currentPageIndex =  getPageIndexOfItem( _currentItemIndex );
    _curentItemMediaId = _playlistItems->at( itemIndex )->getMedia()->guid();
    displayItems();
}

void GridViewPlaylistUI::handleCentralFocusChanged( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    if( _widgetType == widgetType ) {
        _hasCentralFocus = true;
        displayItems();
        emitEndOfPlaylist();
    } else {
        _hasCentralFocus = false;
    }
}

void GridViewPlaylistUI::changeMainUIRegionFocusGraphics( MainUIRegionFocusChangedEvent::UIRegion mainUIRegion ) {
    if( _mainUIRegionGroup == mainUIRegion )
        _hasMainUIRegionFocus = true;
    else
        _hasMainUIRegionFocus = false;

    for( int i = 0; i < _playlistItemsWidgets.count(); i++ ) {
        _playlistItemsWidgets.at( i )->changeMainUIRegionFocusGraphics( _hasMainUIRegionFocus );
    }
}

void GridViewPlaylistUI::scrollLeft() {
    if( _hasCentralFocus && !_animationOn ) {
        animateScroll( true );
        emitEndOfPlaylist();
    }
}

void GridViewPlaylistUI::scrollRight() {
    if( _hasCentralFocus && !_animationOn ) {
        animateScroll( false );
        emitEndOfPlaylist();
    }
}

void GridViewPlaylistUI::emitEndOfPlaylist() {
    EndOfPlaylistEvent::EndOfPlaylistDirection end = EndOfPlaylistEvent::NONE;

    if( isBeginning() ) {
        end = EndOfPlaylistEvent::LEFT;
    }

    if( isEnd() ) {
        end = EndOfPlaylistEvent::RIGHT;
    }

    if( isBeginning() && isEnd() ) {
        end = EndOfPlaylistEvent::BOTH;
    }

    EndOfPlaylistEvent e( end );
    Aggregator::instance()->sendEvent( &e );
}

bool GridViewPlaylistUI::isBeginning() {
    if( _playlistItems == NULL ) {
        return true;
    }

    return _currentPageIndex == 0 && _playlistItems->count() >= 0;
}

bool GridViewPlaylistUI::isEnd() {
    if( _playlistItems == NULL ) {
        return true;
    }

    return _currentPageIndex == getPageIndexOfItem( _playlistItems->count() - 1 ) && _playlistItems->count() >= 0;
}

void GridViewPlaylistUI::animateScroll( bool scrollLeft ) {
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    int animatedItemsStartIndex = 0;
    int animatedItemsEndIndex = 0;
    refreshItemSize();
    _animationOn = true;
    group->addAnimation( getScrollAnimationForPage( _currentPageIndex, scrollLeft, Constants::GRIDVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH ) );

    if( scrollLeft ) {
        group->addAnimation( getScrollAnimationForPage( _currentPageIndex + 1, scrollLeft, Constants::GRIDVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH ) );
        animatedItemsStartIndex = getStartItemIndexOfPage( _currentPageIndex );
        animatedItemsEndIndex = getEndItemIndexOfPage( _currentPageIndex + 1 );
        _currentPageIndex++;
    } else {
        group->addAnimation( getScrollAnimationForPage( _currentPageIndex - 1, scrollLeft, Constants::GRIDVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH ) );
        animatedItemsStartIndex = getStartItemIndexOfPage( _currentPageIndex - 1 );
        animatedItemsEndIndex = getEndItemIndexOfPage( _currentPageIndex );
        _currentPageIndex--;
    }

    connect( group, SIGNAL( finished() ), this, SLOT( animationFinishedSlot() ) );
    group->start();

    for( int i = 0; i < animatedItemsStartIndex; i++ ) {
        _playlistItemsWidgets.at( i )->hide();
        displayItem( _playlistItemsWidgets.at( i ), i );
    }

    for( int i = animatedItemsEndIndex + 1; i < _playlistItemsWidgets.count(); i++ ) {
        _playlistItemsWidgets.at( i )->hide();
        displayItem( _playlistItemsWidgets.at( i ), i );
    }
}

QtParallelAnimationGroup *GridViewPlaylistUI::getScrollAnimationForPage( int pageIndex, bool scrollLeft, int animationLength ) {
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    int offsetX = ( Constants::GRIDVIEW_PLAYLIST_DISTANCE_BETWEEN_PAGES + _parentWidget->width() );

    if( scrollLeft )
        offsetX *= -1;

    for( int i = getStartItemIndexOfPage( pageIndex ); i <= getEndItemIndexOfPage( pageIndex ) ; i++ ) {
        QtPropertyAnimation *aPos = new QtPropertyAnimation( _playlistItemsWidgets.at( i ), "pos", _parentWidget );
        aPos->setDuration( animationLength );
        aPos->setEasingCurve( EASING_CURVE );
        aPos->setStartValue( QPoint( _playlistItemsWidgets.at( i )->pos().x(), _playlistItemsWidgets.at( i )->pos().y() ) );
        aPos->setEndValue( QPoint( _playlistItemsWidgets.at( i )->pos().x() + offsetX,  _playlistItemsWidgets.at( i )->pos().y() ) );
        group->addAnimation( aPos );
    }

    return group;
}

int GridViewPlaylistUI::getPageIndexOfItem( int indexInList ) {
    if( _numberColumnsCurrent * _numberRowsCurrent != 0 )
        return ( int )( indexInList / ( _numberColumnsCurrent * _numberRowsCurrent ) );
    else
        return 0;
}

int GridViewPlaylistUI::getStartItemIndexOfPage( int pageIndex ) {
    return _numberColumnsCurrent * _numberRowsCurrent * pageIndex;
}

int GridViewPlaylistUI::getEndItemIndexOfPage( int pageIndex ) {
    int returnIndex = _numberColumnsCurrent * _numberRowsCurrent * ( ++pageIndex ) - 1;

    if( returnIndex > _playlistItemsWidgets.count() - 1 )
        returnIndex = _playlistItemsWidgets.count() - 1;

    return returnIndex;
}

void GridViewPlaylistUI::animationFinishedSlot() {
    _animationOn = false;
}

QString GridViewPlaylistUI::getMediaTypeName( QUuid mediaTypeId ) {
    if( mediaTypeId == _mediaDB->videoType()->guid() )
        return Constants::MEDIA_TYPE_VIDEO_NAME;

    if( mediaTypeId == _mediaDB->audioType()->guid() )
        return Constants::MEDIA_TYPE_AUDIO_NAME;

    if( mediaTypeId == _mediaDB->imageType()->guid() )
        return Constants::MEDIA_TYPE_IMAGE_NAME;

    if ( mediaTypeId == _mediaDB->documentType()->guid() )
        return Constants::MEDIA_TYPE_DOCUMENT_NAME;

    return "";
}

bool GridViewPlaylistUI::hasCentralFocus() {
    return _hasCentralFocus;
}

bool GridViewPlaylistUI::hasMainFocus() {
    return _hasMainUIRegionFocus;
}

void GridViewPlaylistUI::playCurrentItem() {
    playlistItemDoubleClicked( _currentItemIndex );
}

