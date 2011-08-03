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
#include "PlayerUpdateCheckerModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QTimer>
#include <QDateTime>

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/FileSystem/Storage.h"
#include "../../ToolLib/FileSystem/StorageFactory.h"

#include "../../UpdateServices/PlayerVersion.h"
#include "../../UpdateServices/Version.h"

#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Interfaces/MediaLibrary/ISystemMetaDataRepository.h"

#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Playlist/PlaylistGenerationFinishedEvent.h"
#include "../../Events/TaskManager/TaskExecuteRequestEvent.h"
#include "../../Events/TaskManager/TaskFinishedEvent.h"
#include "../../Events/TaskManager/TaskCancelRequestEvent.h"
#include "../../Events/TaskManager/TaskProgressEvent.h"
#include "../../Events/Update/UpdateAvailableForInstallEvent.h"
#include "../../Widgets/RedBullDialog.h"


#include "UpdateAvailableBox.h"
#include "UpdateServiceUrlGenerator.h"


using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::PlayerUpdateChecker;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::StyleFactory;

PlayerUpdateCheckerModule::PlayerUpdateCheckerModule() : IPlayerUpdateChecker() {
    _internetAvailable = false;
    _allowAutomaticDownload = false;
}

void PlayerUpdateCheckerModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
    _styleFactory = resolveStyleFactory();
    //Prepare Text for UpdateDialog Box
    QFile file( Constants::PLAYERUPDATECHECKER_TEXT_DIR + Constants::PLAYERUPDATECHECKER_DIALOG_NEW );

    if ( file.open( QFile::ReadOnly ) ) {
        QByteArray fileArray = file.readAll();
        file.close();
        _updateBoxText = QString( fileArray );
    } else {
        _updateBoxText = "A new version for the Red Bull Media Player ({#ServerVersion}) is available now - would you like to download?";
    }

    PlayerVersion::instance()->parseString( &_updateBoxText );
    QFile diskSpaceFile( Constants::PLAYERUPDATECHECKER_TEXT_DIR + Constants::PLAYERUPDATECHECKER_DIALOG_DISKSPACE );

    if ( diskSpaceFile.open( QFile::ReadOnly ) ) {
        QByteArray fileArray = diskSpaceFile.readAll();
        diskSpaceFile.close();
        _diskSpaceWarningText = QString( fileArray );
    } else {
        _diskSpaceWarningText = "Please free up some disk space before you can proceed with the Application Update!";
    }

    PlayerVersion::instance()->parseString( &_diskSpaceWarningText );
    QFile downloadFinishedFile( Constants::PLAYERUPDATECHECKER_TEXT_DIR + Constants::PLAYERUPDATECHECKER_DIALOG_FINISHED );

    if ( downloadFinishedFile.open( QFile::ReadOnly ) ) {
        QByteArray fileArray = downloadFinishedFile.readAll();
        downloadFinishedFile.close();
        _downloadFinishedText = QString( fileArray );
    } else {
        _downloadFinishedText = "Your Player Update has been downloaded successfully. Please close your Red Bull Media Player for the installation to proceed.";
    }

    PlayerVersion::instance()->parseString( &_diskSpaceWarningText );
    /* Get PlayerId */
    IMediaLibrary* mediaLibrary = 0;
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;

    if ( ! mediaLibrary ) {
        qFatal( "Could not find MeniaLIbrary module" );
        return;
    }

    UpdateServiceUrlGenerator g( mediaLibrary-> mediaDB()->systemMetaDataRepository()->getPlayerUUID(),
                                 ConfigManager::instance()->getApiKey(),
                                 PlayerVersion::instance(),
                                 this );
    QUrl serviceUrl = g.generateUrl();
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER,
                                     QString( "ServiceUrl:%1" ).arg( serviceUrl.toString() ) );
    _client = new VersionServiceClient( serviceUrl,
                                        this );
    connect( _client,
             SIGNAL( versionFound( QString, QString, qulonglong, qulonglong ) ),
             this,
             SLOT( versionFound( QString, QString, qulonglong, qulonglong ) ) );
    connect( _client,
             SIGNAL( noVersionFound() ),
             this,
             SLOT( noVersionFound() ) );
    _updateTask = 0;
    _checked = false;
    _updateDownloadInProgress = false;
    Aggregator::instance()->registerListener( TaskFinished, this );
    Aggregator::instance()->registerListener( InternetConnectionStatusChanged, this );
    Aggregator::instance()->registerListener( UserSettingsChanged, this );
}

