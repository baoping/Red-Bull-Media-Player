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
#ifndef TOPUIMODULE_H
#define TOPUIMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/ITopUI.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/UpdateInstaller/IUpdateInstaller.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/StateDisplay/HoverBoxHasFocusEvent.h"
#include "TopUICreator.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TopUI {
            class TopUIModule : public ITopUI {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::TopUI::ITopUI )

                public:
                    TopUIModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "TopUI";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                public slots:
                    void exitClickedSlot();
                    void draggingSlot( QPoint distance );
                    void doubleClickedSlot();

                protected:
                    bool event( QEvent *event );
                    void onMainUIRegionFocusChanged( RedBullPlayer::Events::MainUIRegionFocusChangedEvent *event );

                private slots:
                    void closeShell();
                private: // Methods
                    void resolveMediaLibrary();
                    void resolveUpdateInstallerLibrary();
                    void resolveStyleFacctory();

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    TopUICreator *_ui;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    RedBullPlayer::Modules::UpdateInstaller::IUpdateInstaller* _updateInstaller;
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
            };
        }
    }
}

#endif // TOPUIMODULE_H
