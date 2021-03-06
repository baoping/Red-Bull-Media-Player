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
#ifndef ITASKPRESENTER_H
#define ITASKPRESENTER_H

#include <QObject>
#include <QString>
#include <QUuid>

#include "IStatePresenter.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class ITaskPresenter : public RedBullPlayer::Modules::StateDisplay::IStatePresenter {
                    Q_OBJECT

                public:
                    ITaskPresenter( QObject* parent = 0 ) : IStatePresenter( parent ) { }
                    virtual ~ITaskPresenter() { }

                signals:
                    void requestTaskCancel( QUuid );

            };
        }
    }
}

#endif // ITASKPRESENTER_H
