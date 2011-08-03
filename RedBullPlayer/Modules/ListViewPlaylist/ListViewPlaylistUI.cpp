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
#include "ListViewPlaylistUI.h"
#include "ui_ListViewPlaylistUI.h"

// Qt Includes
#include <QPainter>
#include <QDateTime>

// QtAnimationFramework
#include "qtpropertyanimation.h"
#include "qtsequentialanimationgroup.h"


// Project Includes
#include "../../Container/Constants.h"
#include "../../Container/Container.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/Playlist/PlaylistItemSelectedEvent.h"
#include "../../Events/FocusedPlayListItemChangedEvent.h"
#include "../../Events/MediaLibrary/CoverImageRequestedEvent.h"
#include "../../Events/MediaLibrary/CancelCoverImageCreationEvent.h"
#include "../../Events/RequestAlbumCoverImageEvent.h"
#include "../../Events/Playlist/MediaItemContentChangedEvent.h"
#include "../../Events/Playlist/AddedItemToPlaylistEvent.h"
#include "../../Interfaces/IPlaylistItem.h"


using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;

ListViewPlaylistUI::ListViewPlaylistUI( RedBullPlayer::PlayerShell::Shell *shell, QWidget *parent ) :
        QWidget( parent ),
        _ui( new Ui::ListViewPlaylistUI ) {
    _shell = shell;
    _parentWidget = parent;
    _ui->setupUi( this );
    _hasCentralFocus = false;
    _widgetType = CentralFocusChangedEvent::LISTVIEW_PLAYLIST;
    _hasMainUIRegionFocus = false;
    _mainUIRegionGroup = MainUIRegionFocusChangedEvent::CENTRAL;
    _currentPageIndex = 0;
    _currentItemIndex = 0;
    _curentItemMediaId = QUuid();
    _selectedItemInPreviousPlaylistId = QUuid();
    _currentNumberRows = 0;
    EASING_CURVE = QtEasingCurve::InOutCubic;
    _animationOn = false;
    _userSelectedItem = false;
    _mediaDB = 0;
    _itemHeight = Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT;
    _spaceBetweenPages = Constants::LISTVIEW_PLAYLIST_SPACE_BETWEEN_PAGES;
    _imageHeaderBackground = QPixmap( Constants::LISTVIEW_PLAYLIST_IMAGE_DIR + Constants::LISTVIEW_PLAYLIST_HEADER_BACKGROUND_IMAGE );
    _noMainRegionFocusPixmap = QPixmap( Constants::LISTVIEW_PLAYLIST_IMAGE_DIR + Constants::LISTVIEW_PLAYLIST_MAINFOCUS_OVERLAY );

    initIsNewIconOffsetX();
}

ListViewPlaylistUI::~ListViewPlaylistUI() {
    delete _ui;
}

void ListViewPlaylistUI::setupMediaLibraryConnection() {
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

void ListViewPlaylistUI::resolveStyleFactory() {
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

void ListViewPlaylistUI::setupStyle() {
    if( _styleFactory == NULL )
        return;

    bool ok;
    _displayItemTextNormalStyle = _styleFactory->getTextStyle( "ListView_Text", &ok );
    _displayItemTextSelectedStyle = _styleFactory->getTextStyle( "ListView_Text_markiert", &ok );
    setupHeaderTextStyle( _styleFactory->getTextStyle( "ListView_Spalten_HeaderText", &ok ) );
}

void ListViewPlaylistUI::setupHeaderTextStyle( ITextStyle *headerTextStyle ) {
    _headerTextColor = headerTextStyle->foreColor();
    QFont::Weight fontWeight = QFont::Normal;

    if( headerTextStyle->bold() )
        fontWeight = QFont::Bold;

    _headerTextFont = QFont( headerTextStyle->fontName(), headerTextStyle->pointSize(), fontWeight, headerTextStyle->italic() );
    _headerTextFont.setPixelSize( headerTextStyle->pointSize() );
}

void ListViewPlaylistUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
    case QEvent::LanguageChange:
        _ui->retranslateUi( this );
        break;

    default:
        break;
    }
}

