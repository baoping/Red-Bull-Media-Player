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
#include "PodCastTask.h"

#include <QEventLoop>
#include <QFile>
#include <QDebug>

#include "../../EventAggregator/Aggregator.h"
#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Events/EventTypes.h"
#include "../../Events/TaskManager/TaskProgressEvent.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Tools::Network;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

PodCastTask::PodCastTask( QString name, QObject* parent ) : Task( TaskPurpose::DOWNLOAD, name, parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PODCATCHER,
                                     QString( "Create new PodCastTask. Name: %1 Guid: %2" )
                                     .arg( name )
                                     .arg( guid().toString() ) );
    _imageDownloader = 0;
    _podCastDownloader = 0;
}

PodCastTask::~PodCastTask() {
    if ( _imageDownloader != 0 )
        delete _imageDownloader;

    if ( _podCastDownloader != 0 )
        delete _podCastDownloader;
}

void PodCastTask::prepare() {
    Q_ASSERT( _podCastItemUrl.isValid() );
    Q_ASSERT( !_targetDirectory.isEmpty() );
    _imageDownloader = new DownloadUtil( 0 );
    _podCastDownloader = new DownloadUtil( 0 );
    _imageDownloader->setTargetDirectory( _targetDirectory );
    _imageDownloader->setLoggerName( Constants::LOGGER_PODCATCHER );
    _podCastDownloader->setTargetDirectory( _targetDirectory );
    _podCastDownloader->setLoggerName( Constants::LOGGER_PODCATCHER );
    emit prepared( true );
}

void PodCastTask::execute() {
    if ( _podCastImageUrl.isValid() ) {
        downloadImage();
    } else {
        downloadPodCast();
    }
}

void PodCastTask::downloadImage() {
    connect( _imageDownloader,
             SIGNAL( downloadFinished( QString, QString, QString ) ),
             this,
             SLOT( image_downloadFinished( QString, QString, QString ) ) );
    connect( _imageDownloader,
             SIGNAL( downloadFailed( QString, int, QString ) ),
             this,
             SLOT( image_downloadFailed( QString, int, QString ) ) );
    _imageDownloader->downloadFile( _podCastImageUrl );
}

void PodCastTask::downloadPodCast() {
    connect( _podCastDownloader,
             SIGNAL( downloadFinished( QString, QString, QString ) ),
             this,
             SLOT( podCast_downloadFinished( QString, QString, QString ) ) );
    connect( _podCastDownloader,
             SIGNAL( downloadFailed( QString, int, QString ) ),
             this,
             SLOT( podCast_downloadFailed( QString, int, QString ) ) );
    connect( _podCastDownloader,
             SIGNAL( progress( int ) ),
             this,
             SLOT( progress( int ) ) );
    _podCastDownloader->downloadFile( _podCastItemUrl );
}

void PodCastTask::progress( int progress ) {
    TaskProgressEvent ev( this->guid(), progress );
    Aggregator::instance()->sendEvent( &ev );
}

void PodCastTask::clean() {
    if ( state()->isEqual( TaskState::CANCELED ) ) {
        if ( QFile::exists( _imageDownloader->fileName() ) )
            QFile::remove( _imageDownloader->fileName() );

        if ( QFile::exists( _podCastDownloader->fileName() ) )
            QFile::remove( _podCastDownloader->fileName() );
    }
}

void PodCastTask::cancel() {
    qDebug() << "PodCastTask => Cancel";

    if ( _imageDownloader != 0 ) {
        _imageDownloader->cancelDownload();
    }

    if ( _podCastDownloader != 0 ) {
        _podCastDownloader->cancelDownload();
    }

    setState( TaskState::CANCELED );
    setCanceled( true );
    emit canceled();
}

void PodCastTask::image_downloadFinished( QString fileName, QString contentType, QString contentLength ) {
    _podCastImage = QImage( fileName );
    QFile::remove( fileName );
    downloadPodCast();
}

void PodCastTask::image_downloadFailed( QString fileName, int httpStatusCode, QString httpReasonPhrase ) {
    downloadPodCast();
}

void PodCastTask::podCast_downloadFinished( QString fileName, QString contentType, QString contentLength ) {
    _localFileName = fileName;

    if ( !isCanceled() )
        emit finished( true );
}

void PodCastTask::podCast_downloadFailed( QString fileName, int httpStatusCode, QString httpReasonPhrase ) {
    qDebug() << "PodCastTask => DownloadFailed (" << fileName << ")";
    emit finished( false );
}
