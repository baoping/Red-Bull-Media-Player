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
#ifndef SETTINGSDETAILVIEWUI_H
#define SETTINGSDETAILVIEWUI_H

//QT includes
#include <QtGui/QWidget>
#include <QPixmap>

//Project includes
#include "../../Widgets/Button.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Interfaces/UserSettings/ISettingsViewFactory.h"
#include "../../Interfaces/UserSettings/ISettingsSectionView.h"
#include "../../Interfaces/UserSettings/ISettingsPresenter.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"


namespace Ui {
    class SettingsDetailViewUI;
}

class SettingsDetailViewUI : public QWidget {
        Q_OBJECT
    public:
        SettingsDetailViewUI( QWidget *parent = 0 );
        ~SettingsDetailViewUI();

        void setPlaylistRegionWidget( QWidget *playlistRegion );
        void setStyleFactory( RedBullPlayer::Modules::StyleFactory::IStyleFactory *styleFactory );
        void setMediaLibrary( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *mediaLibrary );

        void setupStyle();
        void handlePlaylistItemSelected( RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem );
        void handleCentralUiRegionResized();
        void handleMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion mainUiRegion );
        void handleCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType widgetType );
        void handleFunctionSelected( QString functionTitle );
        void handleKeyPressed( int key );

    public slots:
        void exitClicked();

    signals:
        void emitCentralFocusChanged( bool hasCentralFocus );
        void emitPlaylistNextItemSelected();
        void emitPlaylistPreviousItemSelected();
        void emitMainRegionFocusChanged();
        void userSettingChanged();

    protected:
        void changeEvent( QEvent *e );
        void paintEvent( QPaintEvent *e );
        void enterEvent( QEvent *e );

        void addSectionViews( QList<RedBullPlayer::Modules::SettingsDetailView::ISettingsSectionView *> sections );
        void clearContentArea();

    private:
        void initExitButton();
        void setupTitleStyle();
        void setVisibillity( bool visible );
        void styleScrollBar();

    private slots:
        void refreshGeometry();

    private:
        QPixmap _backgroundImage;
        QPixmap _iconImage;
        RedBullPlayer::Widgets::Button *_exitButton;
        bool _forceToShowNextTime;
        bool _hasCentralFocus;
        bool _hasMainFocus;

        RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType _widgetType;
        RedBullPlayer::Events::MainUIRegionFocusChangedEvent::UIRegion _mainUIRegion;

        QWidget *_playlistRegion;
        QWidget *_moduleRegionWidget;
        RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;

        RedBullPlayer::Modules::SettingsDetailView::ISettingsViewFactory *_viewFactory;
        RedBullPlayer::Modules::SettingsDetailView::ISettingsPresenter *_settingsPresenter;

        Ui::SettingsDetailViewUI *_ui;
};

#endif // SETTINGSDETAILVIEWUI_H