void ListViewPlaylistUI::resizeEvent( QResizeEvent *e ) {
    updateItems();
}

void ListViewPlaylistUI::paintEvent( QPaintEvent *e ) {
    QPainter p( this );
    int playlistMargin = Constants::PLAYLISTUI_ARROW_MARGIN + Constants::PLAYLISTUI_ARROW_WIDTH + Constants::LISTVIEW_PLAYLIST_PADDING_TO_NAVIGATION_ARROW;
    int currentWidth = width() - 2 * playlistMargin;
    int headerTextOffsetX = playlistMargin + ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ICON * currentWidth / 100 );

    //header Background drawing
    /*if(!_imageHeaderBackground.isNull())
    {
        for(int i = playlistMargin;i < width() - playlistMargin; i++)
        {
            p.drawPixmap(i,0,1,Constants::PLAYLISTUI_ARROW_HEIGHT,_imageHeaderBackground);
        }
    }*/

    //draw Text Header
    p.setPen( _headerTextColor );
    p.setFont( _headerTextFont );
    QFontMetrics fontMetrics = p.fontMetrics();

    //draw Title text
    int currentWidthColumnTitle = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_TITLE * currentWidth / 100 );
    p.drawText( headerTextOffsetX, 0,
                currentWidthColumnTitle - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::PLAYLISTUI_ARROW_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( Constants::LISTVIEW_PLAYLIST_COLUMN_TITLE_HEADER_TEXT, Qt::ElideRight,
                                        currentWidthColumnTitle - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );

    //draw Artist text
    headerTextOffsetX += currentWidthColumnTitle;
    int currentWidthColumnArtist = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ARTIST  * currentWidth / 100 );
    p.drawText( headerTextOffsetX, 0,
                currentWidthColumnArtist - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::PLAYLISTUI_ARROW_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( Constants::LISTVIEW_PLAYLIST_COLUMN_ARTIST_HEADER_TEXT, Qt::ElideRight,
                                        currentWidthColumnArtist - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );

    //draw Album text
    headerTextOffsetX += currentWidthColumnArtist;
    int currentWidthColumnAlbum = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ALBUM * currentWidth / 100 );
    p.drawText( headerTextOffsetX, 0,
                currentWidthColumnAlbum - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::PLAYLISTUI_ARROW_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( Constants::LISTVIEW_PLAYLIST_COLUMN_ALBUM_HEADER_TEXT, Qt::ElideRight,
                                        currentWidthColumnTitle - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );

    //draw CreatedAt text
    headerTextOffsetX += currentWidthColumnAlbum;
    int currentWidthColumnCreatedAt = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_CREATED_AT * currentWidth / 100 );
    p.drawText( headerTextOffsetX, 0,
                currentWidthColumnCreatedAt - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::PLAYLISTUI_ARROW_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( Constants::LISTVIEW_PLAYLIST_COLUMN_CREATEDAT_HEADER_TEXT, Qt::ElideRight,
                                        currentWidthColumnCreatedAt - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );

    //draw Length text
    headerTextOffsetX += currentWidthColumnCreatedAt;
    int currentWidthColumnLength = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_LENGTH * currentWidth / 100 );
    p.drawText( headerTextOffsetX, 0,
                currentWidthColumnLength  - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::PLAYLISTUI_ARROW_HEIGHT,
                Qt::AlignRight | Qt::AlignVCenter,
                fontMetrics.elidedText( Constants::LISTVIEW_PLAYLIST_COLUMN_LENGTH_HEADER_TEXT, Qt::ElideRight,
                                        currentWidthColumnLength  - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );

    //draw Download Column
    headerTextOffsetX += currentWidthColumnLength;
    int currentWidthColumnDownload = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_DOWNLOAD * currentWidth / 100 );
    p.drawText( headerTextOffsetX, 0,
                currentWidthColumnDownload, Constants::PLAYLISTUI_ARROW_HEIGHT,
                Qt::AlignHCenter | Qt::AlignVCenter,
                fontMetrics.elidedText( Constants::LISTVIEW_PLAYLIST_COLUMN_DOWNLOAD_HEADER_TEXT, Qt::ElideRight, currentWidthColumnDownload ) );

    //draw FileName Column
    headerTextOffsetX += currentWidthColumnDownload;
    int currentWidthColumnFilename = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_FILENAME  * currentWidth / 100 );
    p.drawText( headerTextOffsetX, 0,
                currentWidthColumnFilename - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::PLAYLISTUI_ARROW_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( Constants::LISTVIEW_PLAYLIST_COLUMN_FILENAME_HEADER_TEXT, Qt::ElideRight,
                                        currentWidthColumnFilename - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );

    //draw File Size text
    headerTextOffsetX += currentWidthColumnFilename;
    p.drawText( headerTextOffsetX, 0,
                width() - headerTextOffsetX - playlistMargin, Constants::PLAYLISTUI_ARROW_HEIGHT,
                Qt::AlignRight | Qt::AlignVCenter,
                fontMetrics.elidedText( Constants::LISTVIEW_PLAYLIST_COLUMN_FILESIZE_HEADER_TEXT, Qt::ElideRight,
                                        width() - headerTextOffsetX - playlistMargin ) );

    //Overlay drawing
    if( !_hasMainUIRegionFocus ) {
        p.setBrush( QBrush( _noMainRegionFocusPixmap ) );
        p.drawRect( -1, -1, width() + 1, height() + 1 );
    }
}

