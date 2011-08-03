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
#include "UpdateInstaller.h"

// QT Includes
#include <QtGlobal>
#include <QProcess>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTimer>

// Project includes
#include "../../ToolLib/UuidCreator.h"
#include "../../Package/Package.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

#if defined(Q_OS_MAC)
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFBundle.h>
#endif



using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::UpdateInstaller;
using namespace RedBullPlayer::Updater;
#ifdef Q_OS_WIN32
const QString UpdateInstaller::_updateApplicationName = "Updtr.exe";
#endif
#ifdef Q_OS_LINUX
const QString UpdateInstaller::_updateApplicationName = "Updtr.com";
#endif
#ifdef Q_OS_MAC
const QString UpdateInstaller::_updateApplicationName = "Updtr";
#endif

const QString UpdateInstaller::_packagedVersionFileName = "version.nfo";
const QString UpdateInstaller::_extracedVersionFileName = "update_target_version.nfo";


UpdateInstaller::UpdateInstaller( RedBullPlayer::Updater::PlayerVersion* currentVersion,
                                  QString updateFileName,
                                  RedBullPlayer::PlayerShell::Shell* shell,
                                  RedBullPlayer::Widgets::UpdateProgressWidget* progressWidget,
                                  QObject* parent )
    : QThread( parent ) {
    Q_ASSERT( currentVersion != 0 );
    Q_ASSERT( shell != 0 );
    Q_ASSERT( ! updateFileName.isNull() );
    _installerFileName = shell->installerStubAbsolutePath();
    _currentVersion = currentVersion;
    _updateFileName = updateFileName.simplified();
    _targetVersion = 0;
    connect( this, SIGNAL( doStepSignal( QString ) ),
             progressWidget, SLOT( doStep( QString ) ) );
    connect( this, SIGNAL( finished() ),
             progressWidget, SLOT( close() ) );
    this->moveToThread( this );
}


void UpdateInstaller::run() {
    qDebug() << Q_FUNC_INFO;
    qDebug() << "Worker: QThread::currentThreadId: " << QThread::currentThreadId();
    QTimer *timer = new QTimer( this );
    timer->setSingleShot( true );
    connect( timer, SIGNAL( timeout() ),
             this, SLOT( startWork() ) );
    timer->start( 50 );
    exec();
    qDebug() << "EXIT: "  << Q_FUNC_INFO;
}


void UpdateInstaller::startWork() {
    qDebug() << Q_FUNC_INFO;
    setupUpdaterApplication();
    quit();
}

#if  defined(Q_OS_MAC)
void UpdateInstaller::setupUpdaterApplication() {
//    emit doStepSignal( tr( "Setting up updater application..." ) );
    emit doStepSignal( tr( "Extracting update package..." ) );
    emit setupUpdaterStarted();
    SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER, "Setting up updater application..." );
    QDir mkdir;
    mkdir.mkdir( "../../../playerUpdate" );
    QDir dir( "../../../playerUpdate" );

    QString targetDiretory = dir.absolutePath().toLatin1();
    targetDiretory = QDir( QDir::currentPath() ).relativeFilePath( targetDiretory );
//    QMessageBox::information(0, "inf", targetDiretory );
    _updateFileName = QDir( QDir::currentPath() ).relativeFilePath( _updateFileName );
//    QMessageBox::information(0, "inf", _updateFileName );
    QStringList unzipArguments;
    unzipArguments << _updateFileName.toLatin1() << "-d" << targetDiretory;
    QProcess unzip;
    unzip.start( "/usr/bin/unzip", unzipArguments );
    if ( ! unzip.waitForStarted() ) {
	SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER, "Could not start unzip application!" );
	emit doStepSignal( tr( "Could not start unzip application! Update failed!" ) );
	return;
    }
    if ( ! unzip.waitForFinished(-1) ) {
	SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER, "Could unzip update package!" );
	emit doStepSignal( tr( "Could unzip update package! Update failed!" ) );
	return;
    }
    if ( unzip.exitCode() != 0 ) {
	QMessageBox::warning( 0, "Error", "An error ocoured during update!" );
	return;
    }
    QFileInfo fi( _updateFileName );
    _updateFileName =  dir.absolutePath() + "/" + fi.fileName().replace( ".zip", "" );
    _updateFileName = QDir( QDir::currentPath() ).relativeFilePath( _updateFileName );
