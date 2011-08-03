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
#include "CoverViewPlaylistUI.h"
#include "ui_CoverViewPlaylistUI.h"

//Qt Includes
#include <QDebug>
#include <QPainter>
#include <QDateTime>
#include <QMessageBox>

// Animation
#include "qtpropertyanimation.h"
#include "qtsequentialanimationgroup.h"

// Project Includes
#include "../../Container/Constants.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/Playlist/PlaylistItemSelectedEvent.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::MediaLibrary;

CoverViewPlaylistUI::CoverViewPlaylistUI( RedBullPlayer::PlayerShell::Shell *shell, PlaylistContentTypeChangedEvent::ContentType contentType, QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::CoverViewPlaylistUI ) {
    _shell = shell;
    _parentWidget = parent;
    _rootWidget = ( _shell->findRegion( "Root" ) )->widget();
    EASING_CURVE = QtEasingCurve::InOutCubic;
    _animationOn = false;
    _ui->setupUi( this );
    _currentIndex = 0;
    _currentMediaItemId = QUuid();
    _selectedItemInPreviousPlaylistId = QUuid();
    _hasCentralFocus = false;
    _hasMainUIRegionFocus = false;
    _mainUIRegionGroup = MainUIRegionFocusChangedEvent::CENTRAL;
    _userSelectedItem = false;

    switch( contentType ) {
        case PlaylistContentTypeChangedEvent::MEDIA:
            _widgetType = CentralFocusChangedEvent::COVERVIEW_PLAYLIST;
            _hasCentralFocus = true; //default player setting hardCoded
            break;

        case PlaylistContentTypeChangedEvent::TV:
            _widgetType = CentralFocusChangedEvent::TV_PLAYLIST;
            break;

        case PlaylistContentTypeChangedEvent::NEWS:
            _widgetType = CentralFocusChangedEvent::NEWS_PLAYLIST;
            break;

        case PlaylistContentTypeChangedEvent::ABOUT:
            _widgetType = CentralFocusChangedEvent::ABOUT_PLAYLIST;
            break;

        case PlaylistContentTypeChangedEvent::SETTINGS:
            _widgetType = CentralFocusChangedEvent::SETTINGS_PLAYLIST;
            break;

        case PlaylistContentTypeChangedEvent::NONE:
            break;
    }

    _coverViewContentType = contentType;
    _numberItemsFormat16To9 = ConfigManager::instance()->numberItemsCoverViewFormat16To9();
    _numberItemsFormat4To3 = ConfigManager::instance()->numberItemsCoverViewFormat4To3();
    _isNewsAboutDetailOpen = false;
}

CoverViewPlaylistUI::~CoverViewPlaylistUI() {
    if( _playlistItems )
        delete _playlistItems;

    delete _ui;
}

