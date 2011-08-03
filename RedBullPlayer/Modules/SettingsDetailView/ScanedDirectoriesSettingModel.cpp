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
#include "ScanedDirectoriesSettingModel.h"

#include <QDir>

#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/FileSystem/USBStickUtil.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"
#include "../../Events/UserSettings/ScanedDirectoriesSettingsChangedEvent.h"
#include "../../EventAggregator/Aggregator.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

ScanedDirectoriesSettingModel::ScanedDirectoriesSettingModel( QObject *parent ) : ISettingsModel( parent ) {
    _rootPathReplacementText = ConfigManager::instance()->rootPathReplacementText();
    _rootPath = resolveRootPath();
}

ScanedDirectoriesSettingModel::~ScanedDirectoriesSettingModel() {
    _views.clear();
}

void ScanedDirectoriesSettingModel::saveData() {
    QList<QString> directories;
    QStringList newDirectories;
    QStringList addedItems;
    QStringList removedItems;
    StringListComparer *comparer = new StringListComparer();
    foreach( QUuid guid, _keyOrderList )
    directories.append( _views.value( guid )->getValue() );
    ConfigManager::instance()->setMediaDirectories( directories );
    newDirectories.append( directories );
    removedItems = comparer->getRemovedItems( _originalDirectories, newDirectories );
    addedItems = comparer->getAddedItems( _originalDirectories, newDirectories );

    if( addedItems.size() > 0 || removedItems.size() > 0 ) {
        ScanedDirectoriesSettingsChangedEvent ev( addedItems, removedItems );
        Aggregator::instance()->sendEvent( &ev );
    }

    _keyOrderList.clear();
    _views.clear();
}

int ScanedDirectoriesSettingModel::addView( QUuid guid, ISettingsScanDirectorySettingView *view, QString absoluteFilePath ) {
    QString displayText = convertToDisplayFilePath( absoluteFilePath );
    view->setValue( displayText );
    _views.insert( guid, view );
    _keyOrderList.append( guid );
    return _keyOrderList.count() - 1;
}

ISettingsScanDirectorySettingView *ScanedDirectoriesSettingModel::removeView( QUuid guid ) {
    ISettingsScanDirectorySettingView *viewToRemove = _views.take( guid );
    int indexRemove = _keyOrderList.indexOf( guid );

    if( indexRemove >= 0 )
        _keyOrderList.removeAt( indexRemove );

    return viewToRemove;
}

QString ScanedDirectoriesSettingModel::convertToDisplayFilePath( QString absoluteFilePath ) {
    return absoluteFilePath;
}

QString ScanedDirectoriesSettingModel::resolveRootPath() {
    QString drivePath = "";
    return drivePath;
}

QList<QString> ScanedDirectoriesSettingModel::getScanedDirectories() {
    QList<QString> scanedDirectories = ConfigManager::instance()->mediaDirectories();
    _originalDirectories.clear();
    _originalDirectories.append( scanedDirectories );
    return scanedDirectories;
}
