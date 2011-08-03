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
#ifndef SETTINGSDETAILVIEWMODULE_H
#define SETTINGSDETAILVIEWMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/UserSettings/ISettingsDetailView.h"
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Events/Playlist/PlaylistItemSelectedEvent.h"
#include "../../Events/FunctionSelectedEvent.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/CentralUIRegionResizedEvent.h"
#include "SettingsDetailViewUI.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class  SettingsDetailViewModule : public ISettingsDetailView {
                    Q_OBJECT;
                    Q_INTERFACES( RedBullPlayer::Modules::SettingsDetailView::ISettingsDetailView );

                public:
                    SettingsDetailViewModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "SettingsDetailView";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                public slots:
                    void emitCentralFocusChangedSlot( bool hasCentralFocus );
                    void emitNextPlaylistItemSelectedSlot();
                    void emitPreviousPlaylistItemSelectedSlot();
                    void emitMainRegionFocusChangedSlot();
                    void emitUserSettingChangedSlot();

                protected:
                    virtual bool event ( QEvent * e );
                    void onPlaylistItemSelected( RedBullPlayer::Events::PlaylistItemSelectedEvent* e );
                    void onFunctionSelected( RedBullPlayer::Events::FunctionSelectedEvent* e );
                    void onKeyPressed( RedBullPlayer::Events::KeyPressedEvent *event );
                    void onCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent *event );
                    void onMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent *event );
                    void onCentralUIRegiontResized( RedBullPlayer::Events::CentralUIRegionResizedEvent *event );

                    void resolveStyleFactory();
                    void setupUiStyle();
                    void resolvePlaylistRegion();
                    void resolveMediaLibrary();

                private:
                    SettingsDetailViewUI *_ui;
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;

                    RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *_mediaLibrary;

            };
        }
    }
}

#endif // SETTINGSDETAILVIEWMODULE_H