void CoverViewPlaylistUI::setupMediaLibraryConnection() {
    if( _widgetType == CentralFocusChangedEvent::COVERVIEW_PLAYLIST ) {
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
}


void CoverViewPlaylistUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void CoverViewPlaylistUI::resizeEvent( QResizeEvent */*e*/ ) {
    updateItems();
}

void CoverViewPlaylistUI::setupPlaylist( QPointer<IPlaylist> playlist ) {
    setPlaylist( playlist );
    emit itemsRearranged();

    if( _hasCentralFocus )
        emitEndOfPlaylist();

    //TV, News and About selected 1st on Startup
    if( _widgetType == CentralFocusChangedEvent::ABOUT_PLAYLIST
            || _widgetType == CentralFocusChangedEvent::NEWS_PLAYLIST
            || _widgetType == CentralFocusChangedEvent::TV_PLAYLIST
            || _widgetType == CentralFocusChangedEvent::SETTINGS_PLAYLIST )
        selectItem( 0, false );
}

void CoverViewPlaylistUI::scrollLeft() {
    if( _playlistItems == NULL )
        return;

    if( !_isNewsAboutDetailOpen )
        if( isEnd() || _animationOn || !_hasCentralFocus ) {
            return;
        }

    _animationOn = true;
    scroll( true );
    selectItem( _currentIndex + 1, true );
    _currentIndex++;
    _currentMediaItemId = _playlistItems->at( _currentIndex )->getMedia()->guid();
    emit itemsRearranged();
}

void CoverViewPlaylistUI::scrollRight() {
    if( _playlistItems == NULL )
        return;

    if( !_isNewsAboutDetailOpen ) {
        if( isBeginning() || _animationOn || !_hasCentralFocus ) {
            return;
        }
    }

    _animationOn = true;
    scroll( false );
    selectItem( _currentIndex - 1, true );
    _currentIndex--;
    _currentMediaItemId = _playlistItems->at( _currentIndex )->getMedia()->guid();
    emit itemsRearranged();
}

void CoverViewPlaylistUI::playCurrentItem( bool skipDocuments, bool fromSelectPreviousItemEvent ) {
    if( _playlistItems == NULL )
        return;

    PlaylistItemSelectedEvent e( _playlistItems->at( getCurrentIndex() ), skipDocuments , fromSelectPreviousItemEvent );
    Aggregator::instance()->sendEvent( &e );
}


void CoverViewPlaylistUI::scroll( bool left ) {
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    int firstIndex = getCurrentIndex() - _numberItemsCurrent / 2;
    int floor = firstIndex;

    if( !left ) floor--;

    int top = getCurrentIndex() + _numberItemsCurrent / 2;

    if( left ) top++;

    for( int i = floor; i <= top; i++ ) {
        if( i >= 0 && i < _playlistItemsWidgets.count() ) {
            group->addAnimation( getAnimationForItem( _playlistItemsWidgets.at( i ), i - firstIndex, left, Constants::PLAYLISTUI_ANIMATION_LENGTH ) );
        }
    }

    connect( group, SIGNAL( finished() ), this, SLOT( animationFinishedSlot() ) );
    group->start();
}


void CoverViewPlaylistUI::scrollToIndex( int indexOnScreenToBeCurrent ) {
    if( _playlistItems->count() == 0 )
        return;

    int currentIndex = getCurrentIndex();
    int indexDiffrence = indexOnScreenToBeCurrent - currentIndex;

    if( currentIndex == indexOnScreenToBeCurrent )
        return;

    if( indexDiffrence == 1 ) {
        scroll( true );
    }

    if( indexDiffrence == -1 ) {
        scroll( false );
    }

    if( indexDiffrence > 1 || indexDiffrence < -1 )
        animateScrollToIndex( indexOnScreenToBeCurrent );

    _currentIndex += indexDiffrence;
    _currentMediaItemId = _playlistItems->at( _currentIndex )->getMedia()->guid();

    if( _hasCentralFocus || _isNewsAboutDetailOpen )
        emit itemsRearranged();
}

void CoverViewPlaylistUI::animateScrollToIndex( int indexOnScreenToBeCurrent ) {
    int currentIndex = getCurrentIndex();
    int indexDiffrence = currentIndex - indexOnScreenToBeCurrent;
    int firstIndex = getCurrentIndex() - _numberItemsCurrent / 2;
    QtParallelAnimationGroup *pgroup = new QtParallelAnimationGroup( _parentWidget );
    int floor = 0;
    int top = 0;

    if( indexDiffrence < 0 ) {
        floor = currentIndex - _numberItemsCurrent / 2;
        top = indexOnScreenToBeCurrent + abs( indexDiffrence );
    } else {
        floor = indexOnScreenToBeCurrent - indexDiffrence;
        top = currentIndex + _numberItemsCurrent / 2;
    }

    for( int i = floor; i <= top; i++ ) {
        if( i >= 0 && i < _playlistItemsWidgets.count() ) {
            pgroup->addAnimation( getScrollToIndexAnimationForItem( _playlistItemsWidgets.at( i ), i - firstIndex, indexDiffrence,
                                  Constants::PLAYLISTUI_ANIMATION_LENGTH ) );
        }
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, QString( "current Index = %1" ).arg( currentIndex ) );
    connect( pgroup, SIGNAL( finished() ), this, SLOT( animationFinishedSlot() ) );
    pgroup->start();
}


IPlaylistItem *CoverViewPlaylistUI::getCurrentItem() {
    int index = getCurrentIndex();

    if( _playlistItems != NULL && index >= 0 && index < _playlistItems->count() ) {
        return _playlistItems->at( index );
    }

    return NULL;
}


int CoverViewPlaylistUI::getCurrentIndex() {
    return _currentIndex;
}


void CoverViewPlaylistUI::setCurrentIndex( int index ) {
    if( _playlistItems == NULL )
        return;

    //loop Playlist in both directions
    if( index < 0 ) {
        index =  _playlistItems->count() - 1;
    } else {
        if( index >= _playlistItems->count() )
            index = 0;
    }

    selectItem( index, true );
    _currentIndex = index;
    _currentMediaItemId = _playlistItems->at( _currentIndex )->getMedia()->guid();
    displayItems();
}


void CoverViewPlaylistUI::setPlaylist( QPointer<IPlaylist> newPlaylist ) {
    clearItems();
    _playlistItems = newPlaylist;
    initItems();
    displayItems();
}


void CoverViewPlaylistUI::updateItems() {
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, Q_FUNC_INFO );
    displayItems();
}

void CoverViewPlaylistUI::playlistItemClicked( int id ) {
    selectItem( id, true );
    scrollToIndex( id );

    if( _isNewsAboutDetailOpen )
        playlistItemDoubleClicked( id );
}

