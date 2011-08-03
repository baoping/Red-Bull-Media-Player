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
#ifndef NEWSDETAILVIEWMODULE_H
#define NEWSDETAILVIEWMODULE_H

// Project Includes
#include "../../Interfaces/INewsDetailView.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/Playlist/PlaylistItemSelectedEvent.h"
#include "../../Events/FunctionSelectedEvent.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/CentralUIRegionResizedEvent.h"

#include "NewsDetailViewUI.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace NewsDetailView {
            class NewsDetailViewModule : public INewsDetailView {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::NewsDetailView::INewsDetailView )

                public slots:
                    void exitClickedSlot();

                public:
                    NewsDetailViewModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "NewsDetailView";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                protected:
                    virtual bool event ( QEvent * e );
                    void setVisibility( bool visible );
                    void onPlaylistItemSelected( RedBullPlayer::Events::PlaylistItemSelectedEvent* e );
                    void onFunctionSelected( RedBullPlayer::Events::FunctionSelectedEvent* e );
                    void onKeyPressed( RedBullPlayer::Events::KeyPressedEvent *event );
                    void onCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent *event );
                    void onMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent *event );
                    void onCentralUIRegionResized( RedBullPlayer::Events::CentralUIRegionResizedEvent *e );
                    void resolveStyleFactory();
                    void resolveMediaLibrary();
                    void setupUiStyle();
                    void getFixedSize();
                    QSize getSmallestMonitorScreenSize();

                protected slots:
                    void updateGeometry();

                private:
                    RedBullPlayer::PlayerShell::Shell* _shell;
                    RedBullPlayer::PlayerShell::Region* _region;
                    NewsDetailViewUI *_ui;
                    bool _forceToShowNextTime;
                    RedBullPlayer::Events::CentralFocusChangedEvent::CentralWidgetType _currentCentralWidgetType;
                    bool _hasMainFocus;
                    int _fixedWidth;
                    int _fixedHeight;
            };
        }
    }
}

#endif // NEWSDETAILVIEWMODULE_H
