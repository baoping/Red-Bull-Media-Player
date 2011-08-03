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
#include "MediaMetadataHandler.h"

// Qt includes
#include <QtPlugin>
#include <QDebug>
#include <QMessageBox>

// Project includes
#include "../../Container/Error.h"
#include "../../Container/Constants.h"
#include "../../PlayerShell/Shell.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/SnapshotCreator/SnapshotCreationRequestedEvent.h"
#include "../../Events/SnapshotCreator/SnapshotCreationRequestData.h"
#include "../../Events/SnapshotCreator/CancelSnapshotCreationEvent.h"
#include "../../Events/MediaLibrary/CoverImageChangedEvent.h"
#include "../../Events/MediaLibrary/ChapterImageCreatedEvent.h"
#include "../../Events/MetaDataChangedEvent.h"
#include "../../ToolLib/UuidCreator.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Tools;


MediaMetadataHandler::MediaMetadataHandler( IMediaLibrary* mediaLibrary, QObject* parent ) : QObject( parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    Q_ASSERT( mediaLibrary != 0 );
    _sentRequests = new QList<MediaRequestObj*>();
    _mediaLibrary = mediaLibrary;
    Aggregator::instance()->registerListener( SnapshotCreated, this );
    Aggregator::instance()->registerListener( CoverImageRequested, this );
    Aggregator::instance()->registerListener( CancelCoverImageCreation, this );
    //    Aggregator::instance()->registerListener( UpdateMediaMetaData, this );
    Aggregator::instance()->registerListener( ChapterImageRequest, this );
    //    Aggregator::instance()->registerListener( MetaDataDiscovered, this );
    _tagLibMetaData = new TagLibBasedMetaDataHandler( _mediaLibrary, this );
}

void MediaMetadataHandler::setMetaDataUsingTagLib( IMedia* media ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, "Call TagLib to receive metadata" );
    _tagLibMetaData->setMetaDataIfAvailable( media );
}