void CoverViewPlaylistUI::selectItem( int itemIndex, bool selectedByUser ) {
    if( _playlistItems == NULL )
        return;

    if( _playlistItems->count() > 0 ) {
        if( getCurrentIndex() < _playlistItemsWidgets.count() )
            _playlistItemsWidgets.at( getCurrentIndex() )->unSelect();

        _playlistItemsWidgets.at( itemIndex )->select();
        _userSelectedItem = selectedByUser;
    }
}

void CoverViewPlaylistUI::playlistItemDoubleClicked( int id ) {
    if( _playlistItems == NULL )
        return;

    PlaylistItemSelectedEvent e( _playlistItems->at( id ) );
    Aggregator::instance()->sendEvent( &e );
}

void CoverViewPlaylistUI::handleCachedPlaylistRestored( QPointer<IPlaylist> cachedPlaylist ) {
    storeSelectedItemId();
    setupPlaylist( cachedPlaylist );
    setCurrentIndexToSelectedInPreviousPlaylistOrFirst();
}

void CoverViewPlaylistUI::handleSearchPlaylistGenerationStarted( QPointer<IPlaylist> newPlaylist ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "begin" );
    storeSelectedItemId();
    setupPlaylist( newPlaylist );
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "end" );
}

void CoverViewPlaylistUI::handleRemovedItemFromPlaylist( QUuid removedItemId ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "remove Item: " + removedItemId.toString() );
    TransformWidget *widgetToRemove = 0;
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

    // int removedItemIndex = _playlistItems->getItemIndexByGuid(removedItemId);

    if( removedItemIndex < 0 )
        return;

    if( removedItemIndex < _playlistItemsWidgets.count() && removedItemIndex >= 0 ) {
        if( getCurrentIndex() == removedItemIndex ) {
            _playlistItemsWidgets.at( removedItemIndex )->unSelect();

            if( removedItemIndex == 0 && _playlistItemsWidgets.count() == 1 ) {
                _currentIndex = 0;
                _currentMediaItemId = QUuid();
            } else {
                if( removedItemIndex == _playlistItemsWidgets.count() - 1 ) {
                    _playlistItemsWidgets.at( removedItemIndex - 1 )->select();
                    _currentIndex = removedItemIndex - 1;
                } else {
                    _playlistItemsWidgets.at( removedItemIndex + 1 )->select();
                    _currentIndex = removedItemIndex;
                }

                _currentMediaItemId = _playlistItems->at( _currentIndex )->getMedia()->guid();
            }
        }

        _userSelectedItem = false;

        if( _hasCentralFocus )
            emit itemsRearranged();

        _playlistItemsWidgets.removeAt( removedItemIndex );
        widgetToRemove->deleteLater();
        updateDisplayWidgetsId();

        if( _hasCentralFocus )
            updateItems();

        SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "end" );
    }
}

void CoverViewPlaylistUI::addMediaItem( QUuid mediaId, MediaDiscoveredEvent::MediaDiscoveredSource source ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "begin" );
    IMedia *newMediaItem = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

    if( newMediaItem == NULL || _playlistItems == NULL )
        return;

    int insertedItemIndex = _playlistItems->getItemIndexByGuid( newMediaItem->guid() );
    bool isPodcast = _mediaDB->mediaRepository()->isOnlineContent( newMediaItem->guid() );
    TransformWidget *newDisplayWidget = new TransformWidget( _parentWidget,
            getImageForContentType( _playlistItems->at( insertedItemIndex ) ),
            getMediaTypeName( newMediaItem->mediaType().guid() ), newMediaItem,
            isPodcast, _hasMainUIRegionFocus, false,
            insertedItemIndex );
    connect( newDisplayWidget, SIGNAL( doubleClicked( int ) ), this, SLOT( playlistItemDoubleClicked( int ) ) );
    connect( newDisplayWidget, SIGNAL( clicked( int ) ), this, SLOT( playlistItemClicked( int ) ) );
    TransformParams p = getTransformParamsForItem( _numberItemsCurrent / 2 );
    newDisplayWidget->scaleToSize( QSize( p.transformedWidth, p.frontHeight ) * 1.2 );

    if( source == MediaDiscoveredEvent::STARTUP_MEDIALIBRARY ||
            source == MediaDiscoveredEvent::SEARCH )
        _playlistItemsWidgets.append( newDisplayWidget );

    if( source == MediaDiscoveredEvent::STARTUP_FOLDER ||
            source == MediaDiscoveredEvent::POD_CATCHER ||
            source == MediaDiscoveredEvent::USER_ADD ) {
        _playlistItemsWidgets.insert( 0, newDisplayWidget );
        _currentIndex++;
    }

    updateDisplayWidgetsId();
    setCurrentIndexAfterAddItem( source );

    if( !_animationOn && _hasCentralFocus )
        displayItems();

    if( _hasCentralFocus )
        emitEndOfPlaylist();

    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "end" );
}