void ListViewPlaylistUI::handleCentralFocusChanged( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    if( _widgetType == widgetType ) {
        _hasCentralFocus = true;
        displayItems();
        emitEndOfPlaylist();
    } else {
        _hasCentralFocus = false;
    }
}

void ListViewPlaylistUI::changeMainUIRegionFocusGraphics( MainUIRegionFocusChangedEvent::UIRegion mainUIRegion ) {
    if( _mainUIRegionGroup == mainUIRegion )
        _hasMainUIRegionFocus = true;
    else
        _hasMainUIRegionFocus = false;

    for( int i = 0; i < _playlistItemsWidgets.count(); i++ ) {
        _playlistItemsWidgets.at( i )->changeMainUIRegionFocusGraphics( _hasMainUIRegionFocus );
    }
}

void ListViewPlaylistUI::updateItems() {
    _currentPageIndex =  getPageIndexOfItem( _currentItemIndex );
    displayItems();
}

void ListViewPlaylistUI::handleCachedPlaylistRestored( QPointer<IPlaylist> cachedPlaylist ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "begin handleCachedPlaylistRestored" );
    storeSelectedItemId();
    setupPlaylist( cachedPlaylist );
    setCurrentIndexToSelectedInPreviousPlaylistOrFirst();
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "end handleCachedPlaylistRestored" );
}

void ListViewPlaylistUI::handleSearchPlaylistGenerationStarted( QPointer<IPlaylist> newPlaylist ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "begin" );
    storeSelectedItemId();
    setupPlaylist( newPlaylist );
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "end" );
}

void ListViewPlaylistUI::handleRemovedItemFromPlaylist( QUuid removedItemId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "remove Item: " + removedItemId.toString() );
    ListViewTransformWidget *widgetToRemove = 0;
    int removedItemIndex = -1;

    for( int i = 0; i < _playlistItemsWidgets.count() && widgetToRemove == 0; i++ ) {
        if ( _playlistItemsWidgets.at( i )->mediaGuid() == removedItemId ) {
            widgetToRemove = _playlistItemsWidgets.at( i );
            removedItemIndex = i;
        }
    }

    if ( ! widgetToRemove ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_LISTVIEWPLAYLIST, "No Widget foound to Remove for id: " + removedItemId.toString() );
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

        SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "end Removed Item" );
    }
}

