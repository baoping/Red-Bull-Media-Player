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
#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QString>
#include <QDomElement>

namespace RedBullPlayer {
    namespace Container {
        class Plugin : public QObject {
                Q_OBJECT
            public:
                Plugin();
                virtual ~Plugin() {}
                virtual void Setup( QObject* shellObj, QDomElement config ) = 0;
                virtual void SetupModuleConnections() {}
                virtual void cleanUp() {}
                virtual QString name() const = 0;
                virtual QString version() const = 0;
                void setInstanceName( QString name );
                QString instanceName() const {
                    return _instanceName;
                }

            protected:
                QString _instanceName;
        };
    }
}
#endif // IPLUGIN_H