void CoverViewPlaylistUI::setCurrentIndexAfterAddItem( MediaDiscoveredEvent::MediaDiscoveredSource source ) {
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

void CoverViewPlaylistUI::setCurrentIndexToUsedAddedItem() {
    selectItem( 0, false );
    scrollToIndex( 0 );
    _currentMediaItemId = _playlistItems->at( 0 )->getMedia()->guid();

    if( _hasCentralFocus )
        emit itemsRearranged();
}

void CoverViewPlaylistUI::setCurrentIndexToUserSelectOrFirst() {
    if( _userSelectedItem ) {
        _currentIndex = _playlistItems->getItemIndexByGuid( _currentMediaItemId );
    } else {
        selectItem( 0, false );
        scrollToIndex( 0 );
        _currentMediaItemId = _playlistItems->at( 0 )->getMedia()->guid();

        if( _hasCentralFocus )
            emit itemsRearranged();
    }
}

void CoverViewPlaylistUI::setCurrentIndexToSelectedInPreviousPlaylistOrFirst() {
    if( _playlistItems->count() == 0 )
        return;

    int indexOfItem = _playlistItems->getItemIndexByGuid( _selectedItemInPreviousPlaylistId );

    if( indexOfItem >= 0 ) {
        selectItem( indexOfItem, false );
        scrollToIndex( indexOfItem );
        _currentMediaItemId = _playlistItems->at( indexOfItem )->getMedia()->guid();
    } else {
        selectItem( 0, false );
        scrollToIndex( 0 );
        _currentMediaItemId = _playlistItems->at( 0 )->getMedia()->guid();
    }

    if( _hasCentralFocus )
        emit itemsRearranged();
}

void CoverViewPlaylistUI::storeSelectedItemId() {
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, QString( "Store Selected uid :" ) + _currentMediaItemId.toString() );
    _selectedItemInPreviousPlaylistId = _currentMediaItemId;
}

void CoverViewPlaylistUI::updateDisplayWidgetsId() {
    for( int i = 0; i < _playlistItemsWidgets.count(); i++ )
        _playlistItemsWidgets.at( i )->setId( i );
}

void CoverViewPlaylistUI::initItems() {
    refreshDisplayFormat( _parentWidget->width(), _parentWidget->height() );
    Q_ASSERT( _playlistItemsWidgets.count() == 0 );

    for( int i = 0; i < _playlistItems->count(); i++ ) {
        IPlaylistItem *item = _playlistItems->at( i );
        QString mediaTypeName = "";
        QUuid currentMediaTypeUid = _playlistItems->at( i )->getMedia()->mediaType().guid();
        bool isPodcast = false;

        if( _widgetType == CentralFocusChangedEvent::COVERVIEW_PLAYLIST ) {
            mediaTypeName = getMediaTypeName( currentMediaTypeUid );
            isPodcast = _mediaDB->mediaRepository()->isOnlineContent( _playlistItems->at( i )->getMedia()->guid() );
        } else {
            mediaTypeName = item->getMedia()->mediaType().name();
        }

        _playlistItemsWidgets.append( new TransformWidget( _parentWidget, getImageForContentType( item ), mediaTypeName, item->getMedia(),
                                      isPodcast, _hasMainUIRegionFocus, true, i ) );
        TransformParams p = getTransformParamsForItem( _numberItemsCurrent / 2 );
        _playlistItemsWidgets.at( i )->scaleToSize( QSize( p.transformedWidth, p.frontHeight ) * 1.2 );
        connect( _playlistItemsWidgets.at( i ), SIGNAL( clicked( int ) ), this, SLOT( playlistItemClicked( int ) ) );
        connect( _playlistItemsWidgets.at( i ), SIGNAL( doubleClicked( int ) ), this, SLOT( playlistItemDoubleClicked( int ) ) );
    }

    if( _currentIndex >= _playlistItems->count() ) {
        _currentIndex = 0;

        if( _playlistItems->count() > 0 )
            _currentMediaItemId = _playlistItems->at( _currentIndex )->getMedia()->guid();
        else
            _currentMediaItemId = QUuid();
    }
}

void  CoverViewPlaylistUI::handleCoverImageChanged( QUuid mediaId ) {
    if( _playlistItems == NULL )
        return;

    int indexOfPlaylistItem = _playlistItems->getItemIndexByGuid( mediaId );

    if( indexOfPlaylistItem >= 0 ) {
        IMedia *media = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

        if( media ) {
            _playlistItems->at( indexOfPlaylistItem )->setMedia( media );
            _playlistItemsWidgets.at( indexOfPlaylistItem )->changeCoverImage( media->coverImage() );
        }
    }
}

