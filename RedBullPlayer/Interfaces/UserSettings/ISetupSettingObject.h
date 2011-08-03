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
#ifndef ISETTINGSVIEWSETUPSETTINGOBJECT_H
#define ISETTINGSVIEWSETUPSETTINGOBJECT_H

#include <QObject>
#include <QString>

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class ISetupSettingObject : public QObject  {
                    Q_OBJECT

                public:
                    ISetupSettingObject( QObject* parent = 0 ) : QObject( parent ) { }
                    virtual ~ISetupSettingObject() {}
                    virtual QString getLabelText() = 0;
                    virtual QString getType() = 0;
                    virtual QString getId() = 0;
            };
        }
    }
}


#endif // ISETTINGSVIEWSETUPSETTINGOBJECT_H
