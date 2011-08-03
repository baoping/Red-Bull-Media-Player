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
#include "SettingsPresenter.h"

#include "SetupFileParser/SettingsViewSetupFileParser.h"
#include "FullScreenSettingModel.h"
#include "LoopPlaylistSettingModel.h"
#include "ContentDownloadSettingModel.h"
#include "CollectStatisticsSettingModel.h"
#include "VideoFileExtensionsSettingModel.h"
#include "AudioFileExtensionsSettingModel.h"
#include "ImageFileExtensionsSettingModel.h"
#include "TextFileExtensionsSettingModel.h"
#include "PresentationFileExtensionsSettingModel.h"
#include "SpreadsheetFileExtensionsSettingModel.h"
#include "ScanedDirectoriesSettingModel.h"
#include "ScanedDirectoriesSettingController.h"
#include "OthersFileExtensionsSettingModel.h"
#include "PdfFileExtensionsSettingModel.h"
#include "LoopIntervalSettingModel.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Modules::MediaLibrary;

SettingsPresenter::SettingsPresenter( QObject *parent, ISettingsViewFactory *viewFactory ) : ISettingsPresenter( parent ) {
    _viewFactory = viewFactory;
    _setupFileParser = new SettingsViewSetupFileParser( this );
    _currentModels = QList<ISettingsModel *>();
}

void SettingsPresenter::setMediaLibrary( IMediaLibrary *mediaLibrary ) {
    _mediaLibrary = mediaLibrary;
}

QList<ISettingsSectionView *> SettingsPresenter::bootstrapViews( QString pathToSetupFile ) {
    QList<ISettingsSectionView *> views;

    if( _setupFileParser->parseSetupFile( pathToSetupFile ) ) {
        QList<ISetupSectionObject *> sections = _setupFileParser->getSections();
        foreach( ISetupSectionObject * section, sections )
        views.append( bootstrapSectionView( section, _setupFileParser ) );
    }

    return views;
}

ISettingsSectionView *SettingsPresenter::bootstrapSectionView( ISetupSectionObject *section, ISetupFileParser *fileParser ) {
    ISettingsSectionView *sectionView = _viewFactory->createSectionView( section );

    if( section->getId() == "scanDirectoriesSettings" ) {
        populateScanedDirectoriesSection( sectionView );
    } else {
        QList<ISetupSubSectionObject *> subSections = fileParser->getSubSectionsOfSection( section->getId() );
        foreach( ISetupSubSectionObject * subSection, subSections )
        sectionView->addSubSectionView( bootstrapSubSectionView( subSection, fileParser ) );
    }

    return sectionView;
}

void SettingsPresenter::populateScanedDirectoriesSection( ISettingsSectionView *sectionView ) {
    ISettingsSubSectionView *subSection = _viewFactory->createSubSectionView( NULL );
    ISettingsModel *scanDirectoriesModel = new ScanedDirectoriesSettingModel( this );
    ScanedDirectoriesSettingController *scanDirectoriesController =
        new ScanedDirectoriesSettingController( static_cast<ScanedDirectoriesSettingModel*>( scanDirectoriesModel ),
                _viewFactory,
                subSection );
    sectionView->addSubSectionView(
        scanDirectoriesController->buildScanedDirectorySettingsView() );
    _currentModels.append( scanDirectoriesModel );
}

ISettingsSubSectionView* SettingsPresenter::bootstrapSubSectionView( ISetupSubSectionObject *subSection, ISetupFileParser *fileParser ) {
    ISettingsSubSectionView *subSectionView = _viewFactory->createSubSectionView( subSection );
    QList<ISetupSettingObject *> settings = fileParser->getSettingsOfSubSection( subSection->getId() );
    foreach( ISetupSettingObject * setting, settings )
    subSectionView->addSettingView( bootstrapSettingView( setting ) );
    return subSectionView;
}

ISettingsSettingView *SettingsPresenter::bootstrapSettingView( ISetupSettingObject *setting ) {
    ISettingsSettingView *settingView = _viewFactory->createSettingView( setting );
    bootstrapModelToView( settingView, setting );
    return settingView;
}

void SettingsPresenter::bootstrapModelToView( ISettingsSettingView *view, ISetupSettingObject *setting ) {
    if( view == NULL || setting == NULL )
        return;

    ISettingsModel *model = NULL;

    if( setting->getId() == "fullScreenSetting" )
        model = new FullScreenSettingModel( this, static_cast<ISettingsBooleanSettingView *>( view ) );
    else if( setting->getId() == "playlistLoop" )
        model = new LoopPlaylistSettingModel( this, static_cast<ISettingsBooleanSettingView *>( view ) );
    else if( setting->getId() == "enableNetworkConnections" ) {
        model = new ContentDownloadSettingModel( this, static_cast<ISettingsBooleanSettingView *>( view ), _mediaLibrary );
        connect( model, SIGNAL( originalValueChanged() ), this, SIGNAL( userSettingChanged() ) );
    } else if( setting->getId() == "enableStatisticUpload" ) {
        model = new CollectStatisticsSettingModel( this, static_cast<ISettingsBooleanSettingView *>( view ), _mediaLibrary );
        connect( model, SIGNAL( originalValueChanged() ), this, SIGNAL( userSettingChanged() ) );
    } else if( setting->getId() == "videoExtensions" )
        model = new VideoFileExtensionsSettingModel( this, static_cast<ISettingsFileExtensionsSettingView *>( view ) );
    else if( setting->getId() == "audioExtensions" )
        model = new AudioFileExtensionsSettingModel( this, static_cast<ISettingsFileExtensionsSettingView *>( view ) );
    else if( setting->getId() == "imageExtensions" )
        model = new ImageFileExtensionsSettingModel( this, static_cast<ISettingsFileExtensionsSettingView *>( view ) );
    else if( setting->getId() == "documentsExtensions" )
        model = new TextFileExtensionsSettingModel( this, static_cast<ISettingsFileExtensionsSettingView *>( view ) );
    else if( setting->getId() == "presentationExtensions" )
        model = new PresentationFileExtensionsSettingModel( this, static_cast<ISettingsFileExtensionsSettingView *>( view ) );
    else if( setting->getId() == "spreadSheetExtensions" )
        model = new SpreadsheetFileExtensionsSettingModel( this, static_cast<ISettingsFileExtensionsSettingView *>( view ) );
    else if( setting->getId() == "othersExtensions" )
        model = new OthersFileExtensionsSettingModel( this, static_cast<ISettingsFileExtensionsSettingView *>( view ) );
    else if( setting->getId() == "PDF" )
        model = new PdfFileExtensionsSettingModel( this, static_cast<ISettingsFileExtensionsSettingView *>( view ) );
    else if( setting->getId() == "loopInterval" )
        model = new LoopIntervalSettingModel( this, static_cast<ISettingsIntegerSettingView *>( view ) );

    if( model != NULL )
        _currentModels.append( model );
}

void SettingsPresenter::saveDataOnClose() {
    foreach( ISettingsModel * model, _currentModels )
    model->saveData();
    _currentModels.clear();
}
