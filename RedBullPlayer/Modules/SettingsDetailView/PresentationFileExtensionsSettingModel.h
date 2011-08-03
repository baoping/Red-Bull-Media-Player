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
#ifndef PRESENTATIONFILEEXTENSIONSSETTINGMODEL_H
#define PRESENTATIONFILEEXTENSIONSSETTINGMODEL_H

#include "../../Interfaces/UserSettings/ISettingsModel.h"
#include "../../Interfaces/UserSettings/ISettingsFileExtensionsSettingView.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class PresentationFileExtensionsSettingModel : public ISettingsModel {
                    Q_OBJECT
                public:
                    PresentationFileExtensionsSettingModel( QObject *parent, ISettingsFileExtensionsSettingView *view );

                    virtual void saveData();

                private:
                    ISettingsFileExtensionsSettingView *_view;
                    QString _originalSetting;
                    QString _currentSetting;
            };
        }
    }
}

#endif // PRESENTATIONFILEEXTENSIONSSETTINGMODEL_H