void ListViewPlaylistUI::addMediaItem( QUuid mediaId, MediaDiscoveredEvent::MediaDiscoveredSource source ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "begin" );
    IMedia *newMediaItem = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

    if( newMediaItem == NULL || _playlistItems == NULL )
        return;

    int insertedItemIndex = _playlistItems->getItemIndexByGuid( newMediaItem->guid() );
    bool isPodCast = _mediaDB->mediaRepository()->isOnlineContent( newMediaItem->guid() );
    ListViewTransformWidget *newDisplayWidget = new ListViewTransformWidget( _parentWidget, getMediaTypeName( newMediaItem->mediaType().guid() ),
                                                                             newMediaItem, isPodCast, _hasMainUIRegionFocus, insertedItemIndex,
                                                                             _displayItemTextNormalStyle, _displayItemTextSelectedStyle );
    connect( newDisplayWidget, SIGNAL( doubleClicked( int ) ), this, SLOT( playlistItemDoubleClicked( int ) ) );
    connect( newDisplayWidget, SIGNAL( clicked( int ) ), this, SLOT( playlistItemClicked( int ) ) );

    if( source == MediaDiscoveredEvent::STARTUP_MEDIALIBRARY ||
        source == MediaDiscoveredEvent::SEARCH )
        _playlistItemsWidgets.append( newDisplayWidget );

    if( source == MediaDiscoveredEvent::STARTUP_FOLDER ||
        source == MediaDiscoveredEvent::POD_CATCHER ||
        source == MediaDiscoveredEvent::USER_ADD ) {
        _playlistItemsWidgets.insert( 0, newDisplayWidget );
        _currentItemIndex++;
    }

    updateDisplayWidgetsId();
    setCurrentIndexAfterAddItem( source );

    if( !_animationOn && _hasCentralFocus )
        displayItems();

    if( _hasCentralFocus )
        emitEndOfPlaylist();

    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "end" );
}

void ListViewPlaylistUI::setCurrentIndexAfterAddItem( MediaDiscoveredEvent::MediaDiscoveredSource source ) {
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

void ListViewPlaylistUI::setCurrentIndexToUsedAddedItem() {
    selectItem( 0 );

    if( _hasCentralFocus )
        emitFocusedItemChangedEvent( 0, false ); //for title display ONLY
}

void ListViewPlaylistUI::setCurrentIndexToUserSelectOrFirst() {
    if( _userSelectedItem ) {
        _currentItemIndex = _playlistItems->getItemIndexByGuid( _curentItemMediaId );
        _currentPageIndex = getPageIndexOfItem( _currentItemIndex );
    } else {
        selectItem( 0 );

        if( _hasCentralFocus )
            emitFocusedItemChangedEvent( 0, false );
    }
}

void ListViewPlaylistUI::setCurrentIndexToSelectedInPreviousPlaylistOrFirst() {
    int indexOfItem = _playlistItems->getItemIndexByGuid( _selectedItemInPreviousPlaylistId );

    if( indexOfItem < 0 )
        indexOfItem = 0; //selects first

    selectItem( indexOfItem );

    if( _hasCentralFocus )
        emitFocusedItemChangedEvent( indexOfItem, false );
}


void ListViewPlaylistUI::storeSelectedItemId() {
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, QString( "Store Selected uid :" ) + _curentItemMediaId.toString() );
    _selectedItemInPreviousPlaylistId = _curentItemMediaId;
}

void ListViewPlaylistUI::selectItem( int itemIndex ) {
    if( _playlistItems->count() == 0 )
        return;

    if( _currentItemIndex < _playlistItemsWidgets.count() )
        _playlistItemsWidgets.at( _currentItemIndex )->unSelect();

    _playlistItemsWidgets.at( itemIndex )->select();
    _currentItemIndex = itemIndex;
    _currentPageIndex =  getPageIndexOfItem( itemIndex );
    _curentItemMediaId = _playlistItems->at( itemIndex )->getMedia()->guid();
    displayItems();
}

void ListViewPlaylistUI::setupPlaylist( QPointer<IPlaylist> playlist ) {
    setPlaylist( playlist );

    if( _hasCentralFocus )
        emitEndOfPlaylist();
}

