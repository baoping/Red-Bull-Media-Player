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
#ifndef SCANEDDIRECTORIESSETTINGMODEL_H
#define SCANEDDIRECTORIESSETTINGMODEL_H

#include <QHash>
#include <QList>
#include <QStringList>

#include "../../Interfaces/UserSettings/ISettingsModel.h"
#include "../../Interfaces/UserSettings/ISettingsScanDirectorySettingView.h"
#include "StringListComparer.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class ScanedDirectoriesSettingModel : public ISettingsModel {
                    Q_OBJECT
                public:
                    ScanedDirectoriesSettingModel( QObject *parent );
                    ~ScanedDirectoriesSettingModel();

                    virtual void saveData();

                    QList<QString> getScanedDirectories();
                    int addView( QUuid guid, ISettingsScanDirectorySettingView *view, QString absoluteFilePath );
                    ISettingsScanDirectorySettingView *removeView( QUuid guid );

                protected:
                    QString convertToDisplayFilePath( QString absoluteFilePath );
                    QString resolveRootPath();

                private:
                    QHash<QUuid, ISettingsScanDirectorySettingView *> _views;
                    QList<QUuid> _keyOrderList;
                    QString _rootPathReplacementText;
                    QString _rootPath;
                    QStringList _originalDirectories;
            };
        }
    }
}

#endif // SCANEDDIRECTORIESSETTINGMODEL_H
