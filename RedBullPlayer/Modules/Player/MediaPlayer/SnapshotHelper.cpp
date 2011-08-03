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
#include "SnapshotHelper.h"

// Qt Includes
#include <QDir>
#include <QFileInfo>
#include <QDebug>

// Project Includes
#include "../../../EventAggregator/Aggregator.h"
#include "../../../Events/EventTypes.h"
#include "../../../Events/SnapshotCaptureEnabledEvent.h"
#include "../../../Interfaces/MediaLibrary/IMedia.h"
#include "../../../ToolLib/SimpleLogger.h"
#include "../../../ToolLib/ConfigManager.h"
#include "../../../Container/Constants.h"
#include "../../../Container/Container.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::Player;

SnapshotHelper::SnapshotHelper( PlayerModel* playerModel,
                                MediaPlayerHandler* mediaPlayerHandler ) {
    _playerModel = playerModel;
    _mediaPlayerHandler = mediaPlayerHandler;
    Aggregator::instance()->registerListener( PlayerStateChanged, this );
}

SnapshotHelper::~SnapshotHelper() {
    Aggregator::instance()->unregisterListener( this );
}

bool SnapshotHelper::event ( QEvent * e ) {
    if( e->type() == PlayerStateChanged ) {
        onPlayerStateChanged( static_cast<PlayerStateChangedEvent*>( e ) );
    }

    return false;
}

void SnapshotHelper::takeSnapshot( Error *error ) {
    QString destDir = ConfigManager::instance()->snapshotDestDir();
    QDir dir( destDir );

    if( !dir.exists() ) {
        if( !dir.mkpath( dir.absolutePath() ) ) {
            error->setErrorMessage( tr( QString( "Directory " + dir.absolutePath() + " does not exist and it cannot be created!" ).toUtf8().constData() ), Error::Normal );
            return;
        }
    }

    QFileInfo fileInfo( dir.absolutePath() );

    if( !fileInfo.isWritable() ) {
        bool success = SnapshotHelper::setAllPermissions( dir.absolutePath() );

        if( !success ) {
            error->setErrorMessage( tr( QString( "Directory " + dir.absolutePath() + " cannot be set to writable!" ).toUtf8().constData() ), Error::Normal );
        }
    }

    QString genericFileName = ConfigManager::instance()->genericFileName();
    QString fileExt = ConfigManager::instance()->fileExt();
    int i = 0;
    QString fileName = genericFileName + QString( "%1" ).arg( i ) + fileExt;
    QFileInfo destFile( dir, fileName );

    while( QFile( destFile.absoluteFilePath() ).exists() ) {
        i++;
        fileName = genericFileName + QString( "%1" ).arg( i ) + fileExt;
        destFile = QFileInfo( dir, fileName );
    }

    takeSnapshot( destFile.absoluteFilePath(), error );
}

void SnapshotHelper::takeSnapshot( QString fileName, Error *error, unsigned int width/* = 0*/, unsigned int height/* = 0*/ ) {
    _mediaPlayerHandler->takeSnapshot( fileName, width, height );

    if ( _mediaPlayerHandler->hasError() ) {
        error->setErrorMessage( _mediaPlayerHandler->lastError()->errorMessage() );
    }
}

void SnapshotHelper::onPlayerStateChanged( PlayerStateChangedEvent *e ) {
    SnapshotCaptureEnabledEvent snapshotEvent;

    switch( e->state() ) {
        case MediaPlayerState::PLAYING:
        case MediaPlayerState::PAUSED:
            if ( ( _playerModel->mediaType() != PlayerModel::UNKOWN ) &&
                    ( ( _playerModel->mediaType() == PlayerModel::STREAM ) ||
                      ( _playerModel->mediaType() == PlayerModel::VIDEO ) ) ) {
                snapshotEvent.setEnabled( true );
            } else {
                snapshotEvent.setEnabled( false );
            }

            break;

        default:
            snapshotEvent.setEnabled( false );
    }

    Aggregator::instance()->sendEvent( &snapshotEvent );
}

bool SnapshotHelper::setAllPermissions( QString fileName ) {
    QFile f( fileName );
    QFlags<QFile::Permission> newPerms;
    newPerms = QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
               QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup |
               QFile::ReadUser | QFile::WriteUser | QFile::ExeUser |
               QFile::ReadOther | QFile::WriteOther | QFile::ExeOther
               ;
    bool retVal = f.setPermissions( newPerms );
    f.close();
    return retVal;
}
