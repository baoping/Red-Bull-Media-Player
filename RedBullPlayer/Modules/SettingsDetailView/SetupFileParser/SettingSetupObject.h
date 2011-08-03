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
#ifndef SETTINGSETUPOBJECT_H
#define SETTINGSETUPOBJECT_H

#include "../../../Interfaces/UserSettings/ISetupSettingObject.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class SettingSetupObject : public ISetupSettingObject {
                    Q_OBJECT

                public:
                    SettingSetupObject( QString id, QString type, QString labelText, QObject *parent )
                        : ISetupSettingObject( parent ) {
                        _id = id;
                        _type = type;
                        _labelText = labelText;
                    }

                    virtual QString getId() {
                        return _id;
                    }
                    virtual QString getType() {
                        return _type;
                    }
                    virtual QString getLabelText() {
                        return _labelText;
                    }

                private:
                    QString _id;
                    QString _type;
                    QString _labelText;
            };
        }
    }
}

#endif // SETTINGSETUPOBJECT_H
