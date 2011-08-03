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
#include "InputParser.h"

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

InputParser::InputParser( QObject* parent ) : QObject( parent ) {
    /*SimpleLogger* sl = new SimpleLogger( this );
    SimpleLogger::_instance = sl;*/
}

void InputParser::run() {
    SimpleLogger::instance()->debug( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "Starting..." ) );
    _se = new SnapshotEngine( this );
    connect ( _se, SIGNAL( snapshotCreated( QString, QString, float ) ),
              this, SLOT( snapShotCreated( QString, QString, float ) ) );
    connect ( _se, SIGNAL( snapshotCreationFaild( QString ) ),
              this, SLOT( snapShotCreationFailed( QString ) ) );
    connect ( _se, SIGNAL( metaDataReceived( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ),
              this, SLOT( metaDataReceived( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString ) ) );
    readNextCommand();
}

void InputParser::writeToConsole( QString msg ) {
    QFile file;
    file.open( stdout, QIODevice::WriteOnly );
    QTextStream consoleOut( &file ); //direct to stdout
    consoleOut << msg << endl;
    file.flush();
    file.close();
}

void InputParser::sendReadyForInput() {
    SimpleLogger::instance()->debug( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "sendReadyForInput: <doc><%1>%2</%1></doc>" )
                                     .arg( Constants::SNAPSHOT_CRETATION_STATE_ELEMENT )
                                     .arg( Constants::SNAPSHOT_CRETATION_STATE_READY_VALUE )
                                   );
    writeToConsole( QString( "<doc><%1>%2</%1></doc>" )
                    .arg( Constants::SNAPSHOT_CRETATION_STATE_ELEMENT )
                    .arg( Constants::SNAPSHOT_CRETATION_STATE_READY_VALUE )
                  );
}

void InputParser::sendBusy() {
    SimpleLogger::instance()->debug( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "sendBusy: <doc><%1>%2</%1></doc>" )
                                     .arg( Constants::SNAPSHOT_CRETATION_STATE_ELEMENT )
                                     .arg( Constants::SNAPSHOT_CRETATION_STATE_BUSY_VALUE )
                                   );
    writeToConsole( QString( "<doc><%1>%2</%1></doc>" )
                    .arg( Constants::SNAPSHOT_CRETATION_STATE_ELEMENT )
                    .arg( Constants::SNAPSHOT_CRETATION_STATE_BUSY_VALUE )
                  );
}

void InputParser::sendError( QString msg ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "sendError: <doc><%1>%2</%1></doc>" )
                                     .arg( Constants::SNAPSHOT_CRETATION_ERROR_ELEMENT )
                                     .arg( msg.simplified() )
                                   );
    writeToConsole( QString( "<doc><%1>%2</%1></doc>" )
                    .arg( Constants::SNAPSHOT_CRETATION_ERROR_ELEMENT )
                    .arg( msg.simplified() )
                  );
}

void InputParser::snapShotCreationFailed( QString error ) {
    sendError( error );
}

void InputParser::readNextCommand() {
    sendReadyForInput();
    QTextStream stream( stdin );
    QString line = stream.readLine();
    WorkState sw = parseLine( line );

    if ( sw == InvalidCommand ) {
        readNextCommand();
    }
}


InputParser::WorkState InputParser::parseLine( QString line ) {
    if ( line.isNull() ) {
        return InvalidCommand;
    }

    SimpleLogger::instance()->debug( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR, QString( "Parsing: %1 " )
                                     .arg( line ) );
    QStringList parts = line.split( '|', QString::SkipEmptyParts );

    if ( parts.count() == 0 ) {
        return InvalidCommand;
    }

    QString command = parts.at( 0 );

    if ( command == "KILL" ) {
        writeToConsole( "Recieved KILL command" );
        SimpleLogger::instance()->info( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR, QString( "Recieved KILL command" ) );
        QCoreApplication::exit( 0 );
        exit( 0 );
        return InvalidCommand;
    }

    if ( command == "SNAPSHOT" ) {
        if ( parts.count() != 4 ) {
            SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "Invalid Command: %1" )
                                            .arg( line )
                                          );
            sendError( "Invalid Command " + line );
        }

        QUuid requestId =  parts.at( 1 );
        QString fileName = parts.at( 2 );

        if ( ! QFile::exists( fileName ) ) {
            qWarning() << "File " << fileName << " does not exist!";
            sendError( "File " + fileName + " does not exist!" );
            return InvalidCommand;
        }

        bool ok;
        float position = parts.at( 3 ).toFloat( &ok );

        if ( ! ok ) {
            qWarning() << "Invalid postion string: " << parts.at( 3 );
            sendError( "Invalid postion string: " + parts.at( 3 ) + "!" );
            return InvalidCommand;
        }

        if ( _se->readyForNewSnapshot() ) {
            _se->requestNewSnapshot( requestId, fileName, position );
            sendBusy();
            return StartedWork;
        }

        return StillBusy;
    }

    if ( command == "METADATA" ) {
        if ( parts.count() != 3 ) {
            qWarning() << "Invalid Command " << line;
            sendError( "Invalid Command " + line );
            return InvalidCommand;
        }

        QUuid requestId =  parts.at( 1 );
        QString fileName = parts.at( 2 );

        if ( ! QFile::exists( fileName ) ) {
            qWarning() << "File " << fileName << " does not exist!";
            sendError( "File " + fileName + " does not exist!" );
            return InvalidCommand;
        }

        if ( _se->readyForNewSnapshot() ) {
            _se->requestMetaData( requestId, fileName );
            sendBusy();
            return StartedWork;
        }

        return StillBusy;
    }

    return InvalidCommand;
}

void InputParser::snapShotCreated( QString requestId, QString fileName, float position ) {
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
    readNextCommand();
    // snCreator->deleteLater();
}

void InputParser::metaDataReceived( QString requestId,
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
    readNextCommand();
    qWarning() << Q_FUNC_INFO;
}






