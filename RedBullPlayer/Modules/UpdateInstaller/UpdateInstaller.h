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
#ifndef UPDATEINSTALLER_H
#define UPDATEINSTALLER_H

#include <QObject>
#include <QThread>

#include "../../UpdateServices/PlayerVersion.h"
#include "../../PlayerShell/Shell.h"
#include "../../Widgets/UpdateProgressWidget.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace UpdateInstaller {
            class UpdateInstaller : public QThread {
                    Q_OBJECT
                public:
                    UpdateInstaller( RedBullPlayer::Updater::PlayerVersion* currentVersion,
                                     QString updateFileName,
                                     RedBullPlayer::PlayerShell::Shell* shell,
                                     RedBullPlayer::Widgets::UpdateProgressWidget* progressWidget,
                                     QObject* parent );


                    RedBullPlayer::Updater::PlayerVersion* currentVersion() const {
                        return _currentVersion;
                    }

                    QString updateFileName() const {
                        return _updateFileName;
                    }


                protected:
                    virtual void run();

                protected slots:
                    void startWork();

                signals:
                    void doStepSignal( QString msg );
                    void setupUpdaterFinished();
                    void setupUpdaterStarted();
                    void setupUpdaterFailed( QString msg );
                    void updaterStarted();
                    void updaterFinished();
                    void updaterFailed( QString msg );

                private: // methods
                    void setupUpdaterApplication();
                    void executeUpdate();

                private: // Data
                    QString _installerFileName;
                    QString _updateFileName;
                    RedBullPlayer::Updater::PlayerVersion* _currentVersion;
                    RedBullPlayer::Updater::PlayerVersion* _targetVersion;
                    static const QString _updateApplicationName;
                    static const QString _packagedVersionFileName;
                    static const QString _extracedVersionFileName;
            };
        }
    }
}
#endif // UPDATEINSTALLER_H
