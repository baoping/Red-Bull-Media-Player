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
#include "SnapshotEngine.h"


// QT includes
#include <QDebug>
#include <QString>
#include <QUuid>
#include <QFileInfo>
#include <QThread>
#include <QTextStream>
#include <cstring>
#include <iostream>
#include <cstring>

// QTVLC includes
#include "../../QTVLC/src/Exceptions/MediaPlayerException.h"
#include "../../QTVLC/src/Exceptions/Exception.h"
#include "../../QTVLC/src/Exceptions/VLCException.h"
#include "../../QTVLC/src/Interfaces/IMediaMetaData.h"
#include "../../QTVLC/src/Interfaces/IMediaPlayer.h"
#include "../../QTVLC/src/Interfaces/IMediaObject.h"
#include "../../QTVLC/src/Interfaces/MediaPlayerPurpose.h"
#include "../../QTVLC/src/Factory/MediaPlayerFactory.h"
#include "../../QTVLC/src/Exceptions/Exception.h"

// Project includes
#include "../EventAggregator/Aggregator.h"
#include "../Events/EventTypes.h"
#include "../Container/Constants.h"
#include "../ToolLib/SimpleLogger.h"

#include "../../QTVLC/src/Factory/MediaPlayerFactory.h"

#include "InputParser.h"

using namespace RedBullPlayer::SnapshotCreator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

SnapshotEngine::SnapshotEngine( QObject* parent ) : QObject( parent ) {
    _readyForNewSnapshot = true;
    /*
        QObject::connect( _mpInterface,SIGNAL(playing()),
    		      this,SLOT(playing()), Qt::QueuedConnection );
        QObject::connect( _mpInterface,SIGNAL(positionChanged(float)),
    		      this,SLOT(positionChanged(float)), Qt::QueuedConnection );
    */
    Aggregator::instance()->registerListener( MediaPlayerPlaying, this );
    Aggregator::instance()->registerListener( MediaPlayerPositionChanged, this );
    SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "%1" )
                                    .arg( Q_FUNC_INFO )
                                  );
}

SnapshotEngine::~SnapshotEngine() {
    Aggregator::instance()->unregisterListener( this );
}

void SnapshotEngine::requestNewSnapshot( QUuid requestId, QString movieFileName, float position ) {
    qDebug() << Q_FUNC_INFO << " movieFileName: " << movieFileName << " position: " << position;
    SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "%1 movieFileName %2 position %3" )
                                    .arg( Q_FUNC_INFO )
                                    .arg( movieFileName )
                                    .arg( position )
                                  );
    _readyForNewSnapshot = false;
    _requestId = requestId;
    _position = position;
    _requestType = Snapshot;
    QString encodedUrl = QString( movieFileName.toUtf8() );  //for german umlauts
    QFileInfo inf( encodedUrl );
    inf.makeAbsolute();

    if ( _mpInterface ) {
        delete _mpInterface;
    }

    _mpInterface = MediaPlayerFactory::instance()->createMediaPlayer( MediaPlayerPurpose::SNAPSHOT_CREATION, inf.absoluteFilePath(), this );
    _mpInterface->play();
}

void SnapshotEngine::requestMetaData( QUuid requestId, QString movieFileName ) {
    _readyForNewSnapshot = false;
    _requestId = requestId;
    _position = 0.1;
    _requestType = MetaData;
    QString encodedUrl = QString( movieFileName.toUtf8() ); //for german umlauts
    QFileInfo inf( encodedUrl );
    inf.makeAbsolute();

    if ( _mpInterface ) {
        delete _mpInterface;
    }

    _mpInterface = MediaPlayerFactory::instance()->createMediaPlayer( MediaPlayerPurpose::SNAPSHOT_CREATION, inf.absoluteFilePath(), this );
    _mpInterface->play();
}


bool SnapshotEngine::event( QEvent* ev ) {
    if ( ev->type() == MediaPlayerPlaying ) {
        qDebug() << "Recieved MediaPlayerPlaying event!";
        playing();
    }

    if ( ev->type() == MediaPlayerPositionChanged ) {
        qDebug() << "Recieved MediaPlayerPositionChanged event!";
        positionChanged();
    }

    return false;
}

void SnapshotEngine::playing() {
    SimpleLogger::instance()->debug( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "%1" )
                                     .arg( Q_FUNC_INFO )
                                   );

    try {
        SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "Calling set position: %1" )
                                        .arg( _position )
                                      );
        _mpInterface->setPosition( _position );
        SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "Calling set position finished" )
                                      );
    } catch ( VLCException* ex ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "VLCException: %1" )
                                        .arg( ex->message() )
                                      );
    } catch ( MediaPlayerException* ex ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "MediaPLayerException: %1" )
                                        .arg( ex->message() )
                                      );
    } catch ( ... ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "An unexpected Error occoured" )
                                      );
    }
}

void SnapshotEngine::positionChanged() {
    SimpleLogger::instance()->warn( Constants::LOGGER_EXTERNALSNAPSHOTCREATOR , QString( "%1" )
                                    .arg( Q_FUNC_INFO )
                                  );

    if ( _requestType == Snapshot ) {
        QString fn = QString( "%1.png" ).arg( _requestId );
        _mpInterface->takeSnapshot( fn );
        _mpInterface->pause();
        _readyForNewSnapshot = true;
        emit snapshotCreated( _requestId.toString(), fn, _position );
    }

    if ( _requestType == MetaData ) {
        IMediaMetaData* md = _mpInterface->mediaObject()->metaData();
        _mpInterface->pause();
        _readyForNewSnapshot = true;
        emit metaDataReceived( _requestId.toString(),
                               md->title(),
                               md-> artist(),
                               md-> genre(),
                               md-> copyright(),
                               md-> album(),
                               md-> trackNumber(),
                               md-> description(),
                               md-> rating(),
                               md-> date(),
                               md-> setting(),
                               md-> url(),
                               md-> language(),
                               md-> nowPlaying(),
                               md-> publisher(),
                               md-> encodedBy(),
                               md-> artworkUrl(),
                               md-> trackID() );
    }
}