//    QMessageBox::information(0, "inf", _updateFileName );
    QProcess chmod;
    QStringList chmodArguments;
    chmodArguments << "+x" << _updateFileName.toLatin1() + "/Contents/MacOS/" + _updateApplicationName.toLatin1();
    chmod.start( "/bin/chmod", chmodArguments );
    if ( ! chmod.waitForStarted() ) {
	SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER, "Could not start chmod!" );
	emit doStepSignal( tr( "Could not start chmod! Update failed!" ) );
	return;
    }
    if ( ! chmod.waitForFinished(-1) ) {
	SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER, "Could not make updater application executable!" );
	emit doStepSignal( tr( "Could not make updater application executable! Update failed!" ) );
	return;
    }
    if ( chmod.exitCode() != 0 ) {
	QMessageBox::warning( 0, "Error", "An error ocoured during update!" );
	return;
    }
    _targetVersion = new PlayerVersion( this, _updateFileName + "/Contents/MacOS/" +  _packagedVersionFileName );
    emit setupUpdaterFinished();
    executeUpdate();
}
#else
void UpdateInstaller::setupUpdaterApplication() {
    qDebug() << Q_FUNC_INFO;
    emit doStepSignal( tr( "Setting up updater application..." ) );
    emit setupUpdaterStarted();
    SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER, "Setting up updater application..." );
    qDebug() << "Logger ok!";
    QFileInfo updateFile( _updateFileName );
    qDebug() << "_updateFileName: " << _updateFileName;
    qDebug() << "_updateApplicationName: " << _updateApplicationName;
    qDebug() << "_extracedVersionFileName: " << _extracedVersionFileName;

    if ( ! updateFile.exists() ) {
        SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER,
                                         "UpdateFile " + updateFile.absoluteFilePath() + " does not exist." );
        SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER,
                                         "Update Aborted!" );
        emit setupUpdaterFailed( "UpdateFile " + updateFile.absoluteFilePath() + " does not exist." );
        qWarning() << "UpdateFile " << updateFile.absoluteFilePath() << " does not exist.";
        return;
    }

    QFileInfo updater( _updateApplicationName );

    if ( updater.exists() ) {
        QString bkFileName = _updateApplicationName + "_" + UuidCreator::create().toString();
        QFile::rename( _updateApplicationName, bkFileName );
        SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER,
                                        "Creating backup of existing updater application at  " + bkFileName + "." );
    }

    QFileInfo targetVersionFile( _extracedVersionFileName );

    if ( targetVersionFile.exists() ) {
        QString bkFileName = _extracedVersionFileName + "_" + UuidCreator::create().toString();
        QFile::rename( _extracedVersionFileName, bkFileName );
        SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER,
                                        "Creating backup of existing target version file at  " + bkFileName + "." );
    }

    qDebug() << "Opening Package";
    Package* package = Package::openEdit( updateFile );
    qDebug() << "Extracting Updater...";
    package->extractFile( _updateApplicationName, _updateApplicationName );
    qDebug() << "Extracting Version.nfo...";
    package->extractFile( _packagedVersionFileName, _extracedVersionFileName );
    package->deleteLater();
    _targetVersion = new PlayerVersion( 0, _extracedVersionFileName );
    emit setupUpdaterFinished();
    executeUpdate();
    qDebug() << "EXIT: " << Q_FUNC_INFO;
}
#endif