void PlayerUpdateCheckerModule::SetupModuleConnections() {
    resolveMediaLibrary();
    _allowAutomaticDownload = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
}

bool PlayerUpdateCheckerModule::resolveMediaLibrary() {
    bool success = false;
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
        success = true;
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
    return success;
}

IStyleFactory* PlayerUpdateCheckerModule::resolveStyleFactory() {
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

bool PlayerUpdateCheckerModule::event( QEvent* e ) {
    if ( e->type() == TaskFinished ) {
        TaskFinishedEvent* taskExecuteResponse = static_cast<TaskFinishedEvent*>( e );
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER, "Recieved TaskFinished Event" );

        if ( taskExecuteResponse->taskId() == _updateTaskId ) {
            if ( _updateTask->result()->value() == TaskResult::SUCCESSFUL ) {
                SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER,
                                                 QString( "UpdateDownloadTask finished to %1. Send UpdateAvailableForInstallEvent" ).arg( _updateTask->localFileName() ) );
                qDebug() << "PlayerUpdateCheckerModule => UpdateAvailableForInstallEvent";
                QString styleSheet = "";

                if ( _styleFactory != 0 ) {
                    bool ok;
                    ITextStyle* style = _styleFactory->getTextStyle( "UserSettingsDialog_Text", &ok );

                    if ( ok ) {
                        styleSheet = style->styleSheet();
                    }
                }

                RedBullDialog* dialog = new RedBullDialog( _downloadFinishedText,
                        styleSheet,
                        RedBullDialog::Ok,
                        _shell );
                dialog->exec();
                UpdateAvailableForInstallEvent ev( _updateTask->localFileName() );
                Aggregator::instance()->sendEvent( &ev );
            }

            _updateDownloadInProgress = false;
            //_updateTask->deleteLater();
            _updateTaskId = QUuid();
            _updateTask = 0;
        }
    }

    if ( e->type() == InternetConnectionStatusChanged ) {
        onInternetConnectionStatusChanged( static_cast<InternetConnectionStatusChangedEvent*>( e ) );
    }

    if( e->type() == UserSettingsChanged ) {
        onUserSettingsChanged( static_cast<UserSettingsChangedEvent*>( e ) );
    }

    return false;
}

void PlayerUpdateCheckerModule::onInternetConnectionStatusChanged( InternetConnectionStatusChangedEvent *e ) {
    _internetAvailable = e->internetAvailable();
    resolveConnectivityChanged();
}

void PlayerUpdateCheckerModule::onUserSettingsChanged( UserSettingsChangedEvent *e ) {
    _allowAutomaticDownload = _mediaLibrary->mediaDB()->systemMetaDataRepository()->allowNetworkConnection();
    resolveConnectivityChanged();
}

void PlayerUpdateCheckerModule::resolveConnectivityChanged() {
    if ( _internetAvailable && _allowAutomaticDownload ) {
        if ( !_checked ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER, "Performing update check doUpdate()" );
            doCheck();
            _checked = true;
        } else {
            SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER, "Check allready done!" );
        }
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER, "No more internet connection" );

        if ( _updateDownloadInProgress && ! _updateTaskId.isNull() ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER, "Cancel Download Task" );
            TaskCancelRequestEvent ev( _updateTaskId );
            Aggregator::instance()->sendEvent( &ev );
        }
    }
}


