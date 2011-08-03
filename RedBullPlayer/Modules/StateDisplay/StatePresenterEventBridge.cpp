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
#include "StatePresenterEventBridge.h"

#include <QDebug>

#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/StateDisplay/RequestHoverBoxChangeVisibilityEvent.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

StatePresenterEventBridge::StatePresenterEventBridge( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, IStatePresenter* statePresenter, QObject *parent ) :  QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "TaskPresenterEventBridge: Create new EventBridge." ) );
    _statePresenter = statePresenter;
    _purpose = purpose;
    Aggregator::instance()->registerListener( HoverBoxChangeVisibility, this );
    Aggregator::instance()->registerListener( ShellWidgetMoved, this );
}

bool StatePresenterEventBridge::event( QEvent* event ) {
    if ( event->type() == HoverBoxChangeVisibility ) {
        onHoverBoxChangeVisibility( static_cast<HoverBoxChangeVisibilityEvent*>( event ) );
    }

    if( event->type() == ShellWidgetMoved ) {
        onShellWidgetMoved( static_cast<ShellWidgetMovedEvent*>( event ) );
    }

    return false;
}

void StatePresenterEventBridge::onHoverBoxChangeVisibility( RedBullPlayer::Events::HoverBoxChangeVisibilityEvent* e )  {
    if ( e->statusGroup() == _purpose ) {
        if ( e->visible() ) {
            _statePresenter->showView( e->xGlobalPosition(), e->yGlobalPosition() );
        } else {
            _statePresenter->hideView();
        }
    }
}

void StatePresenterEventBridge::onShellWidgetMoved( ShellWidgetMovedEvent *e ) {
    if( _statePresenter->isViewVisible() ) {
        RequestHoverBoxChangeVisibilityEvent ev( _purpose, true );
        Aggregator::instance()->sendEvent( &ev );
    }
}