void MediaMetadataHandler::generateCoverImage( IMedia* media, float position /* = -1.0F */, uint second /* = 70 */ ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    Q_ASSERT( media != 0 );
    QFileInfo fileInfo( media->fileName() );

    if ( ! fileInfo.exists() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, fileInfo.absoluteFilePath() + " does not exist!" );
        return;
    }

    bool isCoverImage = false;

    if ( position == -1.0F ) {
        position = ConfigManager::instance()->coverImageGenerationPostition();
        isCoverImage = true;
    }

    SnapshotCreationRequestData* data = new SnapshotCreationRequestData(
        RedBullPlayer::Tools::UuidCreator::create(),
        media->fileName(),
        position,
        second );
    MediaRequestObj* mro = new MediaRequestObj( media->guid(), data->id(), position, isCoverImage );
    _sentRequests->append( mro );
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, QString( "Request snapshot id: %1 position: %2 second: %3" )
                                     .arg( data->id().toString() )
                                     .arg( position )
                                     .arg( second ) );
    SnapshotCreationRequestedEvent ev( data );
    Aggregator::instance()->sendEvent( &ev );
}
/*
void MediaMetadataHandler::requestMetaData( IMedia* media ) {
    SimpleLogger::instance()->debug(Constants::LOGGER_MEDIALIBRARY,Q_FUNC_INFO);
    Q_ASSERT( media != 0 );
    QUuid requestId = RedBullPlayer::Tools::UuidCreator::create();
    MediaRequestObj* mro = new MediaRequestObj( media->guid(), requestId, -0.1F, false );
    _sentRequests->append( mro );
    RequestMediaMetaDataEvent ev( requestId, media->fileName() );
    Aggregator::instance()->sendEvent( &ev );
}
*/
bool MediaMetadataHandler::event( QEvent* event ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );

    if ( event->type() == SnapshotCreated ) {
        onSnapshotCreated( static_cast<RedBullPlayer::Events::SnapshotCreatedEvent*>( event ) );
    }

    if ( event->type() == CoverImageRequested ) {
        onCoverImageRequested( static_cast<RedBullPlayer::Events::CoverImageRequestedEvent*>( event ) );
    }

    if ( event->type() == CancelCoverImageCreation ) {
        onCancelCoverImageCreation( static_cast<RedBullPlayer::Events::CancelCoverImageCreationEvent*>( event ) );
    }

    /*    if ( event->type() == UpdateMediaMetaData ) {
            onUpdateMediaMetaData( static_cast<RedBullPlayer::Events::UpdateMediaMetaDataEvent*>(event));
        } */
    if ( event->type() == ChapterImageRequest ) {
        onChapterImageRequest( static_cast<RedBullPlayer::Events::ChapterImageRequestEvent*>( event ) );
    }

    /*    if ( event->type() == MetaDataDiscovered ) {
            onMediaDataDiscovered( static_cast<RedBullPlayer::Events::MetaDataDiscoveredEvent*>(event));
        } */
    return false;
}
/*
void MediaMetadataHandler::onMediaDataDiscovered( RedBullPlayer::Events::MetaDataDiscoveredEvent* ev ) {
    SimpleLogger::instance()->debug(Constants::LOGGER_MEDIALIBRARY,Q_FUNC_INFO);

    IMedia* mediaObj = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( ev->mediaId() );
    if ( mediaObj == 0 ) {
        SimpleLogger::instance()->warn(Constants::LOGGER_MEDIALIBRARY,"Could not find media with id: " + ev->mediaId());
        return;
    }

    mediaObj->setAlbum((!ev->album().isEmpty()) ? ev->album() : mediaObj->album());
    mediaObj->setArtist((!ev->artist().isEmpty()) ? ev->artist() : mediaObj->artist());
    mediaObj->setArtworkUrl((!ev->artworkUrl().isEmpty()) ? ev->artworkUrl() : mediaObj->artworkUrl());
    mediaObj->setCopyright((!ev->copyright().isEmpty()) ? ev->copyright() : mediaObj->copyright());
    QDateTime date = QDateTime::fromString(ev->date(),Qt::ISODate);
    if (date.isValid()) {
        mediaObj->setDate((!ev->date().isEmpty()) ? date : mediaObj->date());
    }
    mediaObj->setDescription((!ev->description().isEmpty()) ? ev->description() : mediaObj->description());
    mediaObj->setEncodedBy((!ev->encodedBy().isEmpty()) ? ev->encodedBy() : mediaObj->encodedBy());
    mediaObj->setGenre((!ev->genre().isEmpty()) ? ev->genre() : mediaObj->genre());
    mediaObj->setLanguage((!ev->language().isEmpty()) ? ev->language() : mediaObj->language());
    mediaObj->setPublisher((!ev->publisher().isEmpty()) ? ev->publisher() : mediaObj->publisher());
    mediaObj->setRating((!ev->rating().isEmpty()) ? ev->rating() : mediaObj->rating());
    mediaObj->setSetting((!ev->setting().isEmpty()) ? ev->setting() : mediaObj->setting());
    mediaObj->setTitle((!ev->title().isEmpty()) ? ev->title() : mediaObj->title());
    mediaObj->setTrackID((!ev->trackID().isEmpty()) ? ev->trackID() : mediaObj->trackID());
    bool ok;
    uint trackNumber = ev->trackNumber().toUInt(&ok);
    if (ok) {
        mediaObj->setTrackNumber((!ev->trackNumber().isEmpty()) ? trackNumber : mediaObj->trackNumber());
    }
    mediaObj->setUrl((!ev->url().isEmpty()) ? ev->url() : mediaObj->url());

    Error err;
    if (_mediaLibrary->mediaDB()->mediaRepository()->saveMedia(*mediaObj,&err)) {
        MetaDataChangedEvent ev(mediaObj->guid());
        Aggregator::instance()->sendEvent( &ev );
    }
    delete mediaObj;
}
*/
void MediaMetadataHandler::onCoverImageRequested( RedBullPlayer::Events::CoverImageRequestedEvent* ev ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    IMedia* mediaObj = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( ev->mediaId() );

    if ( mediaObj == 0 ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "Could not find media with id: " + ev->mediaId() );
        return;
    }

    if ( mediaObj->mediaType().guid() != _mediaLibrary->mediaDB()->videoType()->guid() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "Cannot create snapshot for non video media: " + ev->mediaId() );
        delete mediaObj;
        return;
    }

    generateCoverImage( mediaObj );
    delete mediaObj;
}

