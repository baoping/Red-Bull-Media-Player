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
#include "DownloadUtil.h"

//Qt includes
#include <QList>
#include <QFile>
#include <QDebug>

//Project includes
#include "../../Container/Constants.h"
#include "../SimpleLogger.h"
#include "../ConfigManager.h"
#include "../helper.h"


using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools::Network;

DownloadUtil::DownloadUtil( QObject* parent ) : QObject( parent ) {
    QDir dir( ConfigManager::instance()->getTempDirectory() );
    _targetDirectory = dir.absolutePath();
    bootstrap();
}

DownloadUtil::~DownloadUtil() {
    if ( _nam )
        _nam->deleteLater();
}

void DownloadUtil::bootstrap() {
    _loggerName = Constants::LOGGER_TOOLLIB;  //defaultLogger
    _nam = new NetworkAccessManager( this );
    connect( _nam, SIGNAL( finished( QUrl, QByteArray, QString, QString ) ), this, SLOT( namFinished( QUrl, QByteArray, QString, QString ) ) );
    connect( _nam, SIGNAL( failed( QUrl, int, QString ) ), this, SLOT( namFailed( QUrl, int, QString ) ) );
    connect( _nam, SIGNAL( progress( int ) ), this, SIGNAL( progress( int ) ) );
}

void DownloadUtil::cancelDownload() {
    _nam->abort();
}

void DownloadUtil::namFinished( QUrl requestUrl, QByteArray data, QString contentType, QString contentLength ) {
    _fileName = QFileInfo( requestUrl.path() ).fileName();
    _requestUrl = requestUrl;
    _contentType = contentType;
    _contentLength = contentLength;
    _httpStatusCode = 200;
    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
                                     QString( "DownloadUtil: Save %1 to %2." )
                                     .arg( _fileName )
                                     .arg( _targetDirectory ) );
    //create directory if not exists
    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
                                     QString( "DownloadUtil: Create target directory if not exists." ) );
    Helper::mkDirIfNotExists( _targetDirectory );
    QFileInfo fileInfo( QDir( _targetDirectory ), _fileName );
    _fileName = fileInfo.absoluteFilePath();
    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
                                     QString( "DownloadUtil: Delete old file if exists." ) );

    //remov existing file
    if ( QFile::exists( _fileName ) ) {
        QFile::remove( _fileName );
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
                                     QString( "DownloadUtil: Save file." ) );
    QFile file( fileInfo.absoluteFilePath() );

    if ( file.open( QIODevice::WriteOnly ) ) {
        file.write( data );
        file.close();
        SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
                                         QString( "DownloadUtil: Emit downloadFinished" ) );
        emit downloadFinished( _fileName, contentType, contentLength );
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
                                         QString( "DownloadUtil: Emit downloadFailed" ) );
        emit downloadFailed( _fileName, -1, "Couldn't write file." );
    }
}

void DownloadUtil::namFailed( QUrl requestUrl, int httpStatusCode, QString httpReasonPhrase ) {
    _fileName = QFileInfo( requestUrl.path() ).fileName();
    QDir dir( _targetDirectory );
    QFileInfo fileInfo( dir, _fileName );
    _fileName = fileInfo.absoluteFilePath();
    _httpStatusCode = httpStatusCode;
    _httpReasonPhrase = httpReasonPhrase;
    emit downloadFailed( fileInfo.absoluteFilePath(), httpStatusCode, httpReasonPhrase );
}

void DownloadUtil::downloadFile( QUrl url ) {
    _nam->get( Helper::decodeUrl( url.toString() ) );
}

void DownloadUtil::setTargetDirectory( QString directory ) {
    QDir dir( directory );
    _targetDirectory = dir.absolutePath();
}
