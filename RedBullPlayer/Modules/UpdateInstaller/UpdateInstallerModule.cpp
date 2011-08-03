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
#include "UpdateInstallerModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../UpdateServices/PlayerVersion.h"
#include "../../Widgets/UpdateProgressWidget.h"
#include "UpdateInstaller.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::UpdateInstaller;
using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Widgets;

UpdateInstallerModule::UpdateInstallerModule() : IUpdateInstaller() { }

void UpdateInstallerModule::Setup( QObject* shell, QDomElement config ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_UPDATEINSTALLER, "Setup Module." );
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString instanceName = config.attribute( "instanceName" );
    Q_ASSERT( ! instanceName.isNull() );
    setInstanceName( instanceName );
}

void UpdateInstallerModule::SetupModuleConnections() {
    SimpleLogger::instance()->debug( Constants::LOGGER_UPDATEINSTALLER, "Bootstrap UpdateInstaller." );
    Aggregator::instance()->registerListener( UpdateAvailableForInstall, this );
}

bool UpdateInstallerModule::event ( QEvent * e ) {
    if ( e->type() == UpdateAvailableForInstall ) {
        onUpdateAvailableForInstall( static_cast<RedBullPlayer::Events::UpdateAvailableForInstallEvent*>( e ) );
    }

    return false;
}

void UpdateInstallerModule::onUpdateAvailableForInstall( RedBullPlayer::Events::UpdateAvailableForInstallEvent *e ) {
    SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER, "Installing update available at: " + e->updateFileName() );
    QFile updateMarker( Constants::UPDATE_AVAILABLE_MARKER_FILENAME );

    if ( ! updateMarker.open( QFile::WriteOnly ) ) {
        //    QMessageBox::warning(0,"Failed to create File!", "Could not create update available file!" );
        return;
    }

    QTextStream ts( &updateMarker );
    ts << e->updateFileName();
    ts.flush();
    updateMarker.flush();
    updateMarker.close();
}

bool UpdateInstallerModule::updateAvailable() const {
    return QFile::exists( Constants::UPDATE_AVAILABLE_MARKER_FILENAME );
}
bool UpdateInstallerModule::installUpdate() {
    _updateProgress = new UpdateProgressWidget( 0 );
    _updateProgress->setHeader( tr( "Updating Application" ) );
    _updateProgress->setSteps( 2 );
    _updateProgress->show();
    _updateProgress->raise();
    QFile updateMarker( Constants::UPDATE_AVAILABLE_MARKER_FILENAME );

    if ( ! updateMarker.open( QFile::ReadOnly ) ) {
        QMessageBox::warning( 0, "Failed to read File!", "Could not read update available file!" );
        return false;
    }

    QTextStream stream( &updateMarker );
    QString fileName = stream.readAll();
    updateMarker.close();
    UpdateInstaller* installer = new UpdateInstaller( PlayerVersion::instance(), fileName, _shell, _updateProgress, 0 );
    QCoreApplication::processEvents();
    qDebug() << Q_FUNC_INFO;
    qDebug() << "QThread::currentThreadId: " << QThread::currentThreadId();
    installer->start( QThread::NormalPriority );
    return true;
}

void UpdateInstallerModule::threadFinished() {
    qDebug() << Q_FUNC_INFO;
    emit installerFinished();
}

void UpdateInstallerModule::setupUpdaterStarted() {
    qDebug() << Q_FUNC_INFO;
    _updateProgress->doStep( tr( "Setting up Updater Application..." ) );
    QCoreApplication::processEvents();
}

void UpdateInstallerModule::setupUpdaterFinished() {
    qDebug() << Q_FUNC_INFO;
    _updateProgress->setHeader( "setupUpdaterFinished" );
    SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER, "Setting up Update Application finished" );
}
void UpdateInstallerModule::setupUpdaterFailed( QString msg ) {
    qDebug() << Q_FUNC_INFO;
    _updateProgress->setHeader( msg );
    SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER, "Setting up Update Application Failed: " + msg );
}
void UpdateInstallerModule::updaterStarted() {
    qDebug() << Q_FUNC_INFO;
    _updateProgress->setHeader( "Executing Updater Application..." );
    _updateProgress->doStep( tr( "Executing Updater Application..." ) );
    QCoreApplication::processEvents();
}

void UpdateInstallerModule::updaterFinished() {
    qDebug() << Q_FUNC_INFO;
    _updateProgress->setHeader( "Update Application finished" );
    SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER, "Update Application finished" );
    _updateProgress->close();
}
void UpdateInstallerModule::updaterFailed( QString msg ) {
    qDebug() << Q_FUNC_INFO;
    _updateProgress->setHeader( msg );
    SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER, "Update Application failed: " + msg );
    _updateProgress->close();
}

void UpdateInstallerModule::timeShot() {
#if defined(Q_OS_WIN)
    QDir d;
    d.mkpath( "update" );
    QFile f;
    f.copy( _shell->installerStubAbsolutePath(), "update/UpdateData" );
    QMessageBox::information( 0, "Copied update file - raising event...", "Copied update file - raising event..." );
    UpdateAvailableForInstallEvent ev( "update/UpdateData" );
    Aggregator::instance()->sendEvent( &ev );
#elif defined(Q_OS_MAC)
    QDir d;
    d.mkpath( "update" );
    QMessageBox::information( 0, "Copied update file - raising event...", "Copied update file - raising event..." );
    QFileInfo fi( "../../../Player.app.zip" );
    UpdateAvailableForInstallEvent ev( fi.absoluteFilePath() );
    Aggregator::instance()->sendEvent( &ev );
#endif
}

Q_EXPORT_PLUGIN2( UpdateInstallerModule, RedBullPlayer::Modules::UpdateInstaller::UpdateInstallerModule );