void ListViewPlaylistUI::setPlaylist( QPointer<IPlaylist> newPlaylist ) {
    clearItems();
    _playlistItems = newPlaylist;
    initItems();
    displayItems();
}

void ListViewPlaylistUI::clearItems() {
    for( int i = _playlistItemsWidgets.count() - 1; i >= 0; i-- ) {
        _playlistItemsWidgets.at( i )->deleteLater();
    }

    _playlistItemsWidgets.clear();
    _currentItemIndex = 0;
    _currentPageIndex = 0;
    _userSelectedItem = false;
}

void ListViewPlaylistUI::initItems() {
    if( _playlistItems == NULL )
        return;

    for( int i = 0; i < _playlistItems->count(); i++ ) {
        QUuid currentMediaTypeUid = _playlistItems->at( i )->getMedia()->mediaType().guid();
        QString mediaTypeName = getMediaTypeName( currentMediaTypeUid );
        IMedia *currentMediaItem =  _playlistItems->at( i )->getMedia();
        bool isPodCast = _mediaDB->mediaRepository()->isOnlineContent( currentMediaItem->guid() );
        _playlistItemsWidgets.append( new ListViewTransformWidget( _parentWidget, mediaTypeName, currentMediaItem,
                                                                   isPodCast, _hasMainUIRegionFocus, i,
                                                                   _displayItemTextNormalStyle, _displayItemTextSelectedStyle ) );
        connect( _playlistItemsWidgets.at( i ), SIGNAL( doubleClicked( int ) ), this, SLOT( playlistItemDoubleClicked( int ) ) );
        connect( _playlistItemsWidgets.at( i ), SIGNAL( clicked( int ) ), this, SLOT( playlistItemClicked( int ) ) );
    }
}

void ListViewPlaylistUI::displayItems() {
    if( _animationOn )
        return;

    refreshNumberOfRows();

    for( int i = 0; i < _playlistItemsWidgets.count(); i++ ) {
        _playlistItemsWidgets.at( i )->hide();
        displayItem( _playlistItemsWidgets.at( i ), i );
    }

    if( _hasCentralFocus )
        emitEndOfPlaylist();
}

void ListViewPlaylistUI::displayItem( ListViewTransformWidget *item, int indexInList ) {
    int marginPageTop = Constants::PLAYLISTUI_ARROW_HEIGHT;
    int navigationMargin = Constants::PLAYLISTUI_ARROW_MARGIN + Constants::PLAYLISTUI_ARROW_WIDTH + Constants::LISTVIEW_PLAYLIST_PADDING_TO_NAVIGATION_ARROW;
    int rowIndex = indexInList % _currentNumberRows;
    int offsetXAxis = ( getPageIndexOfItem( indexInList ) - _currentPageIndex ) * ( Constants::LISTVIEW_PLAYLIST_SPACE_BETWEEN_PAGES + _parentWidget->width() );
    int itemWidth = _parentWidget->width() - 2 * navigationMargin + _isNewColumnOffsetX;

    //set offsetx
    item->setGeometry( navigationMargin - _isNewColumnOffsetX + offsetXAxis,
                       marginPageTop + rowIndex * _itemHeight,
                       itemWidth, _itemHeight );
    int pageIndexOfItem = getPageIndexOfItem( indexInList );

    if( pageIndexOfItem >= _currentPageIndex - 1 && pageIndexOfItem <= _currentPageIndex + 1 )
        item->show();
}

void ListViewPlaylistUI:: initIsNewIconOffsetX() {
    QPixmap newIcon = QPixmap( Constants::LISTVIEW_PLAYLIST_IMAGE_DIR + Constants::LISTVIEW_PLAYLIST_NEW_ICON );

    if( !newIcon.isNull() ) {
        newIcon = newIcon.scaledToHeight( Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT );
        _isNewColumnOffsetX = newIcon.width();
    } else {
        _isNewColumnOffsetX = 0;
    }
}