void PlayerUpdateCheckerModule::versionFound( QString newPlayerVersion, QString downloadUrl, qulonglong zipSize, qulonglong unzipSize ) {
    ConfigManager::instance()->setLastCheck( QDateTime::currentDateTime() );
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER,
                                     "versionFound" );
    QString skippedStr = ConfigManager::instance()->getSkipped();
    Version skippedVersion( skippedStr );
    Version serverVersion( newPlayerVersion );

    if ( skippedVersion.compare( serverVersion ) == 0 ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER,
                                         "Version Skipped" );
        return;
    }

    Version currentVersion( PlayerVersion::instance()->getVersionString() );

    if ( currentVersion.compare( serverVersion ) < 0 ) {
        /* New Version */
        QString temp = QString( _updateBoxText );
        temp = temp.replace( "{#ServerVersion}", newPlayerVersion );
        UpdateAvailableBox *updateBox = new UpdateAvailableBox( temp, _shell );
        int retVal = updateBox->exec();

        switch( retVal ) {
            case 0 : {
                    ConfigManager::instance()->setSkipped( newPlayerVersion );
                    break;
                } //No

            case 1: {
                    //Check if enough disk space free
                    quint64 freeBytes = StorageFactory::defaultStorage()->freeBytes();
                    bool doDownload = false;

                    if ( freeBytes > zipSize ) {
                        if ( unzipSize != 0 ) {
                            doDownload = ( freeBytes > unzipSize );
                        } else {
                            doDownload = true;
                        }
                    }

                    if ( !doDownload ) {
                        //RBS-811 Fehlendes Feedback bei zu wenig Speicherplatz
                        QString styleSheet = "";

                        if ( _styleFactory != 0 ) {
                            bool ok;
                            ITextStyle* style = _styleFactory->getTextStyle( "UserSettingsDialog_Text", &ok );

                            if ( ok ) {
                                styleSheet = style->styleSheet();
                            }
                        }

                        RedBullDialog* dialog = new RedBullDialog( _diskSpaceWarningText,
                                styleSheet,
                                RedBullDialog::Ok,
                                _shell );
                        dialog->exec();
                    }

                    if ( doDownload ) {
                        _updateTask = new UpdateDownloadTask( QString( "Red Bull Media Player v%1" ).arg( serverVersion.versionString() ), 0 );
                        _updateTaskId = _updateTask->guid();
                        connect( _updateTask, SIGNAL( downloadProgress( QUuid, int ) ), this, SLOT( downloadProgress( QUuid, int ) ) );
                        QString tempDir = ConfigManager::instance()->getTempDirectory();
                        _updateTask->setTargetDirectory( tempDir );
                        _updateTask->setDownloadUrl( QUrl( downloadUrl ) );
                        _updateDownloadInProgress = true;
                        TaskExecuteRequestEvent e( _updateTask );
                        Aggregator::instance()->sendEvent( &e );
                    }

                    break;
                } //Yes

            case 2 : //Later
                //Do nothing until next start
                break;

            default:
                break;
        }
    }
}

void PlayerUpdateCheckerModule::downloadProgress( QUuid taskId, int progress ) {
    TaskProgressEvent ev( taskId, progress );
    Aggregator::instance()->sendEvent( &ev );
}

void PlayerUpdateCheckerModule::noVersionFound() {
    ConfigManager::instance()->setLastCheck( QDateTime::currentDateTime() );
}

void PlayerUpdateCheckerModule::doCheck() {
    if ( QFile::exists( Constants::UPDATE_AVAILABLE_MARKER_FILENAME ) ) {
        return;
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER,
                                     "DoCheck" );
    int interval = ConfigManager::instance()->getInterval();
    QDateTime lastCheck = ConfigManager::instance()->getLastCheck();

    if ( lastCheck.addDays( interval ).date() <= QDateTime::currentDateTime().date() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER,
                                         "CheckForUpdate" );
        _client->checkForUpdate();
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER,
                                         "LastCheck" );
    }
}


Q_EXPORT_PLUGIN2( PlayerUpdateCheckerModule, RedBullPlayer::Modules::PlayerUpdateChecker::PlayerUpdateCheckerModule );
