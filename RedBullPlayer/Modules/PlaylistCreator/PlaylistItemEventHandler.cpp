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
#include "PlaylistItemEventHandler.h"

#include <QDir>
#include <QEvent>
#include <QDebug>
#include <QMutexLocker>

#include "../../Container/Constants.h"
#include "../../Container/Error.h"
#include "../../Container/Container.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../Events/Playlist/MediaVanishedEvent.h"
#include "../../Events/Playlist/MediaRemovedEvent.h"
#include "../../Events/Playlist/MediaItemContentChangedEvent.h"
#include "../../Events/TaskManager/TaskExecuteRequestEvent.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../ToolLib/TaskManager/FileTask.h"
#include "../../ToolLib/FileSystem/PathResolver.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"

using namespace RedBullPlayer::Modules::PlaylistCreator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::TaskManager;

QMutex* PlaylistItemEventHandler::_processNewMediaFileMutex = new QMutex( QMutex::NonRecursive );

PlaylistItemEventHandler::PlaylistItemEventHandler( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent ) : QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );
    _mediaLibrary = mediaLibrary;
    _copyTasks = new QHash<QUuid, RedBullPlayer::Modules::TaskManager::ITask*>();
    _moveTasks = new QHash<QUuid, RedBullPlayer::Modules::TaskManager::ITask*>();
    Aggregator::instance()->registerListener( PlaylistItemDrop, this );
    Aggregator::instance()->registerListener( PlaylistItemRemoved, this );
    Aggregator::instance()->registerListener( TaskFinished, this );
    Aggregator::instance()->registerListener( TaskCanceled, this );
}

PlaylistItemEventHandler::~PlaylistItemEventHandler() {
    _copyTasks->clear();
    delete _copyTasks;
    _moveTasks->clear();
    delete _moveTasks;
}

bool PlaylistItemEventHandler::event( QEvent *event ) {
    if( event->type() == PlaylistItemDrop ) {
        onPlaylistItemDrop( static_cast<PlaylistItemDropEvent*>( event ) );
    } else if ( event->type() == PlaylistItemRemoved ) {
        onPlaylistItemRemoved( static_cast<PlaylistItemRemovedEvent*>( event ) );
    } else if ( event->type() == TaskFinished ) {
        onTaskExecuteResponse( static_cast<TaskFinishedEvent*>( event ) );
    } else if ( event->type() == TaskCanceled ) {
        onTaskCancelResponse( static_cast<TaskCanceledEvent*>( event ) );
    }

    return false;
}

void PlaylistItemEventHandler::onPlaylistItemDrop( RedBullPlayer::Events::PlaylistItemDropEvent *e ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );

    if ( e->urls().count() > 0 ) {
        foreach( QString url, e->urls() ) {
            qDebug() << "PlaylistItemEventHandler => OnPlaylistItemDrop=" << url;

            //Approve Url by WhiteList
            if ( ConfigManager::instance()->isInWhiteList( url ) ) {
                QFileInfo fileInfo( url );
                FileTask* task = new FileTask( fileInfo.absoluteFilePath(),
                                               ConfigManager::instance()->getMediaDirectory(),
                                               false );
                _copyTasks->insert( task->guid(), task );
                TaskExecuteRequestEvent ev( task );
                Aggregator::instance()->sendEvent( &ev );
            } else {
                qDebug() << "PlaylistItemEventHandler => Not in WhiteList=" << url;
            }
        }
    }
}

void PlaylistItemEventHandler::onPlaylistItemRemoved( RedBullPlayer::Events::PlaylistItemRemovedEvent *e ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );

    if ( e->urls().count() > 0 ) {
        foreach( QString url, e->urls() ) {
            QFileInfo fileInfo( url );
            //Remove from MediaDb
            IMedia* media = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByFileName( fileInfo.absoluteFilePath() );

            if ( media != 0 ) {
                removeMediaItem( media->guid() );
            }

            delete media;
            //Request MoveTask to delete it from media folder if media is in Media Folder
            QDir mediaDirectory = QDir( ConfigManager::instance()->getMediaDirectory() );

            if( fileInfo.absoluteDir().absolutePath() == mediaDirectory.absolutePath() ) {
                FileTask* task = new FileTask( fileInfo.absoluteFilePath(),
                                               ConfigManager::instance()->getDeleteDirectory(),
                                               true );
                _moveTasks->insert( task->guid(), task );
                TaskExecuteRequestEvent ev( task );
                Aggregator::instance()->sendEvent( &ev );
            }
        }
    }
}

void PlaylistItemEventHandler::onTaskExecuteResponse( RedBullPlayer::Events::TaskFinishedEvent* e ) {
    if ( _copyTasks->contains( e->taskId() ) ) {
        //Copy Task finished
        ITask* task = _copyTasks->take( e->taskId() );

        if ( task->result()->isEqual( TaskResult::SUCCESSFUL ) ) {
            //Successful
            FileTask* copyTask = static_cast<FileTask*>( task );
            processNewMediaFile( QFileInfo( copyTask->newFileName() ), true );
        }
    }
}

