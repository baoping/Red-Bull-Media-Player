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
#ifndef SETTINGSVIEWFACTORY_H
#define SETTINGSVIEWFACTORY_H

#include "../../Interfaces/UserSettings/ISettingsViewFactory.h"
#include "../../Interfaces/UserSettings/ISetupFileParser.h"
#include "../../Interfaces/UserSettings/ISetupSectionObject.h"
#include "../../Interfaces/UserSettings/ISetupSubSectionObject.h"
#include "../../Interfaces/UserSettings/ISetupSettingObject.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class SettingsViewFactory : public ISettingsViewFactory {
                    Q_OBJECT

                public:
                    SettingsViewFactory( QObject* parent, QWidget* viewsParent );

                    virtual QWidget* getViewsParent();
                    virtual void setStyleFactory( RedBullPlayer::Modules::StyleFactory::IStyleFactory *styleFactory );
                    virtual ISettingsSectionView *createSectionView( ISetupSectionObject *section );
                    virtual ISettingsSubSectionView *createSubSectionView( ISetupSubSectionObject *subSection );
                    virtual ISettingsSettingView *createSettingView( ISetupSettingObject *setting );
                    virtual ISettingsScanDirectorySettingView *createScanDirectorySettingView();
                    virtual ISettingsAddDirectorySettingView *createAddDirectorySettingView();

                    RedBullPlayer::Modules::SettingsDetailView::ISettingsSettingView *
                    createFileExtensionSettingView( RedBullPlayer::Modules::SettingsDetailView::ISetupSettingObject *setting );

                    RedBullPlayer::Modules::SettingsDetailView::ISettingsSettingView *
                    createBooleanSettingView( RedBullPlayer::Modules::SettingsDetailView::ISetupSettingObject *setting );

                    RedBullPlayer::Modules::SettingsDetailView::ISettingsSettingView *
                    createIntegerSettingView( RedBullPlayer::Modules::SettingsDetailView::ISetupSettingObject *setting );

                protected:
                    void resolveSectionStyle( RedBullPlayer::Modules::SettingsDetailView::ISettingsSectionView *sectionView );
                    void resolveSubSectionStyle( RedBullPlayer::Modules::SettingsDetailView::ISettingsSubSectionView *subSectionView );
                    void resolveSettingStyle( RedBullPlayer::Modules::SettingsDetailView::ISettingsSettingView *settingView,
                                              QString labelStyleName = "Settings_Label",
                                              QString textBoxStyleName = "SettingsTextBox_Text" );

                private:
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
                    RedBullPlayer::Modules::SettingsDetailView::ISetupFileParser *_setupFileParser;
                    QWidget *_viewsParent;
            };
        }
    }
}

#endif // SETTINGSVIEWFACTORY_H
