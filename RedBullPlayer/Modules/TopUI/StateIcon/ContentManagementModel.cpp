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
#include "ContentManagementModel.h"

#include "../../../EventAggregator/Aggregator.h"
#include "../../../Events/EventTypes.h"

using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

ContentManagementModel::ContentManagementModel( HoverBoxChangeVisibilityEvent::HoverBoxDockPosition hoverBoxPosition, QObject *parent )
    : StateIconModel( TaskPurpose::DOWNLOAD, hoverBoxPosition, parent ) {
    Aggregator::instance()->registerListener( AvailableContentCountChanged, this );
}

bool ContentManagementModel::event( QEvent *event ) {
    if( event->type() == AvailableContentCountChanged ) {
        onAvailableContentCountChanged( static_cast<AvailableContentCountChangedEvent *>( event ) );
    }

    return false;
}

void ContentManagementModel::onAvailableContentCountChanged( AvailableContentCountChangedEvent *e ) {
    emit availableContentCountChanged( e->count() );
}
