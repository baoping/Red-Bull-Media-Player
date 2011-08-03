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
#include "StyleFactoryModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QXmlStreamReader>
#include <QFile>

#include "../../ToolLib/ConfigManager.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"

#include "TextStyle.h"
#include "styleparser.h"

using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;

StyleFactoryModule::StyleFactoryModule() : IStyleFactory() {
}

void StyleFactoryModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
    bootstrap();
}

bool StyleFactoryModule::event ( QEvent * e ) {
    return false;
}

void StyleFactoryModule::bootstrap() {
    StyleParser parser( this );
    QFileInfo configFile( ConfigManager::instance()->styleConfigurationFileName() );
    _styles = parser.parseConfigFile( configFile );
}

ITextStyle* StyleFactoryModule::getTextStyle( QString name, bool *ok ) {
    *ok = false;

    if ( _styles->contains( name ) ) {
        *ok = true;
        return _styles->value( name );
    }

    return new TextStyle(); //return default text style
}

Q_EXPORT_PLUGIN2( StyleFactoryModule, RedBullPlayer::Modules::StyleFactory::StyleFactoryModule );

