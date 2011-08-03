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
#ifndef FAKEMODULE1_H
#define FAKEMODULE1_H

#include <QObject>
#include <QDomElement>
#include <QtPlugin>


#include "Plugin.h"
#include "Shell.h"

// #include "IFakeModule.h"

class IFakeModule: public RedBullPlayer::Container::Plugin {
        Q_OBJECT
    public:
        virtual bool setupCalled() const = 0;
        virtual bool setupModuleConnectionsCalled() const = 0;
        virtual bool cleanUpCalled() const = 0;
        virtual RedBullPlayer::PlayerShell::Shell* shell() const = 0;
        virtual QDomElement config() = 0;
};

Q_DECLARE_INTERFACE( IFakeModule, "at.mii.redbullplayer.modules.fakemodule/1.0" )


class FakeModule1 : public IFakeModule {
        Q_OBJECT
        Q_INTERFACES( IFakeModule )
    public:
        FakeModule1() {
            _setupCalled = false;
            _setupModuleConnectionsCalled = false;
            _cleanUpCalled = false;
        }
        virtual void Setup( QObject* shellObj, QDomElement config ) {
            _shell = qobject_cast<RedBullPlayer::PlayerShell::Shell*>( shellObj );
            _setupCalled = true;
            _config = config;
        }
        virtual void SetupModuleConnections() {
            _setupModuleConnectionsCalled = true;
        }
        virtual void cleanUp() {
            _cleanUpCalled = true;
        }
        virtual QString name() const {
            return "FakeModule1";
        }
        virtual QString version() const {
            return "1.0.0";
        }

        bool setupCalled() const {
            return _setupCalled;
        }

        bool setupModuleConnectionsCalled() const {
            return _setupModuleConnectionsCalled;
        }

        bool cleanUpCalled() const {
            return _cleanUpCalled;
        }

        RedBullPlayer::PlayerShell::Shell* shell() const {
            return _shell;
        }

        QDomElement config() {
            return _config;
        }
    private:
        RedBullPlayer::PlayerShell::Shell* _shell;
        bool _setupCalled;
        bool _setupModuleConnectionsCalled;
        bool _cleanUpCalled;
        QDomElement _config;
};

Q_EXPORT_PLUGIN2( FakeModule1, FakeModule1 )

#endif // FAKEMODULE1_H
