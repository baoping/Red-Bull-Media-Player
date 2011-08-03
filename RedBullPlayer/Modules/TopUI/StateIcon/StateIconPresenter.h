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
#ifndef STATEICONPRESENTER_H
#define STATEICONPRESENTER_H

#include <QObject>

#include "../../../Widgets/StatusDisplayButton.h"

#include "StateIconModel.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TopUI {
            class StateIconPresenter : public QObject {
                    Q_OBJECT
                public:
                    StateIconPresenter( RedBullPlayer::Widgets::StatusDisplayButton* view, StateIconModel* model, QObject *parent = 0 );

                    void disableHoverBox();
                    void requestVisibillityChanged();

                protected:
                    RedBullPlayer::Widgets::StatusDisplayButton* view() const {
                        return _view;
                    }
                    StateIconModel* model() const {
                        return _model;
                    }

                private slots:
                    void viewMouseEnter( int globalX, int globalY );
                    void viewMouseLeave( int globalX, int globalY );
                    void modelVisibilityChanged( bool visible );

                private: //data
                    RedBullPlayer::Widgets::StatusDisplayButton* _view;
                    StateIconModel* _model;
                private: //functions
                    void sendHoverBoxVisibilityChangeEvent( int globalX, int globalY, bool visible );

            };
        }
    }
}

#endif // STATEICONPRESENTER_H