void CoverViewPlaylistUI::handleDocumentOpened( QUuid mediaId ) {
    this->handleMediaItemContentChangedEvent( mediaId );
}

void CoverViewPlaylistUI::handleMediaItemContentChangedEvent( QUuid mediaId ) {
    int indexOfPlaylistItem = _playlistItems->getItemIndexByGuid( mediaId );

    if( indexOfPlaylistItem >= 0 ) {
        IMedia *media = _mediaDB->mediaRepository()->getMediaByGuid( mediaId );

        if( media ) {
            _playlistItems->at( indexOfPlaylistItem )->setMedia( media );
            _playlistItemsWidgets.at( indexOfPlaylistItem )->refreshMediaItemMembers( media, true );
        }
    }
}

void CoverViewPlaylistUI::displayItems() {
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "begin" );

    for( int i = 0; i < _playlistItemsWidgets.count(); i++ ) {
        _playlistItemsWidgets.at( i )->hide();
        _playlistItemsWidgets.at( i )->setTransformMode( Qt::SmoothTransformation );
    }

    refreshDisplayFormat( _parentWidget->width(), _parentWidget->height() );

    if( isBeginning() ) {
        for( int i = 0; i < _numberItemsCurrent / 2 + 1; i++ ) {
            int index = getCurrentIndex() + i;

            if( index >= 0 && index < _playlistItemsWidgets.count() ) {
                displayItem( _playlistItemsWidgets.at( index ), i + _numberItemsCurrent / 2 );
            }
        }
    } else {
        for( int i = -1 * _numberItemsCurrent / 2; i < _numberItemsCurrent / 2 + 1; i++ ) {
            int index = getCurrentIndex() + i;

            if( index >= 0 && index < _playlistItemsWidgets.count() ) {
                displayItem( _playlistItemsWidgets.at( index ), i + _numberItemsCurrent / 2 );
            }
        }
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "end" );
}


void CoverViewPlaylistUI::clearItems() {
    for( int i = _playlistItemsWidgets.count() - 1; i >= 0; i-- ) {
        _playlistItemsWidgets.at( i )->deleteLater();
    }

    _playlistItemsWidgets.clear();
    _currentIndex = 0;
    _userSelectedItem = false;
}


void CoverViewPlaylistUI::displayItem( TransformWidget *item, int indexOnScreen ) {
    TransformParams p = getTransformParamsForItem( indexOnScreen );
    int height = p.frontHeight > p.backHeight ? p.frontHeight : p.backHeight;
    item->resize( p.transformedWidth, 2 * height );
    item->setTransform( p.transformedWidth, p.frontHeight, p.backHeight, p.topRightDiff );
    item->setFadeStyle( getFadeStyleOfItem( indexOnScreen ) );
    item->move( getXOfItem( indexOnScreen ), p.y );
    int indexSelected = getCurrentIndex();

    if( indexOnScreen >= indexSelected - 6 || indexOnScreen <= indexSelected + 6 )
        item->show();
}


int CoverViewPlaylistUI::getXOfItem( int index ) {
    int widthOfPreviousItems = 0;

    for( int i = 0; i < abs( index ); i++ ) {
        widthOfPreviousItems += getTransformParamsForItem( i ).transformedWidth;
    }

    int sign = index < 0 ? -1 : 1;
    return sign * widthOfPreviousItems + index * Constants::PLAYLISTUI_ITEM_GAP;
}

int CoverViewPlaylistUI::getYOfItem( int index ) {
    int middleY = 10;

    if( index == _numberItemsCurrent / 2 ) {
        return middleY;
    }

    int distFromMiddle = abs( index - _numberItemsCurrent / 2 );

    if( distFromMiddle == 1 ) {
        int middleHeight = getItemHeight( _numberItemsCurrent / 2 );
        int nonMiddleHeight = getItemHeight( _numberItemsCurrent / 2 - 1 );
        int diff = middleHeight - nonMiddleHeight;
        return 3 * diff / 4 + middleY;
    }

    bool left = index < _numberItemsCurrent / 2;

    if( left ) {
        return getYOfItem( index + 1 ) + getTopRightDiffOfItem( index + 1 );
    } else {
        return getYOfItem( index - 1 ) + getTopRightDiffOfItem( index - 1 );
    }
}


MirrorWidget::FadeStyle CoverViewPlaylistUI::getFadeStyleOfItem( int index ) {
    return MirrorWidget::FadeDown;
}


bool CoverViewPlaylistUI::isBeginning() {
    if( _playlistItems == NULL || _playlistItems->count() == 0 ) {
        return true;
    }

    return _currentIndex == 0 && _playlistItems->count() >= 0;
}


bool CoverViewPlaylistUI::isEnd() {
    if( _playlistItems == NULL || _playlistItems->count() == 0 ) {
        return true;
    }

    return _currentIndex == _playlistItems->count() - 1 && _playlistItems->count() >= 0;
}

