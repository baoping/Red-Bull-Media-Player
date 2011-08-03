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
#ifndef SETTINGSVIEWSETUPSECTIONOBJECT_H
#define SETTINGSVIEWSETUPSECTIONOBJECT_H

#include "../../../Interfaces/UserSettings/ISetupSectionObject.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class SectionSetupObject : public ISetupSectionObject {
                    Q_OBJECT

                public:
                    SectionSetupObject( QString id, QString title, QString helpText, QObject *parent )
                        : ISetupSectionObject( parent ) {
                        _id = id;
                        _title = title;
                        _helpText = helpText;
                    }

                    virtual QString getId() {
                        return _id;
                    }
                    virtual QString getTitleText() {
                        return _title;
                    }
                    virtual QString getHelpText() {
                        return _helpText;
                    }

                private:
                    QString _id;
                    QString _title;
                    QString _helpText;
            };
        }
    }
}

#endif // SETTINGSVIEWSETUPSECTIONOBJECT_H