void ListViewPlaylistUI::selectNextItemAnimated() {
    if( _currentItemIndex == ( _playlistItemsWidgets.count() - 1 ) )
        return;

    int newPageIndedx = getPageIndexOfItem( _currentItemIndex + 1 );

    if( newPageIndedx > _currentPageIndex )
        scrollLeft();

    emitFocusedItemChangedEvent( _currentItemIndex + 1, true );
}

void ListViewPlaylistUI::selectPreviousItemAnimated() {
    if( _currentItemIndex == 0 )
        return;

    int newPageIndedx = getPageIndexOfItem( _currentItemIndex - 1 );

    if( newPageIndedx < _currentPageIndex )
        scrollRight();

    emitFocusedItemChangedEvent( _currentItemIndex - 1, true );
}

void ListViewPlaylistUI::scrollLeft() {
    if( _hasCentralFocus && !_animationOn ) {
        animateScroll( true );
        emitEndOfPlaylist();
    }
}

void ListViewPlaylistUI::scrollRight() {
    if( _hasCentralFocus && !_animationOn ) {
        animateScroll( false );
        emitEndOfPlaylist();
    }
}

void ListViewPlaylistUI::animateScroll( bool scrollLeft ) {
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    int animatedItemsStartIndex = 0;
    int animatedItemsEndIndex = 0;
    refreshNumberOfRows();
    _animationOn = true;
    group->addAnimation( getScrollAnimationForPage( _currentPageIndex, scrollLeft, Constants::LISTVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH ) );

    if( scrollLeft ) {
        group->addAnimation( getScrollAnimationForPage( _currentPageIndex + 1, scrollLeft, Constants::LISTVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH ) );
        animatedItemsStartIndex = getStartItemIndexOfPage( _currentPageIndex );
        animatedItemsEndIndex = getEndItemIndexOfPage( _currentPageIndex + 1 );
        _currentPageIndex++;
    } else {
        group->addAnimation( getScrollAnimationForPage( _currentPageIndex - 1, scrollLeft, Constants::LISTVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH ) );
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

QtParallelAnimationGroup *ListViewPlaylistUI::getScrollAnimationForPage( int pageIndex, bool scrollLeft, int animationLength ) {
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    int offsetX = ( Constants::LISTVIEW_PLAYLIST_SPACE_BETWEEN_PAGES + _parentWidget->width() );

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

void ListViewPlaylistUI::animationFinishedSlot() {
    _animationOn = false;
}

void ListViewPlaylistUI::emitFocusedItemChangedEvent( int newItemIndex, bool selectedByUser ) {
    QString title = "";
    IPlaylistItem *currentItem = _playlistItems->at( newItemIndex );
    QUuid mediaId = QUuid();

    if( currentItem != NULL && currentItem->getMedia() != NULL ) {
        title = currentItem->getMedia()->playListTitle();
        mediaId = currentItem->getMedia()->guid();
    }

    FocusedPlayListItemChangedEvent e( title, FocusedPlayListItemChangedEvent::LISTVIEW, newItemIndex, selectedByUser, mediaId );
    Aggregator::instance()->sendEvent( &e );
}

void ListViewPlaylistUI::handleFocusedPlaylistItemChanged( int newItemIndex, bool selectedByUser ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "begin" );

    if( _playlistItems == NULL )
        return;

    if( _playlistItems->count() <= 0 )
        return;

    if( _playlistItemsWidgets.count() <= 0 )
        return;

    if( !selectedByUser )
        return;

    if( _currentItemIndex < _playlistItemsWidgets.count() )
        _playlistItemsWidgets.at( _currentItemIndex )->unSelect();

    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "unselected Item" );
    _playlistItemsWidgets.at( newItemIndex )->select();
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "selected new Item" );
    _currentItemIndex = newItemIndex;
    _currentPageIndex =  getPageIndexOfItem( _currentItemIndex );
    _curentItemMediaId = _playlistItems->at( newItemIndex )->getMedia()->guid();

    if( _hasCentralFocus )
        displayItems();

    _userSelectedItem = selectedByUser;
    SimpleLogger::instance()->debug( Constants::LOGGER_LISTVIEWPLAYLIST, "endFocused" );
}

