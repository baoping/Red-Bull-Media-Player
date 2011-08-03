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
#ifndef ICHAPTERSELECT_H
#define ICHAPTERSELECT_H

#include <QObject>

#include "../Container/Plugin.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ChapterSelect {
            class IChapterSelect : public RedBullPlayer::Container::Plugin {
                    Q_OBJECT

                public:
                    virtual ~IChapterSelect() {}
            };
        }
    }
}

Q_DECLARE_INTERFACE( RedBullPlayer::Modules::ChapterSelect::IChapterSelect, "at.mii.redbullplayer.modules.ichapterselect/1.0" )

#endif // ICHAPTERSELECT_H
