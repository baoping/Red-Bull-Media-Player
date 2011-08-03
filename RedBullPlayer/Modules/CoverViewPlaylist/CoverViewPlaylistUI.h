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
#ifndef COVERVIEWPLAYLISTUI_H
#define COVERVIEWPLAYLISTUI_H

// QT includes
#include <QObject>
#include <QPointer>
#include <QtGui/QWidget>
#include <QUuid>

// Animation
#include "qtparallelanimationgroup.h"
#include "qteasingcurve.h"

// Project Includes
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/IPlaylist.h"
#include "../../Widgets/TransformWidget.h"
#include "../PlaylistUI/TransformParams.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/Playlist/PlaylistContentTypeChangedEvent.h"
#include "../../Events/Playlist/EndOfPlaylistEvent.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMediaRepository.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/Playlist/PlaylistNavigationKeyPressedEvent.h"


namespace Ui {
    class CoverViewPlaylistUI;
}

class CoverViewPlaylistUI : public QWidget {
        Q_OBJECT
    public:
        enum DisplayFormatType {
            FOUR_TO_THREE,
            SIXTEEN_TO_NINE
        };

        CoverViewPlaylistUI( RedBullPlayer::PlayerShell::Shell *shell,
                             RedBullPlayer::Events::PlaylistContentTypeChangedEvent::ContentType contentType, QWidget *parent = 0 );
        ~CoverViewPlaylistUI();

        void setupPlaylist( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> playlist );

        void scrollLeft();
        void scrollRight();
        void playCurrentItem( bool skipDocuments = false, bool fromSelectPreviousItemEvent = false );
        RedBullPlayer::Modules::Playlist::IPlaylistItem *getCurrentItem();
        int getCurrentIndex();
        void setCurrentIndex( int index );
        void setPlaylist( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> newPlaylist );
        bool isBeginning();
        bool isEnd();
        void updateItems();
        void changeMainUIRegionFocusGraphics( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUIRegion );
        void handleCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );
        void handleMediaItemContentChangedEvent( QUuid mediaId );
        void handleDocumentOpened( QUuid mediaId );
        void emitEndOfPlaylist();
        void setupMediaLibraryConnection();
        void handleCoverImageChanged( QUuid mediaId );
        void handleFocusedPlaylistItemChanged( int newItemIndex, bool selectedByUser );
        void addMediaItem( QUuid mediaId, RedBullPlayer::Events::MediaDiscoveredEvent::MediaDiscoveredSource source );
        void selectItem( int itemIndex, bool selectedByUser );
        bool hasUserSelectedItem();
        void handleSearchPlaylistGenerationStarted( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> newPlaylist );
        void handleCachedPlaylistRestored( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> cahcedPlaylist );
        void handleRemovedItemFromPlaylist( QUuid removedItemId );
        void handlePlaylistNavigationKeyPressed( RedBullPlayer::Events::PlaylistNavigationKeyPressedEvent::Direction direction );

        bool hasMainFocus();
        bool hasCentralFocus();
        bool isNewsAboutDetailOpen();


    public slots:
        void playlistItemClicked( int id );
        void playlistItemDoubleClicked( int id );
        void animationFinishedSlot();

    protected:
        void changeEvent( QEvent *e );
        void resizeEvent( QResizeEvent *e );
        void initItems();
        void displayItems();
        void clearItems();
        void displayItem( RedBullPlayer::Widgets::TransformWidget *item, int indexOnScreen );
        void updateDisplayWidgetsId();
        int getXOfItem( int index );
        int getYOfItem( int index );
        RedBullPlayer::Widgets::MirrorWidget::FadeStyle getFadeStyleOfItem( int index );
        int getItemWidth( int index );
        int getItemHeight( int index );
        int getTopRightDiffOfItem( int index );
        TransformParams getTransformParamsForItem( int indexOnScreen );
        QtParallelAnimationGroup *getAnimationForItem( RedBullPlayer::Widgets::TransformWidget *widget, int indexOnScreen, bool scrollingLeft, int animLength );
        QtParallelAnimationGroup *getScrollToIndexAnimationForItem( RedBullPlayer::Widgets::TransformWidget *widget, int indexOnScreen, int indexDiffrence, int animLength );
        void scroll( bool left );
        void scrollToIndex( int indexOnScreenToBeCurrent );
        void animateScrollToIndex( int indexOnScreenToBeCurrent );
        QImage getImageForContentType( RedBullPlayer::Modules::Playlist::IPlaylistItem *item );
        void resolveNewsAboutDetailOpenFlag( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );


    private:
        void refreshDisplayFormat( int width, int height );
        QString getMediaTypeName( QUuid mediaTypeId );
        void setCurrentIndexAfterAddItem( RedBullPlayer::Events::MediaDiscoveredEvent::MediaDiscoveredSource source );
        void setCurrentIndexToUserSelectOrFirst();
        void storeSelectedItemId();
        void setCurrentIndexToSelectedInPreviousPlaylistOrFirst();
        void setCurrentIndexToUsedAddedItem();

        Ui::CoverViewPlaylistUI *_ui;
        RedBullPlayer::PlayerShell::Shell *_shell;

        QWidget *_parentWidget;
        QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> _playlistItems;
        QList<RedBullPlayer::Widgets::TransformWidget*> _playlistItemsWidgets;
        int _currentIndex;   // index of currently Selected item aka item in the middle
        QUuid _currentMediaItemId;
        QUuid _selectedItemInPreviousPlaylistId;
        QWidget *_rootWidget;
        QtEasingCurve EASING_CURVE;
        bool _animationOn;
        bool _hasCentralFocus;
        bool _userSelectedItem;
        bool _hasMainUIRegionFocus;
        RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion _mainUIRegionGroup;
        RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType _widgetType;
        RedBullPlayer::Modules::MediaLibrary::IMediaDB *_mediaDB;
        bool _isNewsAboutDetailOpen;

        int _numberItemsFormat4To3;
        int _numberItemsFormat16To9;
        int _numberItemsCurrent;
        DisplayFormatType _currentDisplayFormatType;
        RedBullPlayer::Events::PlaylistContentTypeChangedEvent::ContentType _coverViewContentType;

    signals:
        void itemsRearranged();

};

#endif // COVERVIEWPLAYLISTUI_H
