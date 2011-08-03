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
#include "PodCastWorker.h"

//Qt includes
#include <QHashIterator>
#include <QDebug>

//Project includes
#include "../../Container/Constants.h"
#include "../../Container/IError.h"
#include "../../Container/Error.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/FileSystem/FileSystem.h"
#include "../../ToolLib/FileSystem/Storage.h"
#include "../../ToolLib/FileSystem/StorageFactory.h"
#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../Interfaces/RssFeed/IRssFeedItem.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"
#include "../../Events/ContentManagement/InitiateContentDownloadEvent.h"
#include "../../Events/ContentManagement/CancelContentDownloadEvent.h"
#include "../../Events/ContentManagement/ContentDownloadFinishedEvent.h"
#include "../../Events/ContentManagement/ContentDownloadProgressEvent.h"
#include "../../Events/ContentManagement/ContentDownloadStartedEvent.h"
#include "../../Events/ContentManagement/ContentDownloadCanceledEvent.h"
#include "../../Events/TaskManager/TaskExecuteRequestEvent.h"
#include "../../Events/TaskManager/TaskFinishedEvent.h"
#include "../../Events/TaskManager/TaskCancelRequestEvent.h"
#include "../../Events/TaskManager/TaskStartedEvent.h"
#include "../../Events/TaskManager/TaskProgressEvent.h"
#include "../../Events/TaskManager/TaskCanceledEvent.h"
#include "../../Events/ContentManagement/SkipContentEvent.h"
#include "../../Events/ContentManagement/BanContentEvent.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"
#include "../../Widgets/RedBullDialog.h"

//Module includes
#include "ContentDescriptor.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Tools::RSS;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools::Network;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;

PodCastWorker::PodCastWorker( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary,
                              RedBullPlayer::Modules::StyleFactory::IStyleFactory* styleFactory,
                              PodCastCache* podCastCache,
                              QObject* parent ) : QObject( parent ) {
    _mediaLibrary = mediaLibrary;
    _styleFactory = styleFactory;
    _podCastCache = podCastCache;
    bootstrap();
}

PodCastWorker::~PodCastWorker() {
    _downloadQueue->clear();
    delete _downloadQueue;
}

bool PodCastWorker::event( QEvent* event ) {
    if ( event->type() == InitiateContentDownload ) {
        InitiateContentDownloadEvent* e = static_cast<InitiateContentDownloadEvent*>( event );
        initiateDownload( e->contentId() );
    } else if ( event->type() == CancelContentDownload ) {
        CancelContentDownloadEvent* e = static_cast<CancelContentDownloadEvent*>( event );
        cancelDownload( e->contentId() );
    } else if ( event->type() == TaskStarted ) {
        TaskStartedEvent* e = static_cast<TaskStartedEvent*>( event );
        taskStarted( e->taskId() );
    } else if ( event->type() == TaskFinished ) {
        TaskFinishedEvent* e = static_cast<TaskFinishedEvent*>( event );
        taskExecuted( e->taskId() );
    } else if ( event->type() == TaskProgress ) {
        TaskProgressEvent* e = static_cast<TaskProgressEvent*>( event );
        taskProgress( e->taskId(), e->progress() );
    } else if ( event->type() == TaskCanceled ) {
        TaskCanceledEvent * e = static_cast<TaskCanceledEvent*>( event );
        taskCanceled( e->taskId() );
    } else if ( event->type() == SkipContent ) {
        SkipContentEvent* e = static_cast<SkipContentEvent*>( event );
        skipContent( e->contentId() );
    } else if ( event->type() == BanContent ) {
        BanContentEvent* e = static_cast<BanContentEvent*>( event );
        banContent( e->contentId() );
    } else if ( event->type() == InternetConnectionStatusChanged ) {
        InternetConnectionStatusChangedEvent* e = static_cast<InternetConnectionStatusChangedEvent*>( event );
        _internetAvailable = e->internetAvailable();
    }

    return false;
}

