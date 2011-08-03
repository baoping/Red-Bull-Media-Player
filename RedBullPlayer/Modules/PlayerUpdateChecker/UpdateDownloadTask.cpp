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
#include "UpdateDownloadTask.h"

#include <QEventLoop>
#include <QFile>
#include <QDebug>

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::PlayerUpdateChecker;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Tools::Network;
using namespace RedBullPlayer::Container;

UpdateDownloadTask::UpdateDownloadTask( QString name, QObject* parent ) : Task( TaskPurpose::UPDATE, name, parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER,
                                     QString( "Create new UpdateDownloadTask. Name: %1 Guid: %2" )
                                     .arg( name )
                                     .arg( guid().toString() ) );
    _downloader = 0;
}

UpdateDownloadTask::~UpdateDownloadTask() {
    if ( _downloader != 0 )
        delete _downloader;
}

void UpdateDownloadTask::prepare() {
    Q_ASSERT( _downloadUrl.isValid() );
    Q_ASSERT( !_targetDirectory.isEmpty() );
    _downloader = new DownloadUtil( 0 );
    _downloader->setTargetDirectory( _targetDirectory );
    _downloader->setLoggerName( Constants::LOGGER_PLAYERUPDATECHECKER );
    emit prepared( true );
}

void UpdateDownloadTask::execute() {
    if ( _downloadUrl.isValid() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYERUPDATECHECKER,
                                         QString( "UpdateDownloadTask: Download: %1" )
                                         .arg( _downloadUrl.toString() ) );
        connect( _downloader, SIGNAL( downloadFinished( QString, QString, QString ) ), this, SLOT( downloadFinished( QString, QString, QString ) ) );
        connect( _downloader, SIGNAL( downloadFailed( QString, int, QString ) ), this, SLOT( downloadFailed( QString, int, QString ) ) );
        connect( _downloader, SIGNAL( progress( int ) ), this, SLOT( progress( int ) ) );
        _downloader->downloadFile( _downloadUrl );
    } else {
        emit finished( false );
    }
}

void UpdateDownloadTask::downloadFinished( QString fileName, QString contentType, QString contentLength ) {
    _localFileName = fileName;
    emit finished( true );
}

void UpdateDownloadTask::downloadFailed( QString fileName, int httpStatusCode, QString httpReasonPhrase ) {
    emit finished( false );
}

void UpdateDownloadTask::progress( int progress ) {
    emit downloadProgress( this->guid(), progress );
}


void UpdateDownloadTask::clean() {
    if ( state()->isEqual( TaskState::CANCELED ) ) {
        if ( QFile::exists( _downloader->fileName() ) )
            QFile::remove( _downloader->fileName() );
    }
}

void UpdateDownloadTask::cancel() {
    if ( _downloader != 0 ) {
        _downloader->cancelDownload();
    }

    Task::cancel();
}
