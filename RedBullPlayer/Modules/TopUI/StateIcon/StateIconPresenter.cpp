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
#include "StateIconPresenter.h"

#include "../../../Events/StateDisplay/HoverBoxChangeVisibilityEvent.h"
#include "../../../EventAggregator/Aggregator.h"
#include "../../../Events/EventTypes.h"

using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;

StateIconPresenter::StateIconPresenter( StatusDisplayButton* view, StateIconModel* model, QObject *parent ) :
    QObject( parent ) {
    //Model
    _model = model;
    connect( _model, SIGNAL( buttonVisibilityChanged( bool ) ), this, SLOT( modelVisibilityChanged( bool ) ) );
    _view = view;
    _view->setEnabled( false );
    connect( _view, SIGNAL( mouseEnter( int, int ) ), this, SLOT( viewMouseEnter( int, int ) ) );
    connect( _view, SIGNAL( mouseLeave( int, int ) ), this, SLOT( viewMouseLeave( int, int ) ) );
}

void StateIconPresenter::disableHoverBox() {
    sendHoverBoxVisibilityChangeEvent( 0, 0, false );
}

void StateIconPresenter::viewMouseEnter( int globalX, int globalY ) {
    sendHoverBoxVisibilityChangeEvent( globalX, globalY, true );
}

void StateIconPresenter::viewMouseLeave( int globalX, int globalY ) {
    //sendHoverBoxVisibilityChangeEvent(globalX,globalY,false);
}

void StateIconPresenter::sendHoverBoxVisibilityChangeEvent( int globalX, int globalY, bool visible ) {
    int hoverBoxX = globalX;
    int hoverBoxY = globalY;

    switch( _model->hoverBoxPosition() ) {
        case HoverBoxChangeVisibilityEvent::UPPERLEFTCORNER:
            break;

        case HoverBoxChangeVisibilityEvent::UPPERRIGHTCORNER:
            hoverBoxX += _view->width();
            break;

        case HoverBoxChangeVisibilityEvent::LOWERLEFTCORNER:
            hoverBoxY += _view->height();
            break;

        case HoverBoxChangeVisibilityEvent::LOWERRIGHTCORNER:
            hoverBoxY += _view->height();
            hoverBoxX += _view->width();
            break;
    }

    HoverBoxChangeVisibilityEvent e( _model->purpose(),
                                     visible,
                                     hoverBoxX,
                                     hoverBoxY,
                                     _model->hoverBoxPosition() );
    Aggregator::instance()->sendEvent( &e );
}

void StateIconPresenter::modelVisibilityChanged( bool visible ) {
    _view->setEnabled( visible );
}

void StateIconPresenter::requestVisibillityChanged() {
    _view->requestMouseEnterSignal();
}
