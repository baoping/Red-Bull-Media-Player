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
#ifndef ISTATEPRESENTER_H
#define ISTATEPRESENTER_H

#include <QObject>
#include <QString>
#include <QUuid>

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class IStatePresenter : public QObject {
                    Q_OBJECT

                public:
                    IStatePresenter( QObject* parent = 0 ) : QObject( parent ) { }
                    virtual ~IStatePresenter() {}
                    virtual QUuid guid() const = 0;

                    virtual void showView( int x, int y ) = 0;
                    virtual void hideView() = 0;
                    virtual bool isViewVisible() = 0;

                signals:
                    void stateIconVisibilityChanged( bool visible );
            };
        }
    }
}

#endif // ISTATEPRESENTER_H
