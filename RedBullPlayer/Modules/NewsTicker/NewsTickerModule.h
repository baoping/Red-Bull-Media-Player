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
#ifndef NEWSTICKER_H
#define NEWSTICKER_H

//QT Includes
#include <QObject>
#include <QString>
#include <QEvent>
#include <QList>
#include <QPointer>

// Project Includes
#include "../../Interfaces/INewsTicker.h"
#include "../../Container/Plugin.h"
#include "../../PlayerShell/Shell.h"
#include "../../PlayerShell/UI/Region.h"
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "NewsTickerUI.h"


class QLabel;

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;

namespace RedBullPlayer {
    namespace Modules {
        namespace NewsTicker {
            class NewsTickerModule : public INewsTicker {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::NewsTicker::INewsTicker )
                public:
                    NewsTickerModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "NewsTicker";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                protected:
                    virtual bool event ( QEvent * e );

                private:
                    RedBullPlayer::PlayerShell::Shell* _shell;
                    RedBullPlayer::PlayerShell::Region* _region;
            };
        }
    }
}
#endif // NEWSTICKER_H
