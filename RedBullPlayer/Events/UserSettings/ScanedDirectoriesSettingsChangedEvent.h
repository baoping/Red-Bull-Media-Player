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
#ifndef SCANEDDIRECTORIESSETTINGSCHANGEDEVENT_H
#define SCANEDDIRECTORIESSETTINGSCHANGEDEVENT_H

//QT Includes
#include <QEvent>
#include <QStringList>

#include "../Events_global.h"
#include "../EventTypes.h"

namespace RedBullPlayer {
    namespace Events {
        class EVENTSSHARED_EXPORT ScanedDirectoriesSettingsChangedEvent : public QEvent {
            public:
                ScanedDirectoriesSettingsChangedEvent( QStringList addedDirectories, QStringList removedDirectories )
                    : QEvent( ScanedDirectoriesSettingsChanged ) {
                    _addedDirectories = addedDirectories;
                    _removedDirectories = removedDirectories;
                }

                QStringList addedDirectories() {
                    return _addedDirectories;
                }
                QStringList removedDirectories() {
                    return _removedDirectories;
                }

            private:
                QStringList _addedDirectories;
                QStringList _removedDirectories;
        };
    }
}
#endif // SCANEDDIRECTORIESSETTINGSCHANGEDEVENT_H
