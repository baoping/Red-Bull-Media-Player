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
#ifndef PLAYERUPDATECHECKERMODULE_H
#define PLAYERUPDATECHECKERMODULE_H

//QT Includes
#include <QObject>
#include <QUuid>

// Project Includes
#include "UpdateDownloadTask.h"
#include "../../Interfaces/IPlayerUpdateChecker.h"
#include "../../PlayerShell/Shell.h"
#include "../../UpdateServices/VersionServiceClient.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"
#include "../../Events/UserSettings/UserSettingsChangedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlayerUpdateChecker {
            class PlayerUpdateCheckerModule : public IPlayerUpdateChecker {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::PlayerUpdateChecker::IPlayerUpdateChecker )

                public:
                    PlayerUpdateCheckerModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "PlayerUpdateChecker";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }
                    virtual void doCheck();

                protected:
                    bool event( QEvent * );

                    void onInternetConnectionStatusChanged( RedBullPlayer::Events::InternetConnectionStatusChangedEvent* ev );
                    void onUserSettingsChanged( RedBullPlayer::Events::UserSettingsChangedEvent *event );

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    RedBullPlayer::Updater::VersionServiceClient* _client;
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory* _styleFactory;

                    QString _updateBoxText;
                    QString _diskSpaceWarningText;
                    QString _downloadFinishedText;

                    bool _checked;

                    QUuid _updateTaskId;
                    UpdateDownloadTask* _updateTask;
                    bool _updateDownloadInProgress;
                    bool _internetAvailable;
                    bool _allowAutomaticDownload;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *_mediaLibrary;

                    RedBullPlayer::Modules::StyleFactory::IStyleFactory* resolveStyleFactory();
                    bool resolveMediaLibrary();
                    void resolveConnectivityChanged();

                private slots:
                    void versionFound( QString newPlayerVersion, QString downloadUrl, qulonglong zipSize, qulonglong unzipSize );
                    void noVersionFound();
                    void downloadProgress( QUuid taskId, int progress );
            };
        }
    }
}

#endif // PLAYERUPDATECHECKERMODULE_H
