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
#include "NetworkAccessManager.h"

#include <QDebug>

//Project includes
#include "../SimpleLogger.h"
#include "../../Container/Constants.h"
#include "../ConfigManager.h"

using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools::Network;

NetworkAccessManager::NetworkAccessManager( QObject* parent ) : QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB, "NAM: New NetworkAccessManager created." );
    _reply = 0;
    _manager = new QNetworkAccessManager( this );
    connect( _manager,
             SIGNAL( finished( QNetworkReply* ) ),
             SLOT( requestFinished( QNetworkReply* ) ) );
}

void NetworkAccessManager::get( QUrl url ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB, QString( "NAM: Get(%1)" ).arg( url.toString() ) );
    _lastPercent = 0;

    if ( _reply != 0 ) {
        delete _reply;
        _reply = 0;
    }

    _reply = _manager->get( QNetworkRequest( url ) );
    connect( _reply,
             SIGNAL( downloadProgress( qint64, qint64 ) ),
             SLOT( downloadProgress( qint64, qint64 ) ) );
}

void NetworkAccessManager::downloadProgress( qint64 bytesReceived, qint64 bytesTotal ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
                                     QString( "NAM.DownloadProgress: %1/%2" )
                                     .arg( bytesReceived )
                                     .arg( bytesTotal ) );
    int percent = 0;

    if ( bytesTotal > 0 ) {
        percent = ( bytesReceived * 100 ) / bytesTotal;
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
                                     QString( "NAM.DownloadPercent: LP: %1 CP: %2" )
                                     .arg( _lastPercent )
                                     .arg( percent ) );

    if ( ( percent + 5 ) > _lastPercent ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_TOOLLIB,
                                         QString( "NAM.DownloadPercent: emit progress" ) );
        _lastPercent = percent;
        emit progress( _lastPercent );
    }
}

void NetworkAccessManager::abort() {
    if ( _reply != 0 ) {
        _reply->blockSignals( true );
        _reply->disconnect( this );
        _reply->abort();
    }
}

void NetworkAccessManager::requestFinished( QNetworkReply* reply ) {
    int httpStatusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    QString httpReasonPhrase = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
    QUrl requestUrl = reply->request().url();
    QUrl possibleRedirectUrl = reply->attribute( QNetworkRequest::RedirectionTargetAttribute ).toUrl();

    if ( possibleRedirectUrl.isEmpty() && ( httpStatusCode == 200 ) ) {
        QString contentType = reply->header( QNetworkRequest::ContentTypeHeader ).toString();
        QString contentLength = reply->header( QNetworkRequest::ContentLengthHeader ).toString();
        QByteArray rawData = reply->readAll();
        emit finished( requestUrl, rawData, contentType, contentLength );
    } else if ( !possibleRedirectUrl.isEmpty() && ( possibleRedirectUrl != requestUrl ) ) {
        _manager->get( QNetworkRequest( possibleRedirectUrl ) );
    } else {
        emit failed( requestUrl, httpStatusCode, httpReasonPhrase );
    }
}




