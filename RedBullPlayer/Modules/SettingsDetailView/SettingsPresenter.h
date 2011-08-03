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
#ifndef SETTINGSPRESENTER_H
#define SETTINGSPRESENTER_H

#include "../../Interfaces/UserSettings/ISettingsPresenter.h"
#include "../../Interfaces/UserSettings/ISetupFileParser.h"
#include "../../Interfaces/UserSettings/ISetupSectionObject.h"
#include "../../Interfaces/UserSettings/ISetupSubSectionObject.h"
#include "../../Interfaces/UserSettings/ISetupSettingObject.h"
#include "../../Interfaces/UserSettings/ISettingsViewFactory.h"
#include "../../Interfaces/UserSettings/ISettingsModel.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class SettingsPresenter : public ISettingsPresenter {
                    Q_OBJECT
                public:
                    SettingsPresenter( QObject *parent, ISettingsViewFactory *viewFactory );

                    virtual QList<ISettingsSectionView *> bootstrapViews( QString pathToSetupFile );
                    virtual void saveDataOnClose();
                    virtual void setMediaLibrary( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *mediaLibrary );

                protected:
                    ISettingsSectionView *bootstrapSectionView( ISetupSectionObject *section,
                            ISetupFileParser *fileParser );
                    void populateScanedDirectoriesSection( ISettingsSectionView *sectionView );
                    ISettingsSubSectionView *bootstrapSubSectionView( ISetupSubSectionObject *subSection,
                            ISetupFileParser *fileParser );
                    ISettingsSettingView *bootstrapSettingView( ISetupSettingObject *setting );
                    void bootstrapModelToView( ISettingsSettingView *view, ISetupSettingObject *setting );

                private:
                    ISetupFileParser *_setupFileParser;
                    ISettingsViewFactory *_viewFactory;
                    QList<ISettingsModel *> _currentModels;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *_mediaLibrary;
            };
        }
    }
}

#endif // SETTINGSPRESENTER_H