int CoverViewPlaylistUI::getItemWidth( int index ) {
    int nonMiddleWidth = ( _parentWidget->width() - ( _numberItemsCurrent - 1 ) * Constants::PLAYLISTUI_ITEM_GAP ) / ( _numberItemsCurrent + 1 );

    if( index == _numberItemsCurrent / 2 ) {
        return 2 * nonMiddleWidth;
    }

    return nonMiddleWidth;
}


int CoverViewPlaylistUI::getItemHeight( int index ) {
    return (int) ( getItemWidth( index ) * Constants::PLAYLISTUI_ITEM_RATIO );
}

int CoverViewPlaylistUI::getTopRightDiffOfItem( int index ) {
    int distFromMiddle = abs( index - _numberItemsCurrent / 2 );
    return 5 + ( distFromMiddle - 1 ) * 2;
}

void CoverViewPlaylistUI::animationFinishedSlot() {
    displayItems();
    _animationOn = false;
}

TransformParams CoverViewPlaylistUI::getTransformParamsForItem( int indexOnScreen ) {
    int middleIndex = _numberItemsCurrent / 2;
    int distFromMiddle = abs( indexOnScreen - middleIndex );
    int direction = indexOnScreen - middleIndex;

    if( middleIndex == indexOnScreen ) {
        return TransformParams ( getItemWidth( indexOnScreen ), getItemHeight( indexOnScreen ), getItemHeight( indexOnScreen ), 0, getYOfItem( indexOnScreen ) );
    }

    int previousDiffs = 0;

    for( int i = 1; i < distFromMiddle; i++ ) {
        previousDiffs += i * 5;
    }

    int hmax = getItemHeight( indexOnScreen ) - previousDiffs;
    int hmin = hmax - 5 * distFromMiddle;

    if( direction < 0 ) {
        return TransformParams ( getItemWidth( indexOnScreen ), hmin, hmax, -1 * getTopRightDiffOfItem( indexOnScreen ), getYOfItem( indexOnScreen ) );
    } else {
        return TransformParams ( getItemWidth( indexOnScreen ), hmax, hmin, getTopRightDiffOfItem( indexOnScreen ), getYOfItem( indexOnScreen ) );
    }
}

QtParallelAnimationGroup *CoverViewPlaylistUI::getAnimationForItem( TransformWidget *widget, int indexOnScreen,
        bool scrollingLeft, int animLength ) {
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    int newIndex = indexOnScreen + 1;

    if( scrollingLeft ) {
        newIndex = indexOnScreen - 1;
    }

    QtPropertyAnimation *a1 = new QtPropertyAnimation( widget, "transform", _parentWidget );
    a1->setDuration( animLength );
    a1->setEasingCurve( EASING_CURVE );
    TransformParams params1 = getTransformParamsForItem( indexOnScreen );
    TransformParams params2 = getTransformParamsForItem( newIndex );
    a1->setStartValue( QLineF( params1.transformedWidth, params1.frontHeight, params1.backHeight, params1.topRightDiff ) );
    a1->setEndValue( QLineF( params2.transformedWidth, params2.frontHeight, params2.backHeight, params2.topRightDiff ) );
    group->addAnimation( a1 );
    QtPropertyAnimation *a2 = new QtPropertyAnimation( widget, "pos", _parentWidget );
    a2->setDuration( animLength );
    a2->setEasingCurve( EASING_CURVE );
    a2->setStartValue( QPoint( getXOfItem( indexOnScreen ), getYOfItem( indexOnScreen ) ) );
    a2->setEndValue( QPoint( getXOfItem( newIndex ), getYOfItem( newIndex ) ) );
    group->addAnimation( a2 );
    QtPropertyAnimation *a3 = new QtPropertyAnimation( widget, "size", _parentWidget );
    a3->setDuration( animLength );
    a3->setEasingCurve( EASING_CURVE );
    int height = params2.frontHeight > params2.backHeight ? params2.frontHeight : params2.backHeight;

    if( widget->isHidden() ) {
        a3->setStartValue( QSize( params2.transformedWidth, 2 * height ) );
    }

    a3->setEndValue( QSize( params2.transformedWidth, 2 * height ) );
    group->addAnimation( a3 );
    widget->setFadeStyle( getFadeStyleOfItem( newIndex ) );
    widget->setTransformMode( Qt::SmoothTransformation );
    widget->show();
    return group;
}