/*
void MediaMetadataHandler::onUpdateMediaMetaData( RedBullPlayer::Events::UpdateMediaMetaDataEvent* ev ) {
    SimpleLogger::instance()->debug(Constants::LOGGER_MEDIALIBRARY,Q_FUNC_INFO);
    IMedia* mediaObj = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( ev->mediaId() );
    if ( mediaObj == 0 ) {
        SimpleLogger::instance()->warn(Constants::LOGGER_MEDIALIBRARY,"Could not find media with id: " + ev->mediaId());
        return;
    }
    requestMetaData( mediaObj );
    delete mediaObj;
}
*/
void MediaMetadataHandler::onCancelCoverImageCreation( RedBullPlayer::Events::CancelCoverImageCreationEvent *ev ) {
    QUuid mediaId = ev->mediaId();
    MediaRequestObj* mro = 0;

    for( int i = 0; i < _sentRequests->count() && mro == 0 ; i++ ) {
        if ( _sentRequests->at( i )->mediaId() == mediaId ) {
            mro = _sentRequests->at( i );
            break;
        }
    }

    if ( mro == 0 ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "No Request stored for mediaId: " + mediaId.toString() );
        return;
    }

    CancelSnapshotCreationEvent evCancelSnapshot( mro->requestId() );
    Aggregator::instance()->sendEvent( &evCancelSnapshot );
    _sentRequests->removeOne( mro );
}

void MediaMetadataHandler::onSnapshotCreated( RedBullPlayer::Events::SnapshotCreatedEvent* ev ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    QUuid requestId = ev->data()->requestId();
    QString snapshotFileName = ev->data()->snapshotFileName();
    MediaRequestObj* mro = 0;

    for( int i = 0; i < _sentRequests->count() && mro == 0 ; i++ ) {
        if ( _sentRequests->at( i )->requestId() == requestId ) {
            mro = _sentRequests->at( i );
        }
    }

    if ( mro == 0 ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "No Request stored for id: " + requestId.toString() );
        return;
    }

    if ( ! QFile::exists( snapshotFileName ) ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "No Request stored for Snapshot file found at: " + snapshotFileName );
        return;
    }

    QImage img( snapshotFileName );

    if ( img.isNull() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "Could not load IMage file: " + snapshotFileName );
        return;
    }

    IMedia* mediaObj = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( mro->mediaId() );

    if ( mediaObj == 0 ) {
        //QMessageBox::information( 0 , "", QString("Could not find media with id: %1").arg(mro->mediaId()) );
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "Could not find media with id: " + mro->mediaId().toString() );
        QFile::remove( snapshotFileName );
        return;
    }

    if ( mro->isCoverImage() ) {
        QImage saveImg = img.scaled( 640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        mediaObj->setCoverImage( saveImg );
        Error err;
        _mediaLibrary->mediaDB()->mediaRepository()->saveMedia( *mediaObj, &err );
        Error::raise( &err );
        QFile::remove( snapshotFileName );
        CoverImageChangedEvent evCoverImage( mediaObj->guid() );
        Aggregator::instance()->sendEvent( &evCoverImage );
    } else {
        QImage saveImg = img.scaledToHeight( 50, Qt::SmoothTransformation );
        ISnapshot* snapshot = _mediaLibrary->mediaDB()->snapshotRepository()->createSnapshot(
                                  RedBullPlayer::Tools::UuidCreator::create(), saveImg, mro->position(), mediaObj );
        Error err;
        _mediaLibrary->mediaDB()->snapshotRepository()->saveSnapshot( *snapshot, &err );
        QFile::remove( snapshotFileName );
        ChapterImageCreatedEvent evChapterImage( mediaObj->guid(), mro->position() );
        Aggregator::instance()->sendEvent( &evChapterImage );
        delete snapshot;
    }

    _sentRequests->removeOne( mro );
    delete mro;
    delete mediaObj;
}

void MediaMetadataHandler::onChapterImageRequest( RedBullPlayer::Events::ChapterImageRequestEvent* ev ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_MEDIALIBRARY, Q_FUNC_INFO );
    IMedia* mediaObj = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( ev->mediaId() );

    if ( mediaObj == 0 ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "Could not find media with id: " + ev->mediaId() );
        return;
    }

    if ( mediaObj->mediaType().guid() != _mediaLibrary->mediaDB()->videoType()->guid() ) {
        SimpleLogger::instance()->warn( Constants::LOGGER_MEDIALIBRARY, "Cannot create snapshot for non video media: " + ev->mediaId() );
        delete mediaObj;
        return;
    }

    generateCoverImage( mediaObj, ev->position(), ev->second() );
    delete mediaObj;
}
