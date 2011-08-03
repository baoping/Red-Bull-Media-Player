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
#ifndef PLAYLISTUI_H
#define PLAYLISTUI_H

#include <QtGui/QWidget>
#include <QtGui/QStackedWidget>
#include <QUuid>
#include <QTimer>
#include <QStringList>

// Project Includes
#include "../../PlayerShell/Shell.h"
#include "../../Widgets/TransparentImage.h"
#include "../../Widgets/Button.h"
#include "../../Widgets/CommandButton.h"
#include "../../Widgets/ToogleButton.h"
#include "../../Interfaces/IPlaylistItem.h"
#include "../../Interfaces/ICoverViewPlaylist.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMediaRepository.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Events/FocusedPlayListItemChangedEvent.h"
#include "../../Events/Playlist/PlaylistNavigationKeyPressedEvent.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/Playlist/EndOfPlaylistEvent.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../Events/Playlist/MediaVanishedEvent.h"
#include "../../Events/Playlist/MediaRemovedEvent.h"


namespace Ui {
    class PlaylistUI;
}

class PlaylistUI : public QWidget {
        Q_OBJECT
    public:
        PlaylistUI( RedBullPlayer::PlayerShell::Shell *shell, QWidget *parent = 0 );
        ~PlaylistUI();

        void setNavigation();
        bool hasMainFocus();
        bool hasCentralFocus();

        enum PlaylistTypes {
            GRIDVIEW = 0,
            LISTVIEW = 1,
            COVERVIEW = 2,
            TV = 3,
            SETTINGS = 4,
            NEWS = 5,
            ABOUT = 6,
            MEDIA = 100
        };

        void setupMediaLibraryConnection();
        void handleFocusedPlaylistItemChanged( QString title, QUuid mediaId );
        void resolveStyleFactory();
        void setupStyle();
        void clearTitle();

    public slots:
        bool onLeftNavigationClicked();
        bool onRightNavigationClicked();
        void gridViewClicked();
        void listViewClicked();
        void coverflowViewClicked();
        void plusClicked();
        void minusClicked();
        void setupTitleStyle();

    public:
        void changeTitle( QString title );
        void setNavigationKeysState( RedBullPlayer::Events::EndOfPlaylistEvent::EndOfPlaylistDirection endOfPlaylist );
        void SetPlaylistType( PlaylistTypes playlistType );
        void handleCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );
        void changeMainUIRegionFocusGraphics( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUIRegion );

    protected:
        void changeEvent( QEvent *e );
        void resizeEvent( QResizeEvent *e );
        void setupPlaylistButtons();
        void setupPlaylistStackedWidget();
        void emitNavigationKeyPressedEvent( RedBullPlayer::Events::PlaylistNavigationKeyPressedEvent::Direction direction );
        void emitCentralFocusChangedEvent( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );
        RedBullPlayer::Modules::Playlist::CoverViewPlaylist::ICoverViewPlaylist* loadMediaCoverView();
        int resolveTitleWidth();

        void dropEvent( QDropEvent *e );
        void dragMoveEvent( QDragMoveEvent *e );
        void dragEnterEvent( QDragEnterEvent *e );

    private slots:
        void handleCurrentFreeDiskSpace();


    private: // Methods
        void showDiskSpaceError();
        void tryAddFiles( QStringList fileNames );
        void setupFreeDiskSpaceCheck();

    private: // DATA
        Ui::PlaylistUI *_ui;
        RedBullPlayer::PlayerShell::Shell *_shell;
        RedBullPlayer::Widgets::CommandButton *_leftButton;
        RedBullPlayer::Widgets::CommandButton *_rightButton;
        RedBullPlayer::Widgets::ToogleButton *_gridViewButton;
        RedBullPlayer::Widgets::ToogleButton *_listViewButton;
        RedBullPlayer::Widgets::ToogleButton *_coverflowViewButton;
        RedBullPlayer::Widgets::CommandButton *_plusButton;
        RedBullPlayer::Widgets::CommandButton *_minusButton;
        PlaylistUI::PlaylistTypes _lastMediaType;
        PlaylistUI::PlaylistTypes _currentPlaylistType;
        bool _hasCentralFocus;
        bool _hasMainUIRegionFocus;
        RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion _mainUIRegionGroup;
        QUuid _currentSelectedMediaId;
        RedBullPlayer::Modules::MediaLibrary::IMediaDB *_mediaDB;
        QString _currentTitle;
        RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
        QTimer *_freeDiskSpaceCheckTimer;
};

#endif // PLAYLISTUI_H
