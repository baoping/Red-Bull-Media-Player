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
#ifndef CONTENTMANAGEMENTPRESENTER_H
#define CONTENTMANAGEMENTPRESENTER_H

#include <QObject>

#include "StateIconPresenter.h"
#include "ContentManagementModel.h"
#include "../../../Widgets/StatusCountDisplayButton.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace TopUI {
            class ContentManagementPresenter : public StateIconPresenter {
                    Q_OBJECT
                public:
                    ContentManagementPresenter( RedBullPlayer::Widgets::StatusDisplayButton *view, ContentManagementModel *model, QObject *parent = 0 );


                protected slots:
                    void availableContentCountChaned( int count );

                private:
                    RedBullPlayer::Widgets::StatusCountDisplayButton *_statusButton;
                    ContentManagementModel *_model;
            };
        }
    }
}

#endif // CONTENTMANAGEMENTPRESENTER_H