void PlaylistItemEventHandler::onTaskCancelResponse( RedBullPlayer::Events::TaskCanceledEvent* e ) {
    if ( _copyTasks->contains( e->taskId() ) ) {
        _copyTasks->take( e->taskId() );
    } else if ( _moveTasks->contains( e->taskId() ) ) {
        _moveTasks->take( e->taskId() );
    }
}

void PlaylistItemEventHandler::processNewMediaFile( QFileInfo file, bool userAdded ) {
    PlaylistItemEventHandler::processNewMediaFile( _mediaLibrary, file, userAdded );
}

void PlaylistItemEventHandler::removeMediaItem( QUuid mediaId ) {
    IMedia* media = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( mediaId );

    if ( media != 0 ) {
        Error err;

        if ( _mediaLibrary->mediaDB()->mediaRepository()->deleteMedia( *media, &err ) ) {
            SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, "removed Media from Db, ID = " + media->guid().toString() );
            MediaRemovedEvent ev( media->guid() );
            Aggregator::instance()->sendEvent( &ev );
        } else {
            Error::raise( &err );
        }
    }

    delete media;
}

void PlaylistItemEventHandler::processNewMediaFile( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QFileInfo file, bool userAdded ) {
    QMutexLocker locker( _processNewMediaFileMutex );
    qDebug() << "PlaylistItemEventHandler => processNewMediaFile";
    bool video = ConfigManager::instance()->isInVideoWhiteList( file );
    bool audio = ConfigManager::instance()->isInAudioWhiteList( file );
    bool image = ConfigManager::instance()->isInImageWhiteList( file );
    bool document = ConfigManager::instance()->isInDocumentWhiteList( file );
    IMediaType* mediaType = 0;

    if( video ) {
        mediaType = mediaLibrary->mediaDB()->videoType();
    }

    if( audio ) {
        mediaType = mediaLibrary->mediaDB()->audioType();
    }

    if( image ) {
        mediaType = mediaLibrary->mediaDB()->imageType();
    }

    if ( document ) {
        mediaType = mediaLibrary->mediaDB()->documentType();
    }

    if( mediaType == 0 ) {
        return;
    }

    bool newItem = true;
    QString fileURL = file.absoluteFilePath();
    IMedia* mediaObj = 0;
    mediaObj = mediaLibrary->mediaDB()->mediaRepository()->getMediaByFileName( fileURL );

    if ( ! mediaObj ) {
        mediaObj = mediaLibrary->mediaDB()->mediaRepository()->createNewMedia(
                       RedBullPlayer::Tools::UuidCreator::create(),
                       fileURL,
                       *mediaType );
    } else {
        newItem = false;
    }

    mediaObj->setFileSize( file.size() );

    if ( mediaObj->mediaType().guid() != mediaLibrary->mediaDB()->documentType()->guid() ) {
        SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, "Call Taglib to read metadata..." );
        mediaLibrary->metadataHandler()->setMetaDataUsingTagLib( mediaObj );
    }

    if ( mediaObj->title().isNull() ) {
        mediaObj->setTitle( file.fileName() );
    }

    if ( mediaType->guid() == mediaLibrary->mediaDB()->imageType()->guid() ) {
        QImage imgOrig( fileURL );
        QImage saveImg = imgOrig.scaled( 640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        mediaObj->setCoverImage( saveImg );
    }

    Error err;
    mediaLibrary->mediaDB()->mediaRepository()->saveMedia( *mediaObj, &err );
    Error::raise( &err );
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR,
                                     QString( "Send new discovered Media event %1" )
                                     .arg( mediaObj->fileName() ) );

    if ( newItem ) {
        if( userAdded ) {
            mediaDiscovered(  mediaObj->guid().toString(), 2 );
        } else {
            mediaDiscovered(  mediaObj->guid().toString(), 1 );
        }
    } else {
        mediaContentChanged(  mediaObj->guid().toString() );
    }
}

void PlaylistItemEventHandler::mediaContentChanged( QString mediaGuid ) {
    MediaItemContentChangedEvent* ev = new MediaItemContentChangedEvent( mediaGuid );
    Aggregator::instance()->sendEvent( ev );
}


void PlaylistItemEventHandler::mediaDiscovered( QString mediaGuid, int source ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTCREATOR, Q_FUNC_INFO );
    QUuid mediaID( mediaGuid );
    MediaDiscoveredEvent::MediaDiscoveredSource mediaSource = MediaDiscoveredEvent::STARTUP_MEDIALIBRARY;

    switch( source ) {
        case 0 : {
                mediaSource = MediaDiscoveredEvent::STARTUP_MEDIALIBRARY;
                break;
            }

        case 1: {
                mediaSource = MediaDiscoveredEvent::STARTUP_FOLDER;
                break;
            }

        case 2 : {
                mediaSource = MediaDiscoveredEvent::USER_ADD;
                break;
            }

        case 3 : {
                mediaSource = MediaDiscoveredEvent::POD_CATCHER;
                break;
            }

        case 4 : {
                mediaSource = MediaDiscoveredEvent::SEARCH;
                break;
            }
    }

    MediaDiscoveredEvent* ev = new MediaDiscoveredEvent( mediaID, mediaSource );
    Aggregator::instance()->postEvent( ev );
}
