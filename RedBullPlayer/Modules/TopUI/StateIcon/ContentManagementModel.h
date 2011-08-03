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
#ifndef CONTENTMANAGEMENTMODEL_H
#define CONTENTMANAGEMENTMODEL_H

#include "StateIconModel.h"
#include "../../../Events/ContentManagement/AvailableContentCountChangedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TopUI {
            class ContentManagementModel : public StateIconModel {
                    Q_OBJECT
                public:
                    ContentManagementModel( RedBullPlayer::Events::HoverBoxChangeVisibilityEvent::HoverBoxDockPosition hoverBoxPosition, QObject *parent = 0 );

                protected:
                    bool event( QEvent *event );
                    void onAvailableContentCountChanged( RedBullPlayer::Events::AvailableContentCountChangedEvent *e );

                signals:
                    void availableContentCountChanged( int count );

            };
        }
    }
}

#endif // CONTENTMANAGEMENTMODEL_H
