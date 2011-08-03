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
#include "UpdateEngine.h"

// QT includes
#include <QString>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
// Project Includes
#include "../Container/Constants.h"
#include "../ToolLib/SimpleLogger.h"
#include "UpdateHistoryUpdateTask.h"

// UpdateSteps
#include "v0_9_to_1_0/ConfigFileTask_0_9_to_1_0.h"

#if defined(Q_OS_MAC)
#include "MacOSXInstallerTask.h"
#else
#include "Win32InstallerTask.h"
#endif

using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;


UpdateEngine::UpdateEngine( QObject* parent ) : QThread( parent ) {
    _updateManager = new UpdateManager( this );
    _commonPreUpdate = new UpdateProcess( _fromVersion, _toVersion, this );
    _commonPostUpdate = new UpdateProcess( _fromVersion, _toVersion, this );
}

void UpdateEngine::prepareExecute(
    RedBullPlayer::Widgets::UpdateProgressWidget* progressWidget,
    QString fromVersion,
    QString toVersion,
    QString updateDataFile,
    QString originalPackageFileName ) {
    qDebug() << "Update from: " << fromVersion << " to " << toVersion << " file: " << updateDataFile;
    SimpleLogger::instance()->info( Constants::LOGGER_UPDATER,
                                    QString( "Update Form %1 to %2 using updateFile: %3." )
                                    .arg( fromVersion ).arg( toVersion ).arg( updateDataFile ) );
    _progressWidget = progressWidget;
    _fromVersion = fromVersion;
    _toVersion = toVersion;
    _updateFileName = updateDataFile;
    _originalPackageFileName = originalPackageFileName;
    _progressWidget->setHeader( tr( "Performing Application update from %1 to %2" ).arg( _fromVersion ).arg( _toVersion ) );
    QCoreApplication::processEvents();
    setupCommonPreUpdateTasks();
    setupCustomUpdateTasks();
    setupCommonPostUpdateTasks();
    // QMessageBox::information(0, "Updater",QString("Update Form %1 to %2 using updateFile: %3. using %4 steps.")
    //                                .arg(fromVersion).arg(toVersion).arg(updateDataFile).arg( taskCount() ) );
    _progressWidget->setSteps( taskCount() );
    QCoreApplication::processEvents();
    connect( _updateManager, SIGNAL( taskStarted( QString ) ),
             _progressWidget, SLOT( doStep( QString ) ) );
    connect( _commonPostUpdate, SIGNAL( taskStarted( QString ) ),
             _progressWidget, SLOT( doStep( QString ) ) );
    connect( _commonPreUpdate, SIGNAL( taskStarted( QString ) ),
             _progressWidget, SLOT( doStep( QString ) ) );
    connect( this, SIGNAL( updateMessage( QString ) ),
             _progressWidget, SLOT( setHeader( QString ) ) );
}

void UpdateEngine::run() {
    runTasks();
}

int UpdateEngine::taskCount() {
    UpdateProcess* customProcess = _updateManager->findMatchingProcess( new PlayerVersion( _fromVersion, this ), new PlayerVersion( _toVersion, this ) );
    int preSteps = _commonPreUpdate->tasks().count();
    //   qDebug() << "Update _commonPreUpdate->tasks(): " <<  preSteps << " tasks";
    int customSteps = 0;

    if ( customProcess != 0 ) {
        customSteps = customProcess->tasks().count();
    }

    //   qDebug() << "Update customProcess->tasks(): " << customSteps << " tasks";
    int postSteps = _commonPostUpdate->tasks().count();
    //   qDebug() << "Update _commonPostUpdate->tasks(): " << postSteps << " tasks";
    int tc = preSteps + customSteps + postSteps;
    qDebug() << "Update requires: " << tc << " tasks";
    return tc;
}

void UpdateEngine::setupCommonPreUpdateTasks() {
}

void UpdateEngine::setupCustomUpdateTasks() {
    UpdateProcess* v0_9_to_1_0 = new UpdateProcess( "0.9", "1.0", this );
    v0_9_to_1_0->addTask(
#ifdef Q_OS_MAC
        new ConfigFileTask_0_9_to_1_0( _originalPackageFileName )
#else
        new ConfigFileTask_0_9_to_1_0( _updateFileName )
#endif
    );
    _updateManager->addProcess( v0_9_to_1_0 );
}


void UpdateEngine::setupCommonPostUpdateTasks() {
    _commonPostUpdate->addTask( new UpdateHistoryUpdateTask( _fromVersion, _toVersion, _updateFileName ) );
#if defined(Q_OS_MAC)
    _commonPostUpdate->addTask( new MacOSXInstallerTask( _originalPackageFileName ) );
#else
    _commonPostUpdate->addTask( new Win32InstallerTask( _updateFileName, _originalPackageFileName ) );
#endif
}

void UpdateEngine::runTasks() {
    _commonPreUpdate->execute();
    _updateManager->execute( new PlayerVersion( _fromVersion, this ), new PlayerVersion( _toVersion, this ) );
    _commonPostUpdate->execute();
    emit updateMessage( tr( "Update finished!" ) );
    QThread::currentThread()->sleep( 2 );
    QApplication::exit();
}



