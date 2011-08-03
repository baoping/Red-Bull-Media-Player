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
#include "SnapshotRequestHandler.h"

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QMutexLocker>
#include <QThread>

// QTVLC includes
#include "../../../QTVLC/src/Factory/MediaPlayerFactory.h"
#include "../../../QTVLC/src/Interfaces/IMediaPlayer.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::SnapshotCreator;
using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Container;

SnapshotRequestHandler::SnapshotRequestHandler( RequestMetaData* data, QObject *parent ) :
    QObject( parent ) {
    _positionChangedTimeOutTimer = new QTimer( this );
    _positionChangedTimeOutTimer->setSingleShot( true );
    _positionChangedTimeOutTimer->setInterval( 3000 );
    connect( _positionChangedTimeOutTimer, SIGNAL( timeout() ), this, SLOT( positionChangedTimeOut() ) );
    _timeChangedTimeOutTimer = new QTimer( this );
    _timeChangedTimeOutTimer->setSingleShot( true );
    _timeChangedTimeOutTimer->setInterval( 3000 );
    connect( _timeChangedTimeOutTimer, SIGNAL( timeout() ), this, SLOT( timeChangedTimeOut() ) );
    _mediaPlayer = MediaPlayerFactory::instance()->createMediaPlayerForSnapshot( this );
    connect( _mediaPlayer, SIGNAL( playing() ), this, SLOT( playerPlaying() ) );
    connect( _mediaPlayer, SIGNAL( encounteredError() ), this, SLOT( playerError() ) );
    _currentRequest = data;
}

SnapshotRequestHandler::~SnapshotRequestHandler() {
    _positionChangedTimeOutTimer->stop();
    _timeChangedTimeOutTimer->stop();
    disconnect( _mediaPlayer, SIGNAL( playing() ), this, SLOT( playerPlaying() ) );
    disconnect( _mediaPlayer, SIGNAL( encounteredError() ), this, SLOT( playerError() ) );
    disconnect( _mediaPlayer, SIGNAL( positionChanged( float ) ), this, SLOT( playerPositionChanged( float ) ) );
    disconnect( _mediaPlayer, SIGNAL( timeChanged( qint64 ) ), this, SLOT( playerTimeChanged( qint64 ) ) );
    delete _mediaPlayer;
}

void SnapshotRequestHandler::requestSnapshot() {
    SimpleLogger::instance()->debug( Constants::LOGGER_SNAPSHOTCREATOR,
                                     QString( "RequestSnapshot for:%1" )
                                     .arg( _currentRequest->mediaFileName() ) );
    _posChangedCalled = false;
    _timeChangedCalled = false;
    _mediaPlayer->setMediaLocation( _currentRequest->mediaFileName() );
    _mediaPlayer->play();
}

void SnapshotRequestHandler::playerPlaying() {
    _positionChangedTimeOutTimer->start();
    connect( _mediaPlayer, SIGNAL( positionChanged( float ) ), this, SLOT( playerPositionChanged( float ) ) );
    SimpleLogger::instance()->debug( Constants::LOGGER_SNAPSHOTCREATOR,
                                     QString( "PlayerPlaying file: %1 set to position: %2" )
                                     .arg( _currentRequest->mediaFileName() )
                                     .arg( _currentRequest->position() ) );
    _mediaPlayer->setPosition( _currentRequest->position() );
}

void SnapshotRequestHandler::playerPositionChanged( float position ) {
    disconnect( _mediaPlayer, SIGNAL( positionChanged( float ) ), this, SLOT( playerPositionChanged( float ) ) );

    if ( !_posChangedCalled ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_SNAPSHOTCREATOR,
                                         QString( "DoTheSnapshot for file: %1" )
                                         .arg( _currentRequest->mediaFileName() ) );
        _posChangedCalled = true;
        doTheSnapshot();
    } else {
        SimpleLogger::instance()->debug( Constants::LOGGER_SNAPSHOTCREATOR,
                                         QString( "something wrong" ) );
    }
}

void SnapshotRequestHandler::playerTimeChanged( qint64 time ) {
    disconnect( _mediaPlayer, SIGNAL( timeChanged( qint64 ) ), this, SLOT( playerTimeChanged( qint64 ) ) );

    if ( !_timeChangedCalled ) {
        _timeChangedCalled = true;
        doTheSnapshot();
    }
}

void SnapshotRequestHandler::doTheSnapshot() {
    _positionChangedTimeOutTimer->stop();
    _timeChangedTimeOutTimer->stop();
    _mediaPlayer->pause();
    QDir tempDir( ConfigManager::instance()->getTempDirectory() );
    QString fileName = QString( "%1.png" ).arg( _currentRequest->id().toString() );
    SimpleLogger::instance()->debug( Constants::LOGGER_SNAPSHOTCREATOR,
                                     QString( "doTheSnapshot...fileName: %1" )
                                     .arg( fileName ) );
    QFileInfo fileInfo( tempDir, fileName );

    if ( QFile::exists( fileInfo.absoluteFilePath() ) )
        QFile::remove( fileInfo.absoluteFilePath() );

    _mediaPlayer->takeSnapshot( fileInfo.absoluteFilePath() );
    emit snapshotFinished( _currentRequest->id(), fileInfo.absoluteFilePath() );
}

void SnapshotRequestHandler::snapshotTaken( QString fileName ) {
}


void SnapshotRequestHandler::emitSnapshotFailed() {
    emit snapshotFailed( _currentRequest->id() );
}

void SnapshotRequestHandler::positionChangedTimeOut() {
    disconnect( _mediaPlayer, SIGNAL( positionChanged( float ) ), this, SLOT( playerPositionChanged( float ) ) );
    connect( _mediaPlayer, SIGNAL( timeChanged( qint64 ) ), this, SLOT( playerTimeChanged( qint64 ) ) );
    _timeChangedTimeOutTimer->start();
    _mediaPlayer->setTime( _currentRequest->time()  );
}

void SnapshotRequestHandler::timeChangedTimeOut() {
    emitSnapshotFailed();
}

void SnapshotRequestHandler::playerError() {
    SimpleLogger::instance()->debug( Constants::LOGGER_SNAPSHOTCREATOR,
                                     QString( "player error" ) );
    emitSnapshotFailed();
}