#if defined(Q_OS_MAC)
void UpdateInstaller::executeUpdate() {
    emit doStepSignal( tr( "Starting updater..." ) );
    emit updaterStarted();
    CFURLRef pluginRef = CFBundleCopyBundleURL( CFBundleGetMainBundle() );
    CFStringRef macPath = CFURLCopyFileSystemPath( pluginRef,
                          kCFURLPOSIXPathStyle );
    const char *pathPtr = CFStringGetCStringPtr( macPath,
                          CFStringGetSystemEncoding() );
    qDebug( "Path = %s", pathPtr );
    CFRelease( pluginRef );
    CFRelease( macPath );
    QStringList arguments;
    arguments << _currentVersion->getVersionString()
              << _targetVersion->getVersionString()
              << ""
              << pathPtr;
    SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER,
                                    "Executing update application fromVersion: " + _currentVersion->getVersionString()
                                    + " toVersion: " + _targetVersion->getVersionString()
                                    + " updateFileName: " +
                                    + " originaApplicationPath: " + pathPtr );
    qDebug() <<  "Executing update application fromVersion: " << _currentVersion->getVersionString()
             << " toVersion: " << _targetVersion->getVersionString()
             << " updateFileName: "
             << " dataFile: " << _updateFileName;
    QProcess p;
    bool ok = p.startDetached( _updateFileName + "/Contents/MacOS/" + _updateApplicationName, arguments );

    if ( ok ) {
        SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER,
                                        "Update application started.." );
        emit updaterFinished();
    } else {
        SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER,
                                         "Starting update application failed!" );
        emit updaterFailed( QString( "Starting update application failed!" ) );
    }
}
#else
void UpdateInstaller::executeUpdate() {
    emit doStepSignal( tr( "Starting updater..." ) );
    emit updaterStarted();
    QStringList arguments;
    arguments << _currentVersion->getVersionString()
              << _targetVersion->getVersionString()
              << _updateFileName.replace( " ", "\\ ")
              << _installerFileName.replace( " ", "\\ ");
    SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER,
                                    "Executing update application fromVersion: " + _currentVersion->getVersionString()
                                    + " toVersion: " + _targetVersion->getVersionString()
                                    + " dataFile: " + _updateFileName.replace( " ", "\\ ")
                                    + " StubFilePath: " + _installerFileName.replace( " ", "\\ ") );
    qDebug() <<  "Executing update application fromVersion: " << _currentVersion->getVersionString()
             << " toVersion: " << _targetVersion->getVersionString()
             << " dataFile: " << _updateFileName.replace( " ", "\\ ")
             << " StubFilePath: " << _installerFileName.replace( " ", "\\ ");
#ifdef Q_OS_LINUX
    QFile f( _updateApplicationName );
    f.setPermissions(
        QFile::ExeGroup |
        QFile::ExeOther |
        QFile::ExeOwner |
        QFile::ExeUser |
        QFile::ReadOwner |
        QFile::ReadGroup |
        QFile::ReadOther |
        QFile::WriteOwner |
        QFile::WriteGroup
    );
    QFile startScript( "startUpdater.com" );

    if ( startScript.exists() ) {
        startScript.remove();
    }

    if ( ! startScript.open( QFile::WriteOnly ) ) {
        // QMessageBox::warning(0,"Failed to create File!", "Could not create update available file!" );
        return;
    }

    QTextStream ts( &startScript );
    ts << "#!/bin/bash \n"
       << "export LD_LIBRYRY_PATH=./ \n"
       << "./" << _updateApplicationName
       << " " << arguments.join( " " ) << "\n";
    ts.flush();
    startScript.flush();
    startScript.close();
    startScript.setPermissions(
        QFile::ExeGroup |
        QFile::ExeOther |
        QFile::ExeOwner |
        QFile::ExeUser |
        QFile::ReadOwner |
        QFile::ReadGroup |
        QFile::ReadOther |
        QFile::WriteOwner |
        QFile::WriteGroup
    );
    QProcess p;
    bool ok = p.startDetached( "./startUpdater.com" );
#else
    QProcess p;
    bool ok = p.startDetached( _updateApplicationName, arguments );
#endif

    if ( ok ) {
        SimpleLogger::instance()->info( Constants::LOGGER_UPDATEINSTALLER,
                                        "Update application started.." );
        qDebug() << "Started Updater";
        emit updaterFinished();
    } else {
        SimpleLogger::instance()->error( Constants::LOGGER_UPDATEINSTALLER,
                                         "Starting update application failed!" );
        qDebug() << "Starting Updater Failed!";
        qDebug()  << p.errorString();
        emit updaterFailed( QString( "Starting update application failed!" ) );
    }
}
#endif




