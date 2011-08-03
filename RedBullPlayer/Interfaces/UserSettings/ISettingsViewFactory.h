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
#ifndef ISETTINGSVIEWFACTORY_H
#define ISETTINGSVIEWFACTORY_H

#include <QObject>
#include <QString>
#include <QList>
#include <QWidget>

#include "../StyleFactory/IStyleFactory.h"
#include "ISettingsSectionView.h"
#include "ISetupSectionObject.h"
#include "ISetupSubSectionObject.h"
#include "ISetupSettingObject.h"
#include "ISettingsSubSectionView.h"
#include "ISettingsSettingView.h"
#include "ISettingsBooleanSettingView.h"
#include "ISettingsScanDirectorySettingView.h"
#include "ISettingsAddDirectorySettingView.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class ISettingsViewFactory : public QObject  {
                    Q_OBJECT

                public:
                    ISettingsViewFactory( QObject* parent = 0 ) : QObject( parent ) { }
                    virtual ~ISettingsViewFactory() {}
                    virtual QWidget* getViewsParent() = 0;
                    virtual void setStyleFactory( RedBullPlayer::Modules::StyleFactory::IStyleFactory *styleFactory ) = 0;
                    virtual ISettingsSectionView *createSectionView( ISetupSectionObject *section ) = 0;
                    virtual ISettingsSubSectionView *createSubSectionView( ISetupSubSectionObject *subSection ) = 0;
                    virtual ISettingsSettingView *createSettingView( ISetupSettingObject *setting ) = 0;
                    virtual ISettingsScanDirectorySettingView *createScanDirectorySettingView() = 0;
                    virtual ISettingsAddDirectorySettingView *createAddDirectorySettingView() = 0;
            };
        }
    }
}


#endif // ISETTINGSVIEWFACTORY_H
