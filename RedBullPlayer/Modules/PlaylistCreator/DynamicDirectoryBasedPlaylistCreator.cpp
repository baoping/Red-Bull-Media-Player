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
#include "DynamicDirectoryBasedPlaylistCreator.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QUuid>
#include <QFileInfo>
#include <QDirIterator>
#include <QThread>

// Project Includes
#include "../../Container/Constants.h"
#include "../../Container/Error.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SleeperThread.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Interfaces/MediaLibrary/ISearchEngine.h"
#include "../../Events/Playlist/PlaylistCreatedEvent.h"
#include "../../Events/MediaLibrary/ChapterImageRequestEvent.h"
#include "../../Events/EventTypes.h"
#include "../MediaLibrary/Media.h"
#include "../MediaLibrary/MediaType.h"
#include "DummyPlaylist.h"
#include "DummyPlaylistItem.h"
#include "DummyChapter.h"
#include "PlaylistItemEventHandler.h"

using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

DynamicDirectoryBasedPlaylistCreator::DynamicDirectoryBasedPlaylistCreator( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent ) : QObject( parent ) {
    Q_ASSERT( mediaLibrary != 0 );
    _mediaLibrary = mediaLibrary;
    _mediaDirectories.clear();
    _deleteDir = "";
}


void DynamicDirectoryBasedPlaylistCreator::createPlaylistFromBaseDirectory() {
    // Wait 1s to enshure that all other modules are initialized before we fire any events!
    SleeperThread::msleep(1000);

    emit startupPlaylistGenerationStarted();
    initBaseDir();

    if ( _mediaDirectories.count() == 0 ) {
        emit startupPlaylistGenerationFinished();
        return;
    } else {
        QDir directory( _mediaDirectories.at( 0 ) );

        if ( ! directory.exists() ) {
            emit startupPlaylistGenerationFinished();
            return;
        }
    }

    QList<QPair<QUuid, QString>*>* discoverdMedia = readMediaFromMediaLibrary();
    readNewMediaFromFS( discoverdMedia );
    filterVanishedMedia( discoverdMedia );

    while( !discoverdMedia->empty() ) {
        delete discoverdMedia->takeFirst();
    }

    delete discoverdMedia;
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Playlist Creation finished" ) );
    emit startupPlaylistGenerationFinished();
    emit playlistGenerationFinished();
}


QList<QPair<QUuid, QString>*>* DynamicDirectoryBasedPlaylistCreator::readMediaFromMediaLibrary() {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );
    QList<QPair<QUuid, QString>*>* discoveredMedia = _mediaLibrary->mediaDB()->mediaRepository()->allMediaInUIOrder();
    int i;

    for( i = 0; i < discoveredMedia->count(); i++ ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Send from DB discovered Media event %1" )
                                         .arg( discoveredMedia->at( i )->second ) );
        emit mediaDiscovered( discoveredMedia->at( i )->first , 0 );
    }

    return discoveredMedia;
}

void DynamicDirectoryBasedPlaylistCreator::readNewMediaFromFS( QList<QPair<QUuid, QString>*>* discoveredMedia ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );

    for( int i = 0; i < _mediaDirectories.count(); i++ ) {
        QString dir = _mediaDirectories.at( i );
        QDir directory( dir );
        QFileInfoList list = directory.entryInfoList( QDir::NoFilter, QDir::Time | QDir::Reversed );

        for( int fileInFsIndex = 0; fileInFsIndex < list.length(); fileInFsIndex++ ) {
            QFileInfo file = list.at( fileInFsIndex );

            if( !file.isFile() ) {
                continue;
            }

            if ( ! findInPairList( file.absoluteFilePath(), discoveredMedia ) ) {
                processNewMediaFile( file, false );
            }
        }
    }
}

bool DynamicDirectoryBasedPlaylistCreator::findInPairList( QString searchText, QList<QPair<QUuid, QString>*>* list ) {
    bool found = false;
    QListIterator<QPair<QUuid, QString>*> i( *list );

    while( i.hasNext() ) {
        QPair<QUuid, QString>* p = i.next();

        if ( p->second == searchText ) {
            found = true;
            break;
        }
    }

    return found;
}


void DynamicDirectoryBasedPlaylistCreator::processNewMediaFile( QFileInfo file, bool userAdded ) {
    PlaylistItemEventHandler::processNewMediaFile( _mediaLibrary, file, userAdded );
    SleeperThread::msleep(100);
}

void DynamicDirectoryBasedPlaylistCreator::filterVanishedMedia( QList<QPair<QUuid, QString>*>* discoveredMedia ) {
    int i;

    for( i = 0; i < discoveredMedia->count(); i++ ) {
        if ( ! QFile::exists( discoveredMedia->at( i )->second ) ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Send Media Vanished event %1" )
                                             .arg( discoveredMedia->at( i )->second ) );
            emit mediaVanished( discoveredMedia->at( i )->first );
            Error err;
            IMedia* curr = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( discoveredMedia->at( i )->first );
            _mediaLibrary->mediaDB()->mediaRepository()->deleteMedia( *curr, &err );
            Error::raise( &err );
            delete curr;
        }
    }
}

bool DynamicDirectoryBasedPlaylistCreator::isInWhiteList( QString whiteListConfigUrl, QFileInfo file ) {
    return ConfigManager::instance()->isInWhiteList( file );
}

void DynamicDirectoryBasedPlaylistCreator::initBaseDir() {
    _mediaDirectories.clear();

    for( int i = 0; i < ConfigManager::instance()->mediaDirectories().count(); i++ ) {
        QString dirString = ConfigManager::instance()->mediaDirectories().at( i );
        QDir directory( dirString );

        if ( !_mediaDirectories.contains( directory.absolutePath() ) ) {
            _mediaDirectories.append( directory.absolutePath() );
        }

        QDirIterator it( dirString, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories );

        while ( it.hasNext() ) {
            QString subDirString = it.next();
            QDir subDir( subDirString );

            if ( !_mediaDirectories.contains( subDir.absolutePath() ) ) {
                _mediaDirectories.append( subDir.absolutePath() );
            }
        }
    }
}

void DynamicDirectoryBasedPlaylistCreator::initDeleteDir() {
    if( _deleteDir.length() == 0 ) {
        _deleteDir = ConfigManager::instance()->getDeleteDirectory();
    }
}

void DynamicDirectoryBasedPlaylistCreator::mkDirIfNotExists( QString dir ) {
    QDir directory = QDir( dir );

    if( !directory.exists() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Creating dir %1" ).arg( dir ) );
        bool success = directory.mkpath( directory.absolutePath() );

        if( !success ) {
            Error err;
            err.setErrorMessage( QString( "Cannot create directory '%1'" ).arg( directory.absolutePath() ) );
            Error::raise( &err );
        } else {
            SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, QString( "Dir %1 successfully created" ).arg( directory.absolutePath() ) );
        }
    }
}