QtParallelAnimationGroup *CoverViewPlaylistUI::getScrollToIndexAnimationForItem( TransformWidget *widget, int indexOnScreen,
        int indexDiffrence, int animLength ) {
    int newIndex = indexOnScreen + indexDiffrence;
    QtParallelAnimationGroup *group = new QtParallelAnimationGroup( _parentWidget );
    QtPropertyAnimation *a1 = new QtPropertyAnimation( widget, "transform", _parentWidget );
    a1->setDuration( animLength );
    a1->setEasingCurve( EASING_CURVE );
    TransformParams params1 = getTransformParamsForItem( indexOnScreen );
    TransformParams params2 = getTransformParamsForItem( newIndex );
    a1->setStartValue( QLineF( params1.transformedWidth, params1.frontHeight, params1.backHeight, params1.topRightDiff ) );
    a1->setEndValue( QLineF( params2.transformedWidth, params2.frontHeight, params2.backHeight, params2.topRightDiff ) );
    group->addAnimation( a1 );
    QtPropertyAnimation *a2 = new QtPropertyAnimation( widget, "pos", _parentWidget );
    a2->setDuration( animLength );
    a2->setEasingCurve( EASING_CURVE );
    a2->setStartValue( QPoint( getXOfItem( indexOnScreen ), getYOfItem( indexOnScreen ) ) );
    a2->setEndValue( QPoint( getXOfItem( newIndex ), getYOfItem( newIndex ) ) );
    group->addAnimation( a2 );
    QtPropertyAnimation *a3 = new QtPropertyAnimation( widget, "size", _parentWidget );
    a3->setDuration( animLength );
    a3->setEasingCurve( EASING_CURVE );
    int height = params2.frontHeight > params2.backHeight ? params2.frontHeight : params2.backHeight;

    if( widget->isHidden() ) {
        a3->setStartValue( QSize( params2.transformedWidth, 2 * height ) );
    }

    a3->setEndValue( QSize( params2.transformedWidth, 2 * height ) );
    group->addAnimation( a3 );
    widget->setFadeStyle( getFadeStyleOfItem( newIndex ) );
    widget->setTransformMode( Qt::SmoothTransformation );
    widget->show();
    return group;
}

QImage CoverViewPlaylistUI::getImageForContentType( IPlaylistItem *item ) {
    if( item == 0 || item->getMedia() == 0 ) {
        return QImage();
    }

    // QString url = item->getThumbnailUrl();
    QString mediaType = item->getMedia()->mediaType().name();

    if( mediaType == Constants::MEDIA_TYPE_TV_NAME ) {
        QImage thumbnail = item->getMedia()->coverImage();
        QImage icon( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_TV_ICON );
        int topMargin = (int) ( ( ( float )Constants::COVERVIEW_PLAYLIST_TV_ICON_TOP / ( float )icon.height() ) * thumbnail.height() );
        int bottomMargin = (int) ( ( ( float )Constants::COVERVIEW_PLAYLIST_TV_ICON_BOTTOM / ( float )icon.height() ) * thumbnail.height() );
        int leftMargin = (int) ( ( ( float )Constants::COVERVIEW_PLAYLIST_TV_ICON_LEFT / ( float )icon.width() ) * thumbnail.width() );
        int rightMargin = (int) ( ( ( float )Constants::COVERVIEW_PLAYLIST_TV_ICON_RIGHT / ( float )icon.width() ) * thumbnail.width() );
        int iconWidth = thumbnail.width() + leftMargin + rightMargin;
        int iconHeight = thumbnail.height() + topMargin + bottomMargin;
        icon = icon.scaled( iconWidth, iconHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        QPainter painter( &icon );
        painter.drawImage( QPoint( leftMargin, topMargin ), thumbnail );
        return icon;
    } else if ( mediaType == Constants::MEDIA_TYPE_DOCUMENT_NAME ) {
        if ( !item->getMedia()->coverImage().isNull() )
            return item->getMedia()->coverImage();

        QString fileName = item->getMedia()->fileName();

        if ( ConfigManager::instance()->isPdfDocument( fileName ) ) {
            return QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_PDF_THUMBNAIL );
        }

        if ( ConfigManager::instance()->isPowerpointDocument( fileName ) ) {
            return QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_POWERPOINT_THUMBNAIL );
        }

        if ( ConfigManager::instance()->isWordDocument( fileName ) ) {
            return QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_WORD_THUMBNAIL );
        }

        if ( ConfigManager::instance()->isExcelDocument( fileName ) ) {
            return QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_EXCEL_THUMBNAIL );
        }

        if ( ConfigManager::instance()->isOtherDocument( fileName ) ) {
            return QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_OTHER_THUMBNAIL );
        }
    }

    return item->getMedia()->coverImage();
}

void CoverViewPlaylistUI::changeMainUIRegionFocusGraphics( MainUIRegionFocusChangedEvent::UIRegion mainUIRegion ) {
    if( _mainUIRegionGroup == mainUIRegion )
        _hasMainUIRegionFocus = true;
    else
        _hasMainUIRegionFocus = false;
}

