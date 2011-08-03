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
#include "ContainerTest.h"

// QT includes
#include <QFileInfo>
#include <QList>

// Project includes
#include "TestContainer.h"
#include "Container.h"
#include "Plugin.h"
#include "Shell.h"

#include "IFakeModule.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::PlayerShell;

void ContainerTest::checkSingletonInstance() {
    Container* c1 = Container::instance();
    Container* c2 = Container::instance();
    QVERIFY( c1 == c2 );
}

void ContainerTest::loadModuleXml()  {
    Container* c = new TestContainer();
    QFileInfo xmlFile( "PluginsTest.xml" );
    c->loadModuleXML( xmlFile );
}

void ContainerTest::bootsrapModules() {
    Container* c = new TestContainer();
    QFileInfo xmlFile( "PluginsTest.xml" );
    c->loadModuleXML( xmlFile );
    Shell* s = new Shell();
    c->setShellObject( s );
    c->BootstrapModules();
    QVERIFY( c->modules().count() == 3 );
}

void ContainerTest::findModule() {
    Container* c = new TestContainer();
    QFileInfo xmlFile( "PluginsTest.xml" );
    c->loadModuleXML( xmlFile );
    Shell* s = new Shell();
    c->setShellObject( s );
    c->BootstrapModules();
    Plugin* newsTicker = c->findModule( "NewsTicker" );
    QVERIFY( newsTicker != 0 );
    QVERIFY( newsTicker->name() == "NewsTicker" );
    newsTicker = c->findModule( "NewsTicker", QString::null, "1.0.0" );
    QVERIFY( newsTicker != 0 );
    QVERIFY( newsTicker->name() == "NewsTicker" );
    QVERIFY( newsTicker->version() == "1.0.0" );
    newsTicker = c->findModule( "NewsTicker", QString::null, "1.0.1" );
    QVERIFY( newsTicker == 0 );
}

void ContainerTest::should_call_setup_setupo_connections_on_load() {
    // QVERIFY2( false, "Not IMplemented!" );
    Container* c = new TestContainer();
    QFileInfo xmlFile( "PluginsTest.xml" );
    c->loadModuleXML( xmlFile );
    Shell* s = new Shell();
    c->setShellObject( s );
    c->BootstrapModules();
    Plugin* fakeModule1 = c->findModule( "FakeModule1" );
    QVERIFY( fakeModule1 != 0 );
    QVERIFY( fakeModule1->name() == "FakeModule1" );
    IFakeModule* fm = qobject_cast<IFakeModule*>( fakeModule1 );
    QVERIFY2( fm->setupCalled(), "Setup must me called" );
    QVERIFY2( fm->setupModuleConnectionsCalled(), "SetupModuleConnections must me called" );
}

void ContainerTest::should_call_cleanup() {
    Container* c = new TestContainer();
    QFileInfo xmlFile( "PluginsTest.xml" );
    c->loadModuleXML( xmlFile );
    Shell* s = new Shell();
    c->setShellObject( s );
    c->BootstrapModules();
    Plugin* fakeModule1 = c->findModule( "FakeModule1" );
    QVERIFY( fakeModule1 != 0 );
    QVERIFY( fakeModule1->name() == "FakeModule1" );
    IFakeModule* fm = qobject_cast<IFakeModule*>( fakeModule1 );
    QVERIFY2( fm->setupCalled(), "Setup must me called" );
    QVERIFY2( fm->setupModuleConnectionsCalled(), "SetupModuleConnections must me called" );
    c->cleanUpAllModules();
    QVERIFY2( fm->cleanUpCalled(), "cleanUpAllModules must call cleanup on modules" );
}

