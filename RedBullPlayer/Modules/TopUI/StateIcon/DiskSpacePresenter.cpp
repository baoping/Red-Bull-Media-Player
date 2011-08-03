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
#include "DiskSpacePresenter.h"

#include "../../../ToolLib/SimpleLogger.h"
#include "../../../Container/Constants.h"

using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

DiskSpacePresenter::DiskSpacePresenter( StatusDisplayButton* view, DiskSpaceModel* model, QObject *parent ) :
    StateIconPresenter( view, model, parent ) {
    view->setEnabled( true );
    connect( model, SIGNAL( freeDiskSpaceChanged( quint64, quint64, quint64 ) ), this, SLOT( freeDiskSpaceChanged( quint64, quint64, quint64 ) ) );
    model->doCheck();
}

void DiskSpacePresenter::freeDiskSpaceChanged( quint64 freeBytes, quint64 usedBytes, quint64 maxBytes ) {
    if ( maxBytes > 0 ) {
        double p = ( double )( ( usedBytes * 100 ) / maxBytes );
        DiskSpaceButton* concretView = static_cast<DiskSpaceButton*>( view() );
        concretView->setPercent( ( int )p );
    }
}