void CoverViewPlaylistUI::handleCentralFocusChanged( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    if( _widgetType == widgetType )
        _hasCentralFocus = true;
    else
        _hasCentralFocus = false;

    if( _hasCentralFocus ) {
        // if just one item play it
        if( _widgetType == CentralFocusChangedEvent::TV_PLAYLIST
                && _playlistItemsWidgets.count() == 1 ) {
            qDebug() << "CoverViewPlaylistUI => only 1 TV item -> autoplay";
            PlaylistItemSelectedEvent e( _playlistItems->at( 0 ), false, false, true );
            Aggregator::instance()->sendEvent( &e );
        } else {
            displayItems();
            emitEndOfPlaylist();
        }
    }

    resolveNewsAboutDetailOpenFlag( widgetType );
}

void CoverViewPlaylistUI::handlePlaylistNavigationKeyPressed( PlaylistNavigationKeyPressedEvent::Direction direction ) {
    switch( direction ) {
        case PlaylistNavigationKeyPressedEvent::LEFT : {
                scrollRight();
                break;
            }

        case PlaylistNavigationKeyPressedEvent::RIGHT : {
                scrollLeft();
                break;
            }
    }

    if( _isNewsAboutDetailOpen )
        playlistItemDoubleClicked( getCurrentIndex() );
}

void CoverViewPlaylistUI::handleFocusedPlaylistItemChanged( int newItemIndex, bool selectedByUser ) {
    if( _playlistItems == NULL )
        return;

    if( _playlistItemsWidgets.count() <= 0 )
        return;

    if( !selectedByUser )
        return;

    if( _playlistItems->count() > 0 ) {
        selectItem( newItemIndex, selectedByUser );
        _currentIndex = newItemIndex;
        _currentMediaItemId = _playlistItems->at( _currentIndex )->getMedia()->guid();

        if( _hasCentralFocus ) {
            displayItems();
            emitEndOfPlaylist();
        }
    }
}

void CoverViewPlaylistUI::emitEndOfPlaylist() {
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, Q_FUNC_INFO );
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "begin" );
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
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, QString( "END_OF Playlist = %1" ).arg( end ) );
    SimpleLogger::instance()->debug( Constants::LOGGER_COVERVIEWPLAYLIST, "begin" );
}

QString CoverViewPlaylistUI::getMediaTypeName( QUuid mediaTypeId ) {
    if ( mediaTypeId == _mediaDB->videoType()->guid() )
        return Constants::MEDIA_TYPE_VIDEO_NAME;

    if ( mediaTypeId == _mediaDB->audioType()->guid() )
        return Constants::MEDIA_TYPE_AUDIO_NAME;

    if ( mediaTypeId == _mediaDB->imageType()->guid() )
        return Constants::MEDIA_TYPE_IMAGE_NAME;

    if ( mediaTypeId == _mediaDB->documentType()->guid() )
        return Constants::MEDIA_TYPE_DOCUMENT_NAME;

    return "";
}

bool CoverViewPlaylistUI::hasCentralFocus() {
    return _hasCentralFocus;
}

bool CoverViewPlaylistUI::hasMainFocus() {
    return _hasMainUIRegionFocus;
}

bool CoverViewPlaylistUI::hasUserSelectedItem() {
    return _userSelectedItem;
}

void CoverViewPlaylistUI::refreshDisplayFormat( int width, int height ) {
    if( ( width / height ) < 1.77 ) {
        _numberItemsCurrent = _numberItemsFormat4To3;
        _currentDisplayFormatType = CoverViewPlaylistUI::FOUR_TO_THREE;
    } else {
        _numberItemsCurrent = _numberItemsFormat16To9;
        _currentDisplayFormatType = CoverViewPlaylistUI::SIXTEEN_TO_NINE;
    }
}

void CoverViewPlaylistUI:: resolveNewsAboutDetailOpenFlag( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    if( _coverViewContentType == PlaylistContentTypeChangedEvent::NEWS ) {
        if( widgetType == CentralFocusChangedEvent::NEWS_DETAIL )
            _isNewsAboutDetailOpen = true;
        else
            _isNewsAboutDetailOpen = false;
    } else {
        if( _coverViewContentType == PlaylistContentTypeChangedEvent::ABOUT ) {
            if( widgetType == CentralFocusChangedEvent::ABOUT_DETAIL )
                _isNewsAboutDetailOpen = true;
            else
                _isNewsAboutDetailOpen = false;
        } else {
            if( _coverViewContentType == PlaylistContentTypeChangedEvent::SETTINGS ) {
                if( widgetType == CentralFocusChangedEvent::SETTINGS_DETAIL )
                    _isNewsAboutDetailOpen = true;
                else
                    _isNewsAboutDetailOpen = false;
            }
        }
    }
}

bool CoverViewPlaylistUI::isNewsAboutDetailOpen() {
    return _isNewsAboutDetailOpen;
}


