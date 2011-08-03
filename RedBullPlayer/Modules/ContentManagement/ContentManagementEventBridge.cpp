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
#include "ContentManagementEventBridge.h"

#include "../../Events/EventTypes.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/ContentManagement/InitiateContentDownloadEvent.h"
#include "../../Events/ContentManagement/SkipContentEvent.h"
#include "../../Events/ContentManagement/BanContentEvent.h"
#include "../../Events/ContentManagement/CancelContentDownloadEvent.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;

ContentManagementEventBridge::ContentManagementEventBridge( ContentManagementModel *model, QObject *parent )
    : QObject( parent ) {
    _model = model;
    connect( _model, SIGNAL( download( QString ) ), this, SLOT( emitInitiateContentDownload( QString ) ) );
    connect( _model, SIGNAL( cancelDownload( QString ) ), this, SLOT( emitCancelContentDownload( QString ) ) );
    connect( _model, SIGNAL( skipDownload( QString ) ), this, SLOT( emitSkipContentEvent( QString ) ) );
    connect( _model, SIGNAL( deleteDownload( QString ) ), this, SLOT( emitDeleteContentDownload( QString ) ) );
    Aggregator::instance()->registerListener( BulkNewContentAvailable, this );
    Aggregator::instance()->registerListener( ContentDownloadStarted, this );
    Aggregator::instance()->registerListener( ContentDownloadProgress, this );
    Aggregator::instance()->registerListener( ContentDownloadCanceled, this );
    Aggregator::instance()->registerListener( NewContentAvailable , this );
    Aggregator::instance()->registerListener( RemoveContent , this );
}

bool ContentManagementEventBridge::event( QEvent* event ) {
    if( event->type() == BulkNewContentAvailable ) {
        onBulkNewContentAvailable( static_cast<BulkNewContentAvailableEvent *>( event ) );
    }

    if( event->type() == ContentDownloadStarted ) {
        onContentDownloadStarted( static_cast<ContentDownloadStartedEvent *>( event ) );
    }

    if( event->type() == ContentDownloadProgress ) {
        onContentDownloadProgress( static_cast<ContentDownloadProgressEvent *>( event ) );
    }

    if( event->type() == ContentDownloadCanceled ) {
        onContentDownloadCanceled( static_cast<ContentDownloadCanceledEvent *>( event ) );
    }

    if( event->type() == RemoveContent ) {
        onRemoveContent( static_cast<RemoveContentEvent *>( event ) );
    }

    if( event->type() == NewContentAvailable ) {
        onNewContentAvailable( static_cast<NewContentAvailableEvent *>( event ) );
    }

    return false;
}


void ContentManagementEventBridge::onBulkNewContentAvailable( BulkNewContentAvailableEvent *e ) {
    _model->onBulkNewContentAvailable( e->content(), e->category() );
}

void ContentManagementEventBridge::onContentDownloadProgress( ContentDownloadProgressEvent *e ) {
    _model->onContentDownloadProgress( e->contentId(), e->progress() );
}

void ContentManagementEventBridge::onContentDownloadStarted( ContentDownloadStartedEvent *e ) {
    _model->onContentDownloadStarted( e->contentId() );
}

void ContentManagementEventBridge::onContentDownloadCanceled( ContentDownloadCanceledEvent *e ) {
    _model->onContentDownloadCanceled( e->contentId() );
}

void ContentManagementEventBridge::onNewContentAvailable( NewContentAvailableEvent *e ) {
    _model->onNewContentAvailable( e->content(), e->category() );
}

void ContentManagementEventBridge::onRemoveContent( RemoveContentEvent *e ) {
    _model->onRemoveContent( e->contentId() );
}

void ContentManagementEventBridge::emitInitiateContentDownload( QString contentId ) {
    InitiateContentDownloadEvent e( contentId );
    Aggregator::instance()->sendEvent( &e );
}

void ContentManagementEventBridge::emitCancelContentDownload( QString contentId ) {
    CancelContentDownloadEvent e( contentId );
    Aggregator::instance()->sendEvent( &e );
}

void ContentManagementEventBridge::emitSkipContentEvent( QString contentId ) {
    SkipContentEvent e( contentId );
    Aggregator::instance()->sendEvent( &e );
}

void ContentManagementEventBridge::emitDeleteContentDownload( QString contentId ) {
    BanContentEvent e( contentId );
    Aggregator::instance()->sendEvent( &e );
}

