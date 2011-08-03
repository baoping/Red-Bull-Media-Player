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
#ifndef LISTVIEWPLAYLISTUI_H
#define LISTVIEWPLAYLISTUI_H

#include <QObject>
#include <QPointer>
#include <QtGui/QWidget>
#include <QUuid>
#include <QHash>

// QtAnimationFramework
#include "qtparallelanimationgroup.h"
#include "qteasingcurve.h"

// Project Includes
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/IPlaylist.h"
#include "../../Widgets/ListViewTransformWidget.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/Playlist/EndOfPlaylistEvent.h"
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMediaRepository.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"

namespace Ui {
    class ListViewPlaylistUI;
}

class ListViewPlaylistUI : public QWidget {
    Q_OBJECT
public:
    ListViewPlaylistUI( RedBullPlayer::PlayerShell::Shell *shell, QWidget *parent = 0 );
    ~ListViewPlaylistUI();

    void setupPlaylist( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> playlist );
    void updateItems();
    void changeMainUIRegionFocusGraphics( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUIRegion );
    void handleCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );
    void handleFocusedPlaylistItemChanged( int newItemIndex, bool selectedByUser );
    void handleMediaItemContentChangedEvent( QUuid mediaId );
    void handleDocumentOpened( QUuid mediaId );

    void scrollLeft();
    void scrollRight();
    void setupMediaLibraryConnection();
    void addMediaItem( QUuid mediaId, RedBullPlayer::Events::MediaDiscoveredEvent::MediaDiscoveredSource source );

    bool hasMainFocus();
    bool hasCentralFocus();
    void playCurrentItem();
    void selectNextItemAnimated();
    void selectPreviousItemAnimated();

    void handleSearchPlaylistGenerationStarted( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> newPlaylist );
    void handleCachedPlaylistRestored( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> cahcedPlaylist );
    void handleRemovedItemFromPlaylist( QUuid removedItemId );

    void resolveStyleFactory();
    void setupStyle();

public slots:
    void playlistItemDoubleClicked( int id );
    void playlistItemClicked( int id );
    void animationFinishedSlot();

protected:
    void changeEvent( QEvent *e );
    void paintEvent( QPaintEvent *e );
    void resizeEvent( QResizeEvent *e );
    void setPlaylist( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> newPlaylist );
    void clearItems();
    void initItems();
    void displayItems();
    void displayItem( RedBullPlayer::Widgets::ListViewTransformWidget *item, int indexInList );
    void updateDisplayWidgetsId();
    void emitEndOfPlaylist();
    bool isBeginning();
    bool isEnd();
    int getStartItemIndexOfPage( int pageIndex );
    int getEndItemIndexOfPage( int pageIndex );
    QtParallelAnimationGroup *getScrollAnimationForPage( int pageIndex, bool scrollLeft, int animationLength );
    void animateScroll( bool scrollLeft );
    void emitFocusedItemChangedEvent( int newItemIndex, bool selectedByUser );
    void selectItem( int itemIndex );
    void setupHeaderTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *headerTextStyle );

private:
    Ui::ListViewPlaylistUI *_ui;
    RedBullPlayer::PlayerShell::Shell *_shell;
    QWidget *_parentWidget;

    QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> _playlistItems;
    QList<RedBullPlayer::Widgets::ListViewTransformWidget*> _playlistItemsWidgets;

    int _itemHeight;
    int _spaceBetweenPages;
    int _currentNumberRows;
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

    RedBullPlayer::Modules::MediaLibrary::IMediaDB *_mediaDB;
    RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
    RedBullPlayer::Modules::StyleFactory::ITextStyle *_displayItemTextNormalStyle;
    RedBullPlayer::Modules::StyleFactory::ITextStyle *_displayItemTextSelectedStyle;
    QColor _headerTextColor;
    QFont _headerTextFont;

    QPixmap _imageHeaderBackground;
    QPixmap _noMainRegionFocusPixmap;

    int _isNewColumnOffsetX;

private:
    void refreshNumberOfRows();
    int getPageIndexOfItem( int indexInList );
    QString getMediaTypeName( QUuid mediaTypeId );
    void setCurrentIndexAfterAddItem( RedBullPlayer::Events::MediaDiscoveredEvent::MediaDiscoveredSource source );
    void setCurrentIndexToUserSelectOrFirst();
    void storeSelectedItemId();
    void setCurrentIndexToSelectedInPreviousPlaylistOrFirst();
    void setCurrentIndexToUsedAddedItem();
    void initIsNewIconOffsetX();
};

#endif // LISTVIEWPLAYLISTUI_H
