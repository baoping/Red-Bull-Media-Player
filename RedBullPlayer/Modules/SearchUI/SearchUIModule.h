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
#ifndef SEARCHUIMODULE_H
#define SEARCHUIMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/ISearchUI.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/CentralFocusChangedEvent.h"
#include "../../Events/Playlist/PlaylistGenerationFinishedEvent.h"
#include "SearchUICreator.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace SearchUI {
            class SearchUIModule : public ISearchUI {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::SearchUI::ISearchUI )

                public:
                    SearchUIModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "SearchUI";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                protected:
                    bool event( QEvent * e );
                    void onMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent *event );
                    void onCentralFocusChanged( RedBullPlayer::Events::CentralFocusChangedEvent *e );
                    void onPlaylistGenerationFinished( RedBullPlayer::Events::PlaylistGenerationFinishedEvent *e );

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    SearchUICreator *_ui;
            };
        }
    }
}


#endif // SEARCHUIMODULE_H
