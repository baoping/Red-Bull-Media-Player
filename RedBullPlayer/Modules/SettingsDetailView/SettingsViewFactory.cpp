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
#include "SettingsViewFactory.h"

#include "../../Interfaces/StyleFactory/ITextStyle.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../Container/Constants.h"
#include "SetupFileParser/SettingsViewSetupFileParser.h"
#include "SectionView.h"
#include "SubSectionView.h"
#include "FileExtensionsSettingView.h"
#include "BooleanSettingView.h"
#include "ScanedDirectorySettingView.h"
#include "AddScanedDirectorySettingView.h"
#include "IntegerSettingView.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

SettingsViewFactory::SettingsViewFactory( QObject* parent, QWidget* viewsParent ) : ISettingsViewFactory( parent ) {
    _viewsParent = viewsParent;
    _setupFileParser = new SettingsViewSetupFileParser( this );
}

void SettingsViewFactory::setStyleFactory( IStyleFactory *styleFactory ) {
    _styleFactory = styleFactory;
}

ISettingsSectionView *SettingsViewFactory::createSectionView( ISetupSectionObject *section ) {
    ISettingsSectionView *sectionView = new SectionView( section->getTitleText(), section->getHelpText(), _viewsParent );
    resolveSectionStyle( sectionView );
    return sectionView;
}

ISettingsSubSectionView *SettingsViewFactory::createSubSectionView( ISetupSubSectionObject *subSection ) {
    QString subSectionTitle = "";

    if( subSection != NULL )
        subSectionTitle = subSection->getTitleText();

    ISettingsSubSectionView *subSectionView = new SubSectionView( _viewsParent, subSectionTitle );
    resolveSubSectionStyle( subSectionView );
    return subSectionView;
}

ISettingsSettingView *SettingsViewFactory::createSettingView( ISetupSettingObject *setting ) {
    if( setting->getType() == "boolSetting" )
        return createBooleanSettingView( setting );
    else if( setting->getType() == "intSetting" )
        return createIntegerSettingView( setting );
    else
        return createFileExtensionSettingView( setting );
}

ISettingsSettingView *SettingsViewFactory::createFileExtensionSettingView( ISetupSettingObject *setting ) {
    ISettingsSettingView *settingView = new FileExtensionsSettingView( _viewsParent, UuidCreator::create(), setting->getLabelText() );
    resolveSettingStyle( settingView, "Settings_FileExtensions_Label" );
    return settingView;
}

ISettingsSettingView *SettingsViewFactory::createBooleanSettingView( ISetupSettingObject *setting ) {
    ISettingsBooleanSettingView *settingView = new BooleanSettingView( _viewsParent, UuidCreator::create(), setting->getLabelText() );
    resolveSettingStyle( settingView );
    return settingView;
}

ISettingsSettingView *SettingsViewFactory::createIntegerSettingView( ISetupSettingObject *setting ) {
    ISettingsIntegerSettingView *settingView = new IntegerSettingView( _viewsParent, UuidCreator::create(), setting->getLabelText() );
    resolveSettingStyle( settingView );
    return settingView;
}

ISettingsAddDirectorySettingView *SettingsViewFactory::createAddDirectorySettingView() {
    ISettingsAddDirectorySettingView *settingView = new AddScanedDirectorySettingView( _viewsParent, UuidCreator::create() );
    resolveSettingStyle( settingView );
    return settingView;
}

ISettingsScanDirectorySettingView *SettingsViewFactory::createScanDirectorySettingView() {
    ISettingsScanDirectorySettingView *settingView = new ScanedDirectorySettingView( _viewsParent,
            UuidCreator::create() );
    resolveSettingStyle( settingView );
    return settingView;
}

void SettingsViewFactory::resolveSectionStyle( ISettingsSectionView *sectionView ) {
    if( _styleFactory == NULL )
        return;

    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "SettingsSection_Title", &ok );

    if( ok )
        sectionView->setTitleStyle( titleTextStyle );
}

void SettingsViewFactory::resolveSubSectionStyle( ISettingsSubSectionView *subSectionView ) {
    if( _styleFactory == NULL )
        return;

    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "SettingsSubSection_Title", &ok );

    if( ok )
        subSectionView->setTitleStyle( titleTextStyle );
}

void SettingsViewFactory::resolveSettingStyle( ISettingsSettingView *settingView, QString labelStyleName,
        QString textBoxStyleName ) {
    if( _styleFactory == NULL )
        return;

    bool ok;
    ITextStyle *labelTextStyle = _styleFactory->getTextStyle( labelStyleName, &ok );

    if( ok )
        settingView->setLabelStyle( labelTextStyle );

    ITextStyle *textBoxTextStyle = _styleFactory->getTextStyle( textBoxStyleName, &ok );

    if( ok )
        settingView->setTextBoxStyle( textBoxTextStyle );
}

QWidget *SettingsViewFactory::getViewsParent() {
    return _viewsParent;
}


