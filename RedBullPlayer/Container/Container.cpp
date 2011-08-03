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
#include "Container.h"

// QT Includes
#include <QDebug>
#include <QPluginLoader>
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QApplication>
#include <QThread>

// Project Includes
#include "Plugin.h"

using namespace RedBullPlayer::Container;

Container* Container::_instance = 0;
Container::Container( QObject* parent ) : QObject( parent ) {
    _bootstrapped = false;
    QFileInfo pluginsXml( "./plugins.xml" );

    if ( pluginsXml.exists() ) {
        QFile f( pluginsXml.absoluteFilePath() );
        _modulesXml.setContent( &f );
    }
}

Container* Container::instance() {
    if ( Container::_instance == 0 ) {
        _instance = new Container( 0 );
    }

    return _instance;
}

void Container::setShellObject( QObject* shellObj ) {
    Q_ASSERT( shellObj != 0 );
    _shellObj = shellObj;
}

void Container::loadModuleXML( QFileInfo pluginsXml ) {
    if ( pluginsXml.exists() ) {
        QFile f( pluginsXml.absoluteFilePath() );
        _modulesXml.setContent( &f );
    }
}

void Container::BootstrapModules() {
    if( _bootstrapped ) {
        qWarning() << "Container already botsrapped";
        return;
    }

    emit startLoading( _modulesXml.documentElement().elementsByTagName( "Module" ).count() );
    QCoreApplication::processEvents();
    BootstrapModules_private( _modulesXml );
    setupAllModuleConnections();
    bool worked = QObject::connect( QApplication::instance(), SIGNAL( aboutToQuit() ),
                                    this, SLOT( applicationAboutToQuit() ) );

    if ( ! worked ) {
        qWarning() << "Container: Could not connect aboutToQuit to applicationAboutToQuit";
    }

    _bootstrapped = true;
}

void Container::BootstrapModules_private( QDomNode node ) {
    QDomNode currNode = node.firstChild();

    while ( !currNode.isNull() ) {
        if ( currNode.isElement() ) {
            QDomElement elem = currNode.toElement();

            if ( elem.tagName() == "Module" ) {
                QString library = getLibraryName( elem.attribute( "library" ) );
                QDomElement config = elem.firstChildElement( "ModuleConfig" );
                loadModule( library, config );
                BootstrapModules_private( elem );
            }
        }

        currNode = currNode.nextSibling();
    }
}

void Container::loadModule( QString fileName, QDomElement config ) {
    emit loadingPlugin( QFileInfo( fileName ) .baseName() );
    QCoreApplication::processEvents();
    QPluginLoader pl( this );
    pl.setFileName( fileName );

    if ( pl.load() ) {
        QObject* o = pl.instance();
        Plugin* nt = qobject_cast<Plugin*>( o );

        if ( nt ) {
            qDebug() << QString( "Loaded %1" ).arg( nt->name() );
            registerModule( nt, config );
        } else {
            qWarning() << QString( "Loading plugin failed: className %1" ).arg( nt->metaObject()->className() );
            qFatal( QString( "Loading plugin failed: className %1" ).arg( nt->metaObject()->className() ).toLatin1() );
        }
    } else {
        qWarning() << QString( "Loading %1 failed: %2" ).arg( fileName ).arg( pl.errorString() );
        qFatal(  QString( "Loading %1 failed: %2" ).arg( fileName ).arg( pl.errorString() ).toLatin1() );
    }
}

void Container::registerModule( Plugin* plugin, QDomElement config ) {
    plugin->Setup( _shellObj, config );
    _modules.append( plugin );
}

void Container::setupAllModuleConnections() {
    Plugin* p;
    foreach( p, _modules ) {
        emit initializingPlugin( p->name() );
        QCoreApplication::processEvents();
        p->SetupModuleConnections();
    }
}

void Container::cleanUpAllModules() {
    Plugin* p;
    foreach( p, _modules ) {
        p->cleanUp();
    }
}


Plugin* Container::findModule( QString name, QString instanceName, QString version ) {
    QList<Plugin*>* list = findAll( name, instanceName, version );

    if ( list->isEmpty() ) {
        return 0;
    } else {
        Plugin *ret = list->at( 0 );
        delete list;
        return ret;
    }
}

QList<Plugin*>* Container::findAll( QString name, QString instanceName, QString version ) {
    Q_ASSERT( ! name.isNull() );
    Q_ASSERT( ! name.isEmpty() );
    QList<Plugin*>* retval = new QList<Plugin*>();
    Plugin* p;
    foreach( p, _modules ) {
        bool ok = true;
        ok = p->name() == name;

        if ( ok && ( ! version.isNull() && ! version.isEmpty() ) ) {
            ok = p->version() == version;
        }

        if ( ok && ( ! instanceName.isNull() && ! instanceName.isEmpty() ) ) {
            ok = p->instanceName() == instanceName;
        }

        if ( ok ) {
            retval->append( p );
        }
    }
    return retval;
}

QString Container::getLibraryName( QString platformIndependentName ) {
    QString prefix = "";
    QString sufix = platformIndependentName;
    int lastIndex = platformIndependentName.lastIndexOf( "/" );

    if( lastIndex >= 0 ) {
        prefix = platformIndependentName.left( lastIndex + 1 );
        sufix = platformIndependentName.right( platformIndependentName.length() - lastIndex - 1 );
    }

#if defined(Q_OS_WIN)
    return platformIndependentName + ".dll";
#elif defined(Q_OS_MAC)
    return prefix + "lib" + sufix + ".dylib";
#else //Linux
    return prefix + "lib" + sufix + ".so";
#endif
}

void Container::addModule( Plugin *newModule ) {
    _modules.append( newModule );
}

void Container::applicationAboutToQuit() {
    cleanUpAllModules();
    qWarning() << Q_FUNC_INFO;
    QApplication::processEvents();
}
