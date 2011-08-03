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
#ifndef WIN32INSTALLERTASK_H
#define WIN32INSTALLERTASK_H

#include <QObject>
#include <QString>

#include "UpdateTask.h"

namespace RedBullPlayer {
    namespace Updater {

        class Win32InstallerTask : public RedBullPlayer::Updater::UpdateTask {
            public:
                Win32InstallerTask( QString updatePackagePath, QString originalApplicationPath );
                virtual bool doWork();

            private:
                QString _updatePackagePath;
                QString _originalApplicationPath;


        };
    }
}
#endif // WIN32INSTALLERTASK_H