void ListViewPlaylistUI::handleDocumentOpened( QUuid mediaId ) {
    this->handleMediaItemContentChangedEvent( mediaId );
}

void ListViewPlaylistUI::handleMediaItemContentChangedEvent( QUuid mediaId ) {
    if( _playlistItems == NULL )
        return;

    int indexOfPlaylistItem = _playlistItems->getItemIndexByGuid( mediaId );

    if( indexOfPlaylistItem >= 0 ) {
        IMedia *media = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

        if( media ) {
            _playlistItems->at( indexOfPlaylistItem )->setMedia( media );
            /*_playlistItemsWidgets.at(indexOfPlaylistItem)->setIsNewFlag(media->isNew(),false);
            _playlistItemsWidgets.at(indexOfPlaylistItem)->setLength(media->length(),true);*/
            _playlistItemsWidgets.at( indexOfPlaylistItem )->refreshMediaItemMembers( media, true );
        }
    }
}

void ListViewPlaylistUI::emitEndOfPlaylist() {
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

bool ListViewPlaylistUI::isBeginning() {
    if( _playlistItems == NULL )
        return true;

    return _currentPageIndex == 0 && _playlistItems->count() >= 0;
}

bool ListViewPlaylistUI::isEnd() {
    if( _playlistItems == NULL )
        return true;

    return _currentPageIndex == getPageIndexOfItem( _playlistItems->count() - 1 ) && _playlistItems->count() >= 0;
}

int ListViewPlaylistUI::getStartItemIndexOfPage( int pageIndex ) {
    return _currentNumberRows * pageIndex;
}

int ListViewPlaylistUI::getEndItemIndexOfPage( int pageIndex ) {
    int returnIndex = _currentNumberRows * ( ++pageIndex ) - 1;

    if( returnIndex > _playlistItemsWidgets.count() - 1 )
        returnIndex = _playlistItemsWidgets.count() - 1;

    return returnIndex;
}

bool ListViewPlaylistUI::hasCentralFocus() {
    return _hasCentralFocus;
}

bool ListViewPlaylistUI::hasMainFocus() {
    return _hasMainUIRegionFocus;
}

void ListViewPlaylistUI::playCurrentItem() {
    playlistItemDoubleClicked( _currentItemIndex );
}

void ListViewPlaylistUI::playlistItemClicked( int id ) {
    emitFocusedItemChangedEvent( id, true );
}


void ListViewPlaylistUI::playlistItemDoubleClicked( int id ) {
    if( _playlistItems == NULL )
        return;

    PlaylistItemSelectedEvent e( _playlistItems->at( id ) );
    Aggregator::instance()->sendEvent( &e );
}

int ListViewPlaylistUI::getPageIndexOfItem( int indexInList ) {
    if( _currentNumberRows == 0 )
        return 0;

    return ( int )indexInList / _currentNumberRows;
}

void ListViewPlaylistUI::refreshNumberOfRows() {
    int headerHeight = Constants::PLAYLISTUI_ARROW_HEIGHT;
    int currentHeight = _parentWidget->height();
    _currentNumberRows = ( int )( currentHeight - headerHeight ) / _itemHeight;
}

void ListViewPlaylistUI::updateDisplayWidgetsId() {
    for( int i = 0; i < _playlistItemsWidgets.count(); i++ )
        _playlistItemsWidgets.at( i )->setId( i );
}

QString ListViewPlaylistUI::getMediaTypeName( QUuid mediaTypeId ) {
    QString mediaTypeName = "";

    if( mediaTypeId == _mediaDB->videoType()->guid() ) {
        mediaTypeName = Constants::MEDIA_TYPE_VIDEO_NAME;
    } else {
        if( mediaTypeId == _mediaDB->audioType()->guid() ) {
            mediaTypeName = Constants::MEDIA_TYPE_AUDIO_NAME;
        } else {
            if( mediaTypeId == _mediaDB->imageType()->guid() ) {
                mediaTypeName = Constants::MEDIA_TYPE_IMAGE_NAME;
            }
        }
    }

    return mediaTypeName;
}



