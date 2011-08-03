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
#ifndef GRIDVIEWPLAYLISTUI_H
#define GRIDVIEWPLAYLISTUI_H

#include <QObject>
#include <QPointer>
#include <QtGui/QWidget>
#include <QUuid>
#include <QHash>
#include <QPixmap>

// Animation
#include "qtparallelanimationgroup.h"
#include "qteasingcurve.h"

// Project Includes
#include "../../Interfaces/IPlaylist.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMediaRepository.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Widgets/GridViewTransformWidget.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/Playlist/EndOfPlaylistEvent.h"
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../PlayerShell/Shell.h"

namespace Ui {
    class GridViewPlaylistUI;
}

class GridViewPlaylistUI : public QWidget {
        Q_OBJECT
    public:
        enum DisplayFormatType {
            FOUR_TO_THREE,
            SIXTEEN_TO_NINE
        };

        GridViewPlaylistUI( RedBullPlayer::PlayerShell::Shell *shell, QWidget *parent = 0 );
        ~GridViewPlaylistUI();

        void setupPlaylist( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> playlist );

        void updateItems();
        void changeMainUIRegionFocusGraphics( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUIRegion );
        void handleCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );
        void handleCoverImageChanged( QUuid mediaId );
        void handleFocusedPlaylistItemChanged( int newItemIndex, bool selectedByUser );
        void handleMediaItemContentChangedEvent( QUuid mediaId );
        void handlePlayerTrackStarted( QUuid mediaId );
        void handleDocumentOpened( QUuid mediaId );
        void cancelAllCoverImageRequsts();
        void scrollLeft();
        void scrollRight();
        void setupMediaLibraryConnection();

        void addMediaItem( QUuid mediaId, RedBullPlayer::Events::MediaDiscoveredEvent::MediaDiscoveredSource source );
        void handleSearchPlaylistGenerationStarted( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> newPlaylist );
        void handleCachedPlaylistRestored( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> cahcedPlaylist );
        void handleRemovedItemFromPlaylist( QUuid removedItemId );

        bool hasMainFocus();
        bool hasCentralFocus();
        void playCurrentItem();
        void selectNextItemAnimated();
        void selectPreviousItemAnimated();

        void resolveStyleFactory();
        void setupStyle();

    public slots:
        void playlistItemDoubleClicked( int id );
        void playlistItemClicked( int id );
        void animationFinishedSlot();

    protected:
        void changeEvent( QEvent *e );
        void resizeEvent( QResizeEvent *e );
        void setPlaylist( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> newPlaylist );
        void clearItems();
        void initItems();
        void displayItems();
        void displayItem( RedBullPlayer::Widgets::GridViewTransformWidget *item, int indexInList );
        void updateDisplayWidgetsId();
        void emitEndOfPlaylist();
        bool isBeginning();
        bool isEnd();
        int getStartItemIndexOfPage( int pageIndex );
        int getEndItemIndexOfPage( int pageIndex );
        QtParallelAnimationGroup *getScrollAnimationForPage( int pageIndex, bool scrollLeft, int animationLength );
        void animateScroll( bool scrollLeft );
        void emitCoverImageRequestEvent( QUuid mediaId );
        void emitCancelCoverImageCreationEvent( QUuid mediaId );
        void emitFocusedItemChangedEvent( int newItemIndex, bool selectedByUser );
        void emitMediaItemContentChanged( QUuid mediaId );
        void emitRequestAlbumCover( QUuid mediaId );
        void emitAddedItemToPlaylist( QUuid mediaId, RedBullPlayer::Events::MediaDiscoveredEvent::MediaDiscoveredSource source );
        void selectItem( int itemIndex );

    private:
        void refreshItemSize();
        void refreshDisplayFormat( int width, int height );
        int getPageIndexOfItem( int indexInList );
        QString getMediaTypeName( QUuid mediaTypeId );
        void setCurrentIndexAfterAddItem( RedBullPlayer::Events::MediaDiscoveredEvent::MediaDiscoveredSource source );
        void setCurrentIndexToUserSelectOrFirst();
        void storeSelectedItemId();
        void setCurrentIndexToSelectedInPreviousPlaylistOrFirst();
        void setCurrentIndexToUsedAddedItem();

        Ui::GridViewPlaylistUI *_ui;
        RedBullPlayer::PlayerShell::Shell *_shell;
        QWidget *_parentWidget;

        QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> _playlistItems;
        QList<RedBullPlayer::Widgets::GridViewTransformWidget*> _playlistItemsWidgets;

        int _itemWidth;
        int _itemHeight;
        int _spaceBetweenRows;
        int _numberColumnsFormat4To3;
        int _numberRowsFormat4To3;
        int _numberColumnsFormat16To9;
        int _numberRowsFormat16To9;
        int _numberRowsCurrent;
        int _numberColumnsCurrent;
        DisplayFormatType _currentDisplayFormatType;
        QPixmap _isNewIcon;

        int _currentPageIndex;
        int _currentItemIndex;
        QUuid _curentItemMediaId;
        QUuid _selectedItemInPreviousPlaylistId;

        QtEasingCurve EASING_CURVE;
        bool _animationOn;

        bool _userSelectedItem;
        bool _hasCentralFocus;
        RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType _widgetType;
        bool _hasMainUIRegionFocus;
        RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion _mainUIRegionGroup;

        QHash<QUuid, int> _waitingItemsForCoverImage;

        RedBullPlayer::Modules::MediaLibrary::IMediaDB *_mediaDB;
        RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
        RedBullPlayer::Modules::StyleFactory::ITextStyle *_displayItemTextStyle;

};

#endif // GRIDVIEWPLAYLISTUI_H
