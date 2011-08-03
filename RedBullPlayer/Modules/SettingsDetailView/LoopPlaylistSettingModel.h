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
#ifndef LOOPPLAYLISTSETTINGMODEL_H
#define LOOPPLAYLISTSETTINGMODEL_H

#include "../../Interfaces/UserSettings/ISettingsModel.h"
#include "../../Interfaces/UserSettings/ISettingsBooleanSettingView.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class LoopPlaylistSettingModel : public ISettingsModel {
                    Q_OBJECT
                public:
                    LoopPlaylistSettingModel( QObject *parent, ISettingsBooleanSettingView *view );
                    virtual void saveData();

                protected slots:
                    void valueChanged( bool value );

                private:
                    ISettingsBooleanSettingView *_view;
                    bool _currentSettingValue;
            };
        }
    }
}

#endif // LOOPPLAYLISTSETTINGMODEL_H
