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
#include "CommandProcessor.h"

// Qt includes
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QTextStream>
#include <QDataStream>
#include <QStringList>
#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include <cstring>
#include <iostream>
#include <cstring>


// Project includes
#include "../Container/Constants.h"
#include "../../QTVLC/src/Factory/MediaPlayerFactory.h"
#include "../ToolLib/SimpleLogger.h"

using namespace std;
using namespace RedBullPlayer::SnapshotCreator;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;


CommandProcessor::CommandProcessor( QObject *parent ) :
    QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "Starting..." ) );
    _se = new SnapshotEngine( this );
    connect ( _se, SIGNAL( snapshotCreated( QString, QString, float ) ),
              this, SLOT( snapShotCreated( QString, QString, float ) ) );
    connect ( _se, SIGNAL( snapshotCreationFaild( QString ) ),
              this, SLOT( snapShotCreationFailed( QString ) ) );
    connect ( _se, SIGNAL( metaDataReceived( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ),
              this, SLOT( metaDataReceived( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ) );
}

void CommandProcessor::requestSnapshot( SnapshotCreationData* data ) {
    qDebug() << Q_FUNC_INFO;
    _se->requestNewSnapshot( data->requestId(), data->movieFileName(), data->position() );
}


void CommandProcessor::writeToConsole( QString msg ) {
    QFile file;
    file.open( stdout, QIODevice::WriteOnly );
    QTextStream consoleOut( &file ); //direct to stdout
    consoleOut << msg << endl;
    file.flush();
    file.close();
}

void CommandProcessor::sendError( QString msg ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "sendError: <doc><%1>%2</%1></doc>" )
                                     .arg( Constants::SNAPSHOT_CRETATION_ERROR_ELEMENT )
                                     .arg( msg.simplified() )
                                   );
    writeToConsole( QString( "<doc><%1>%2</%1></doc>" )
                    .arg( Constants::SNAPSHOT_CRETATION_ERROR_ELEMENT )
                    .arg( msg.simplified() )
                  );
}

void CommandProcessor::snapShotCreationFailed( QString error ) {
    sendError( error );
    QCoreApplication::quit();
}

void CommandProcessor::snapShotCreated( QString requestId, QString fileName, float position ) {
    qWarning() << Q_FUNC_INFO;
    SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "%1" )
                                    .arg( Q_FUNC_INFO )
                                  );
    writeToConsole(
        QString( "<doc><%1 %2=\"%4\" %3=\"%5\" /></doc>" )
        .arg( Constants::SNAPSHOT_CRETATION_SNAPSHOTCREATED_ELEMENT )
        .arg( Constants::SNAPSHOT_CRETATION_REQUESTID_ATTRIBUTE )
        .arg( Constants::SNAPSHOT_CRETATION_FILENAME_ATTRIBUTE )
        .arg( requestId )
        .arg( fileName ) );
    QCoreApplication::quit();
    // snCreator->deleteLater();
}

void CommandProcessor::metaDataReceived( QString requestId,
        QString title,
        QString artist,
        QString genre,
        QString copyright,
        QString album,
        QString trackNumber,
        QString description,
        QString rating,
        QString date,
        QString setting,
        QString url,
        QString language,
        QString nowPlaying,
        QString publisher,
        QString encodedBy,
        QString artworkUrl,
        QString trackID ) {
    SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "%1" )
                                    .arg( Q_FUNC_INFO )
                                  );
    QDomDocument doc;
    QDomElement root = doc.createElement( "doc" );
    doc.appendChild( root );
    QDomElement curr;
    QDomCDATASection cdata;
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_ELEMENT );
    curr.setAttribute( Constants::SNAPSHOT_CRETATION_REQUESTID_ATTRIBUTE, requestId );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_title_ELEMENT );
    cdata = doc.createCDATASection( title );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_artist_ELEMENT );
    cdata = doc.createCDATASection( artist );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_genre_ELEMENT );
    cdata = doc.createCDATASection( genre );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_copyright_ELEMENT );
    cdata = doc.createCDATASection( copyright );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_album_ELEMENT );
    cdata = doc.createCDATASection( album );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_trackNumber_ELEMENT );
    cdata = doc.createCDATASection( trackNumber );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_description_ELEMENT );
    cdata = doc.createCDATASection( description );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_rating_ELEMENT );
    cdata = doc.createCDATASection( rating );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_date_ELEMENT );
    cdata = doc.createCDATASection( date );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_setting_ELEMENT );
    cdata = doc.createCDATASection( setting );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_url_ELEMENT );
    cdata = doc.createCDATASection( url );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_language_ELEMENT );
    cdata = doc.createCDATASection( language );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_nowPlaying_ELEMENT );
    cdata = doc.createCDATASection( nowPlaying );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_publisher_ELEMENT );
    cdata = doc.createCDATASection( publisher );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_encodedBy_ELEMENT );
    cdata = doc.createCDATASection( encodedBy );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_artworkUrl_ELEMENT );
    cdata = doc.createCDATASection( artworkUrl );
    curr.appendChild( cdata );
    root.appendChild( curr );
    curr = doc.createElement( Constants::METADATA_RECEIVE_METADATARECEIVED_trackID_ELEMENT );
    cdata = doc.createCDATASection( trackID );
    curr.appendChild( cdata );
    root.appendChild( curr );
    writeToConsole( doc.toString( -1 ) );
    qWarning() << Q_FUNC_INFO;
    QCoreApplication::quit();
}







