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
#ifndef ISTYLEFACTORY_H
#define ISTYLEFACTORY_H

#include <QObject>
#include <QUuid>

#include "../../Container/Plugin.h"
#include "ITextStyle.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StyleFactory {
            class IStyleFactory : public RedBullPlayer::Container::Plugin {
                    Q_OBJECT

                public:
                    virtual ~IStyleFactory() {}
                    virtual ITextStyle* getTextStyle( QString name, bool *ok ) = 0;
            };
        }
    }
}

Q_DECLARE_INTERFACE( RedBullPlayer::Modules::StyleFactory::IStyleFactory, "at.mii.redbullplayer.modules.styleFactory/1.0" )

#endif // ISTYLEFACTORY_H
