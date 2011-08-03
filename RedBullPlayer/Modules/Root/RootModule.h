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
#ifndef ROOTMODULE_H
#define ROOTMODULE_H

//QT Includes
#include <QObject>
#include <QString>
#include <QEvent>

// Project Includes
#include "../../Interfaces/IRootModule.h"
#include "../../Container/Plugin.h"
#include "../../PlayerShell/Shell.h"
#include "../../PlayerShell/UI/Region.h"
#include "MainUI.h"

using namespace RedBullPlayer::Container;

namespace RedBullPlayer {
    namespace Modules {
        namespace Root {
            class RootModule : public IRootModule {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::Root::IRootModule )
                public:
                    RootModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "Root";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }
                protected:
                    virtual bool event( QEvent* event );
                private:
                    RedBullPlayer::PlayerShell::Shell* _shell;
                    RedBullPlayer::PlayerShell::Region* _region;
                    MainUI* _ui;
            };
        }
    }
}
#endif // NEWSTICKER_H
