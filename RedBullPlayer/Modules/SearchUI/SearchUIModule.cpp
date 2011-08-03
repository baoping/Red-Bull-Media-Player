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
#include "SearchUIModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>

// Project Includes
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

#include "SearchUICreator.h"

using namespace RedBullPlayer::Modules::SearchUI;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Tools;

SearchUIModule::SearchUIModule() : ISearchUI() {
}

void SearchUIModule::Setup( QObject* shell, QDomElement config ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_SEARCHUICREATOR, Q_FUNC_INFO );
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    _ui = new SearchUICreator( _region->widget() );

    if( _region->widget()->layout() == NULL ) {
        new QHBoxLayout( _region->widget() );
    }

    _region->widget()->layout()->setMargin( 0 );
    _region->widget()->layout()->setSpacing( 0 );
    _region->widget()->layout()->addWidget( _ui );
    Aggregator::instance()->registerListener( MainUIRegionFocusChanged, this );
    Aggregator::instance()->registerListener( CentralFocusChanged, this );
    Aggregator::instance()->registerListener( PlaylistGenerationFinished, this );
}

void SearchUIModule::SetupModuleConnections() {
    _ui->resolveStyleFactory();
    _ui->setupStyle();
}

bool SearchUIModule::event ( QEvent * e ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_SEARCHUICREATOR, Q_FUNC_INFO );

    if( e->type() ==  MainUIRegionFocusChanged ) {
        onMainUIRegionFocusChanged( static_cast<MainUIRegionFocusChangedEvent*>( e ) );
    }

    if( e->type() == CentralFocusChanged ) {
        onCentralFocusChanged( static_cast<CentralFocusChangedEvent*>( e ) );
    }

    if( e->type() == PlaylistGenerationFinished ) {
        onPlaylistGenerationFinished( static_cast<PlaylistGenerationFinishedEvent*>( e ) );
    }

    return false;
}

void SearchUIModule::onPlaylistGenerationFinished( PlaylistGenerationFinishedEvent *e ) {
    _ui->enableSearchGlobally();
}

void SearchUIModule::onCentralFocusChanged( CentralFocusChangedEvent *e ) {
    _ui->handleCentralFocusChanged( e->widgetType() );
}

void SearchUIModule::onMainUIRegionFocusChanged( MainUIRegionFocusChangedEvent *event ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_SEARCHUICREATOR, Q_FUNC_INFO );
    _ui->changeMainUIRegionFocusGraphics( event->uiRegion() );
}

Q_EXPORT_PLUGIN2( SearchUIModule, RedBullPlayer::Modules::SearchUI::SearchUIModule )
