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
#ifndef USERSETTINGSMODULE_H
#define USERSETTINGSMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/UserSettings/IUserSettings.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../PlayerShell/Shell.h"
#include "../../Widgets/RedBullDialog.h"
#include "../../Events/UserSettings/ShowUserSettingsDialogEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace UserSettings {
            class UserSettingsModule : public IUserSettings {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::UserSettings::IUserSettings )

                public:
                    UserSettingsModule();
                    virtual ~UserSettingsModule();

                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "UserSettings";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                protected:
                    bool event( QEvent* );

                    void onShowUserSettingsDialog( RedBullPlayer::Events::ShowUserSettingsDialogEvent* e );

                private: //data
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::Widgets::RedBullDialog* _dialog;

                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory* _styleFactory;

                private: //functions
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* resolveMediaLibrary();
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory* resolveStyleFactory();
                    QString getFileContent( QString fileName );

                private slots:
                    void showDialog();
                    void dialogClosed();
            };
        }
    }
}

#endif // USERSETTINGSMODULE_H
