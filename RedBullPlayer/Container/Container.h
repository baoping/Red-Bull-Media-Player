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
#ifndef CONTAINER_H
#define CONTAINER_H

// QT Includes
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QObject>
#include <QLibrary>
#include <QFileInfo>
#include <QList>

// Project includes
#include "Plugin.h"

namespace RedBullPlayer {
    namespace Container {
        class Container : public QObject {
                Q_OBJECT
            public:
                static Container* instance();

                void setShellObject( QObject* shellObj );
                void loadModuleXML( QFileInfo xmlFile );
                void BootstrapModules();
                Plugin* findModule( QString name, QString instanceName = QString::null, QString version = QString::null );
                QList<Plugin*>* findAll( QString name, QString instanceName = QString::null, QString version = QString::null );
                const QList<Plugin*>& modules() const {
                    return _modules;
                }
                void addModule( Plugin *newModule );
                void registerModule( Plugin* plugin, QDomElement pluginConfig );
                void setupAllModuleConnections();
                void cleanUpAllModules();

            protected:
                static Container* _instance;

                Container( QObject* parent = 0 );

                void loadModule( QString libraryFileName, QDomElement config );
                void BootstrapModules_private( QDomNode node );
                QString getLibraryName( QString platformIndependentName );

                QObject* _shellObj;
                QDomDocument _modulesXml;
                QList<Plugin*> _modules;
                bool _bootstrapped;

            private slots:
                void applicationAboutToQuit();

            signals:
                void startLoading( int pluginCount );
                void loadingPlugin( QString pluginName );
                void initializingPlugin( QString pluginName );
        };
    }
}
#endif // CONTAINER_H