void PodCastWorker::bootstrap() {
    _useQueue = false;
    _internetAvailable = true;
    _downloadQueue = new QQueue<QString>();
    _podCastTasks = new QHash<QString, PodCastTask*>();
    _storageReservations = new QHash<QString, RedBullPlayer::Tools::IStorageReservation*>();
    _moveTasks = new QHash<QString, FileTask*>();
    _rssFeedImageDownloadUtil = new DownloadUtil( this );
    connect( _rssFeedImageDownloadUtil,
             SIGNAL( downloadFinished( QString, QString, QString ) ),
             this,
             SLOT( rssFeedImagedownloadFinished( QString, QString, QString ) ) );
    connect( _rssFeedImageDownloadUtil,
             SIGNAL( downloadFailed( QString, int, QString ) ),
             this,
             SLOT( rssFeedImagedownloadFailed( QString, int, QString ) ) );
    //Subscrib to events
    Aggregator::instance()->registerListener( InitiateContentDownload, this );
    Aggregator::instance()->registerListener( CancelContentDownload, this );
    Aggregator::instance()->registerListener( SkipContent, this );
    Aggregator::instance()->registerListener( TaskStarted, this );
    Aggregator::instance()->registerListener( TaskFinished, this );
    Aggregator::instance()->registerListener( TaskProgress, this );
    Aggregator::instance()->registerListener( TaskCanceled, this );
    Aggregator::instance()->registerListener( BanContent, this );
    Aggregator::instance()->registerListener( InternetConnectionStatusChanged, this );
}

void PodCastWorker::skipContent( QString contentId ) {
    _podCastCache->skipContent( contentId );
}

void PodCastWorker::banContent( QString contentId ) {
    _podCastCache->banContent( contentId );
}

void PodCastWorker::showDiskSpaceError() {
    QString styleSheet = "";

    if ( _styleFactory != 0 ) {
        bool ok;
        ITextStyle* style = _styleFactory->getTextStyle( "UserSettingsDialog_Text", &ok );

        if ( ok ) {
            styleSheet = style->styleSheet();
        }
    }

    QString dialogText;
    QFile downloadFinishedFile( Constants::ROOT_UI_TEXT_DIR + Constants::DISKSPACE_FULL_TEXT );

    if ( downloadFinishedFile.open( QFile::ReadOnly ) ) {
        QByteArray fileArray = downloadFinishedFile.readAll();
        downloadFinishedFile.close();
        dialogText = QString( fileArray );
    } else {
        dialogText = "There is not enough disk space left on your volume. Please free up some space. Until that RedBull Media Player will not fully function.";
    }

    RedBullDialog* dialog = new RedBullDialog( dialogText,
            styleSheet,
            RedBullDialog::Ok,
            0 );
    dialog->exec();
}

void PodCastWorker::initiateDownload( QString contentId ) {
    if ( !_internetAvailable ) {
        qDebug() << "PodCastWorker => No internet available.. return";
        return;
    }

    qDebug() << "PodCastWorker => InitiateContentDownload (" << contentId << ")";
    qDebug() << "PodCastWorker => UseQueue=" << _useQueue;

    if ( _useQueue ) {
        _downloadQueue->enqueue( contentId );
    } else {
        QString tempDir = ConfigManager::instance()->getTempDirectory();

        if ( _podCastCache->isSkippedContent( contentId ) ) {
            qDebug() << "PodCastWorker => ResetSkipContent";
            _podCastCache->resetSkipContent( contentId );
        }

        ContentDescriptor* descriptor = _podCastCache->getContentDescriptorById( contentId );
        Error err;
        IStorage* storage = StorageFactory::defaultStorage();
        qDebug() << "Create storageReservation for:" <<  descriptor->content()->fileSize() << "bytes";
        IStorageReservation* storageReservation = storage->reserveStorage( descriptor->content()->fileSize(), &err );

        if ( ! storageReservation ) {
            showDiskSpaceError();
            return;
        }

        _storageReservations->insert( contentId, storageReservation );
        //Create Task
        PodCastTask* task = new PodCastTask( descriptor->content()->name() );

        if ( !descriptor->imageDownloadUrl().isEmpty() ) {
            task->setPodCastImageUrl( descriptor->imageDownloadUrl() );
        }

        task->setPodCastItemUrl( descriptor->content()->downloadUrl() );
        task->setTargetDirectory( tempDir );
        qDebug() << "PodCastWorker => CreatePodCastTask (" << task->guid() << ")";
        _podCastTasks->insert( contentId, task );
        TaskExecuteRequestEvent ev( task );
        Aggregator::instance()->sendEvent( &ev );
        ContentDownloadStartedEvent startedEvent( contentId );
        Aggregator::instance()->sendEvent( &startedEvent );
    }
}

void PodCastWorker::taskStarted( QUuid taskId ) {
    /*QHashIterator<QString,PodCastTask*> i(*_podCastTasks);
    while (i.hasNext()) {
        i.next();
        if (i.value()->guid() == taskId) {
            ContentDownloadStartedEvent ev(i.key());
            Aggregator::instance()->sendEvent(&ev);
            break;
        }
    }*/
}

