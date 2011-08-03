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
#include "ScanedDirectoriesSettingController.h"

#include <QFileDialog>
#include <QApplication>
#include <QDir>

#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Tools;

ScanedDirectoriesSettingController::ScanedDirectoriesSettingController( ScanedDirectoriesSettingModel *model,
        ISettingsViewFactory *viewFactory,
        ISettingsSubSectionView *subSectionView ) {
    _model = model;
    _viewFactory = viewFactory;
    _subSectionView = subSectionView;
}

ISettingsSubSectionView *ScanedDirectoriesSettingController::buildScanedDirectorySettingsView() {
    QList<QString> scanedDirectories = _model->getScanedDirectories();
    ISettingsScanDirectorySettingView *view;

    for( int i = 0; i < scanedDirectories.count(); i++ ) {
        QString directory = scanedDirectories.at( i );
        view = _viewFactory->createScanDirectorySettingView();
        _model->addView( view->guid(), view, directory );
        connect( view, SIGNAL( removeClicked( QUuid ) ), this, SLOT( removeSlot( QUuid ) ) );

        if( i == 0 )
            view->setReadOnly( true ); //Media Directory  Settings ReadOnly

        _subSectionView->addSettingView( view );
    }

    _addDirectoryView = _viewFactory->createAddDirectorySettingView();
    connect( _addDirectoryView, SIGNAL( addClicked() ), this, SLOT( addSlot() ) );
    _subSectionView->addSettingView( _addDirectoryView );
    return _subSectionView;
}

void ScanedDirectoriesSettingController::removeSlot( QUuid guid ) {
    ISettingsScanDirectorySettingView *removedView = _model->removeView( guid );
    _subSectionView->removeSettingView( removedView );
    delete removedView;
}

void ScanedDirectoriesSettingController::addSlot() {
    QString dirPath = "";

    if ( !ConfigManager::instance()->getLastDialogDirectory().isEmpty() ) {
        dirPath = ConfigManager::instance()->getLastDialogDirectory();
    } else {
        QDir dir( ConfigManager::instance()->getMediaDirectory() );
        dir.cdUp();
        dirPath = dir.absolutePath();
    }

    QFileDialog fileDialog( QApplication::activeWindow(), "Choose directory to be scanned", dirPath );
    fileDialog.setViewMode( QFileDialog::Detail );
    fileDialog.setFileMode( QFileDialog::Directory );
    fileDialog.setOption( QFileDialog::ShowDirsOnly );

    if( fileDialog.exec() ) {
        ConfigManager::instance()->setLastDialogDirectory( fileDialog.directory().absolutePath() );
        QString newDirectoryPath = fileDialog.selectedFiles().at( 0 );
        ISettingsScanDirectorySettingView *newView = _viewFactory->createScanDirectorySettingView();
        int atIndex = _model->addView( newView->guid(), newView, newDirectoryPath );
        connect( newView, SIGNAL( removeClicked( QUuid ) ), this, SLOT( removeSlot( QUuid ) ) );
        _subSectionView->insertSettingView( atIndex, newView );
    }
}
