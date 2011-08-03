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
#ifndef UPDATEINSTALLERMODULE_H
#define UPDATEINSTALLERMODULE_H

//QT Includes
#include <QObject>
#include <QEvent>
#include <QHash>
#include <QPair>
#include <QUrl>
#include <QTimer>

// Project Includes
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/UpdateInstaller/IUpdateInstaller.h"
#include "../../Widgets/UpdateProgressWidget.h"
#include "../../Events/Update/UpdateAvailableForInstallEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace UpdateInstaller {
            class UpdateInstallerModule : public IUpdateInstaller {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::UpdateInstaller::IUpdateInstaller )

                public:
                    UpdateInstallerModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "UpdateInstaller";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                    virtual bool updateAvailable() const;
                    virtual bool installUpdate();
                protected:
                    bool event( QEvent * e );

                protected slots:
                    void threadFinished();
                    void timeShot();
                    void setupUpdaterStarted();
                    void setupUpdaterFinished();
                    void setupUpdaterFailed( QString msg );
                    void updaterStarted();
                    void updaterFinished();
                    void updaterFailed( QString msg );

                signals:
                    void installerFinished();


                private: //data
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    QTimer *_test_timer;
                    static const QString _updateAvailableMarkerFileName;
                    RedBullPlayer::Widgets::UpdateProgressWidget *_updateProgress;

                private: //function


                    // EventHandlers
                    void onUpdateAvailableForInstall( RedBullPlayer::Events::UpdateAvailableForInstallEvent *e );


            };
        }
    }
}

#endif // UPDATEINSTALLERMODULE_H