void PodCastWorker::taskExecuted( QUuid taskId ) {
    qDebug() << "PodCastWorker => TaskExecuted (" << taskId << ")";
    QHashIterator<QString, PodCastTask*> pCIter( *_podCastTasks );

    while ( pCIter.hasNext() ) {
        pCIter.next();

        if ( pCIter.value()->guid() == taskId ) {
            PodCastTask* task = pCIter.value();
            _podCastTasks->remove( pCIter.key() );

            IStorageReservation* storageReservation = _storageReservations->value( pCIter.key() );
            Error err;

            if ( storageReservation ) {
                _storageReservations->remove( pCIter.key() );
                qDebug() << "Commit Reservation of: " << storageReservation->reservedBytes() << "bytes";
                StorageFactory::defaultStorage()->commitReservation( storageReservation, &err );
            } else {
                qWarning() << "No Storage Reservation found for PodCastTask: " << taskId;
            }

            if ( task->result()->isEqual( TaskResult::SUCCESSFUL ) ) {
                if ( task->hasImage() ) {
                    _podCastCache->setRssFeedItemImage( pCIter.key(), task->podCastImage().toImage() );
                }

                FileTask* moveTask = new FileTask( task->localFileName(),
                                                   ConfigManager::instance()->getMediaDirectory(),
                                                   true );
                _moveTasks->insert( pCIter.key(), moveTask );
                TaskExecuteRequestEvent ev( moveTask );
                Aggregator::instance()->sendEvent( &ev );
            }

            return;
        }
    }

    QHashIterator<QString, FileTask*> mI( *_moveTasks );

    while ( mI.hasNext() ) {
        mI.next();

        if ( mI.value()->guid() == taskId ) {
            FileTask* task = mI.value();
            _moveTasks->remove( mI.key() );
            _podCastCache->setRssFeedItemLocalFileName( mI.key(), task->newFileName() );
            _podCastCache->saveRssFeedItemToDb( mI.key() );
            ContentDownloadFinishedEvent ev( mI.key() );
            Aggregator::instance()->sendEvent( &ev );
            return;
        }
    }
}

void PodCastWorker::taskProgress( QUuid taskId, int progress ) {
    QHashIterator<QString, PodCastTask*> i( *_podCastTasks );

    while ( i.hasNext() ) {
        i.next();

        if ( i.value()->guid() == taskId ) {
            ContentDownloadProgressEvent ev( i.key(), progress );
            Aggregator::instance()->sendEvent( &ev );
            break;
        }
    }
}

void PodCastWorker::taskCanceled( QUuid taskId ) {
    qDebug() << "PodCastWorker => TaskCanceled (" << taskId << ")";
    QHashIterator<QString, PodCastTask*> i( *_podCastTasks );

    while ( i.hasNext() ) {
        i.next();

        if ( i.value()->guid() == taskId ) {
            IStorageReservation* storageReservation = _storageReservations->value( i.key() );
            Error err;

            if ( storageReservation ) {
                _storageReservations->remove( i.key() );
                qDebug() << "Canceled Reservation of: " << storageReservation->reservedBytes() << "bytes";
                StorageFactory::defaultStorage()->cancelReservation( storageReservation, &err );
            } else {
                qWarning() << "No Storage Reservation found for PodCastTask: " << taskId;
            }

            ContentDownloadCanceledEvent ev( i.key() );
            Aggregator::instance()->sendEvent( &ev );
            _podCastTasks->remove( i.key() );
            break;
        }
    }
}

void PodCastWorker::cancelDownload( QString contentId ) {
    qDebug() << "PodCastWorker => CancelDownload(" << contentId << ")";

    if ( _useQueue ) {
        qDebug() << "PodCastWorker => CancelDownload remove from Queue";
        _downloadQueue->removeAll( contentId );
    }

    if ( _podCastTasks->contains( contentId ) ) {
        qDebug() << "PodCastWorker => CancelDownload send TaskCancelRequestEvent";
        PodCastTask* task = _podCastTasks->value( contentId );
        TaskCancelRequestEvent ev( task->guid() );
        Aggregator::instance()->sendEvent( &ev );
    } else {
        qDebug() << "PodCastWorker => CancelDownload no PodCastTask for given Id";
    }
}

void PodCastWorker::parseStarted() {
    _useQueue = true;
}

void PodCastWorker::parseFinished() {
    _useQueue = false;
    int i;

    for( i = 0; i < _downloadQueue->count(); i++ ) {
        initiateDownload( _downloadQueue->dequeue() );
    }
}

void PodCastWorker::rssFeedImagedownloadFinished( QString fileName, QString contentType, QString contentLength ) {
    _podCastCache->setRssFeedImage( fileName );
}

void PodCastWorker::rssFeedImagedownloadFailed( QString fileName, int httpStatusCode, QString reasonPhrase ) {
    SimpleLogger::instance()->error( Constants::LOGGER_PODCATCHER, "RssFeedImage download failed." );
}


