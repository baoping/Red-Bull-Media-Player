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
#ifndef INEWSTICKER_H
#define INEWSTICKER_H

#include <QObject>

#include "../Container/Plugin.h"

using namespace RedBullPlayer::Container;

namespace RedBullPlayer {
    namespace Modules {
        namespace NewsTicker {
            class INewsTicker :  public Plugin {
                    Q_OBJECT
                public:
                    virtual ~INewsTicker() {}
            };
        }
    }
}

Q_DECLARE_INTERFACE( RedBullPlayer::Modules::NewsTicker::INewsTicker, "at.mii.redbullplayer.modules.newsticker/1.0" )

#endif // INEWSTICKER_H
