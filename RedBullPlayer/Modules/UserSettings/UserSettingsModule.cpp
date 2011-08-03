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
#include "UserSettingsModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QFile>
#include <QTimer>

#include "../../ToolLib/ConfigManager.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Container/Error.h"
#include "../../Events/UserSettings/UserSettingsChangedEvent.h"

using namespace RedBullPlayer::Modules::UserSettings;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;

UserSettingsModule::UserSettingsModule() {
    _dialog = 0;
}

void UserSettingsModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
}

void UserSettingsModule::SetupModuleConnections() {
    Aggregator::instance()->registerListener( ShowUserSettingsDialog, this );
    _mediaLibrary = resolveMediaLibrary();
    _styleFactory = resolveStyleFactory();

    if ( _mediaLibrary == 0 ) return;

    QString styleSheet = "";

    if ( _styleFactory != 0 ) {
        bool ok;
        ITextStyle* style = _styleFactory->getTextStyle( "UserSettingsDialog_Text", &ok );

        if ( ok ) {
            styleSheet = style->styleSheet();
        }
    }

    QString collectText = getFileContent( ConfigManager::instance()->collectFile() );
    QString collectCheckText = getFileContent( ConfigManager::instance()->collectCheckFile() );
    QString contentText = getFileContent( ConfigManager::instance()->contentFile() );
    QString contentCheckText = getFileContent( ConfigManager::instance()->contentCheckFile() );
    QString infoText = getFileContent( ConfigManager::instance()->infoFile() );
    _dialog = new RedBullDialog( styleSheet, RedBullDialog::Ok, _shell );
    _dialog->addLabel( collectText );
    _dialog->addCheckBox( "CollectCheck", collectCheckText, true );
    _dialog->addLabel( contentText );
    _dialog->addCheckBox( "ContentCheck", contentCheckText, true );
    _dialog->addLabel( infoText );
    connect( _dialog, SIGNAL( accepted() ), this, SLOT( dialogClosed() ) );
    connect( _dialog, SIGNAL( rejected() ), this, SLOT( dialogClosed() ) );

    if ( ConfigManager::instance()->showDialogOnStart() ) {
        QTimer::singleShot( 1000, this, SLOT( showDialog() ) );
    }
}

UserSettingsModule::~UserSettingsModule() {
    Aggregator::instance()->unregisterListener( this );
    delete _dialog;
}

void UserSettingsModule::dialogClosed() {
    if ( ConfigManager::instance()->showDialogOnStart() ) {
        ConfigManager::instance()->setShowDialogOnStart( false ); //do not show again
    }

    bool collectStatistic = _dialog->getChecked( "CollectCheck" );
    bool downloadContent = _dialog->getChecked( "ContentCheck" );
    bool oldCollectStatistic = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowCollectStatistic();
    bool oldDownloadContent = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
    bool sendChangedEvent = false;

    if ( oldCollectStatistic != collectStatistic ) {
        _mediaLibrary->mediaDB()->systemMetaDataRepository()->setAllowCollectStatistic( collectStatistic );
        sendChangedEvent = true;
    }

    if ( oldDownloadContent != downloadContent ) {
        _mediaLibrary->mediaDB()->systemMetaDataRepository()->setAllowNetworkConnection( downloadContent );
        sendChangedEvent = true;
    }

    if ( sendChangedEvent ) {
        UserSettingsChangedEvent e;
        Aggregator::instance()->sendEvent( &e );
    }
}

bool UserSettingsModule::event( QEvent* e ) {
    if ( e->type() == ShowUserSettingsDialog ) {
        onShowUserSettingsDialog( static_cast<ShowUserSettingsDialogEvent*>( e ) );
    }

    return false;
}

void UserSettingsModule::onShowUserSettingsDialog( ShowUserSettingsDialogEvent* e ) {
    showDialog();
}

void UserSettingsModule::showDialog() {
    if ( _dialog == 0 ) return;

    if ( _mediaLibrary->mediaDB()->systemMetaDataRepository()->getMetaData( PLAYER_ALLOW_COLLECT_STATISTIC_DATAKEY ).isEmpty() &&
            _mediaLibrary->mediaDB()->systemMetaDataRepository()->getMetaData( PLAYER_ALLOW_NETWORK_CONNECTION_DATAKEY ).isEmpty() ) {
        _dialog->setChecked( "CollectCheck", true );
        _dialog->setChecked( "ContentCheck", true );
    } else {
        _dialog->setChecked( "CollectCheck", _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowCollectStatistic() );
        _dialog->setChecked( "ContentCheck", _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection() );
    }

    _dialog->exec();
}

QString UserSettingsModule::getFileContent( QString fileName ) {
    QString retVal = "";
    QFile file( fileName );

    if( QFile( fileName ).exists() && file.open( QFile::ReadOnly ) ) {
        retVal = QString( file.readAll() );
        file.close();
    }

    return retVal;
}

IMediaLibrary* UserSettingsModule::resolveMediaLibrary() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        return retVal;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
    return 0;
}

IStyleFactory* UserSettingsModule::resolveStyleFactory() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        return retVal;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
    return 0;
}

Q_EXPORT_PLUGIN2( StatisticsModule, RedBullPlayer::Modules::UserSettings::UserSettingsModule );
