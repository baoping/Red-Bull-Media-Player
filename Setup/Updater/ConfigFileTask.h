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
#ifndef CONFIGFILEEXTRACTTASK_H
#define CONFIGFILEEXTRACTTASK_H

#include "UpdateTask.h"

#include <QObject>
#include <QString>
#include <QSettings>

namespace RedBullPlayer {
    namespace Updater {
        class ConfigFileTask : public UpdateTask {
            public:
                ConfigFileTask(
#if defined(Q_OS_MAC)
                    QString oldAppBudleName,
#else
                    QString updateFileName,
#endif
                    QString taskName = "Configuration File Update Task",
                    QString configFileName = "config.ini" );
                virtual bool doWork();
                virtual void updateSettings() = 0;
            protected:
                void extractConfigFile();
                void repackageConfigFile();


            protected:
                QString _configFileName;
#if defined(Q_OS_MAC)
                QString _oldAppBudleName;
#else
                QString _updateFileName;
#endif
                QString _tmpConfigFileName;
                QSettings* _playerSettings;
        };

    }
}
#endif // CONFIGFILEEXTRACTTASK_H
