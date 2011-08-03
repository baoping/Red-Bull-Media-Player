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
#ifndef ISEARCHUI_H
#define ISEARCHUI_H

#include <QObject>

#include "../Container/Plugin.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SearchUI {
            class ISearchUI : public RedBullPlayer::Container::Plugin {
                    Q_OBJECT
                public:
                    virtual ~ISearchUI() {}
            };
        }
    }
}

Q_DECLARE_INTERFACE( RedBullPlayer::Modules::SearchUI::ISearchUI, "at.mii.redbullplayer.modules.isearchui/1.0" )

#endif // ISEARCHUI_H
