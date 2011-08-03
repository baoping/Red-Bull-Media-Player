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
#ifndef COLLECTSTATISTICSSETTINGMODEL_H
#define COLLECTSTATISTICSSETTINGMODEL_H

#include "../../Interfaces/UserSettings/ISettingsModel.h"
#include "../../Interfaces/UserSettings/ISettingsBooleanSettingView.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class CollectStatisticsSettingModel : public ISettingsModel {
                    Q_OBJECT
                public:
                    CollectStatisticsSettingModel( QObject *parent, ISettingsBooleanSettingView *view,
                                                   RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *mediaLibrary );


                    virtual void saveData();

                protected slots:
                    void valueChanged( bool value );

                private:
                    ISettingsBooleanSettingView *_view;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *_mediaLibrary;
                    bool _currentSettingValue;
                    bool _originalValue;
            };
        }
    }
}

#endif // COLLECTSTATISTICSSETTINGMODEL_H
