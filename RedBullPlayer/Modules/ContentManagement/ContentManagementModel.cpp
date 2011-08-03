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
#include "ContentManagementModel.h"

#include "../../Container/Constants.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;

ContentManagementModel::ContentManagementModel( ContentManagementViewFactory *viewFactory,
        IContentListView *availableListView,
        IContentListView *downloadedListView,
        IContentListView *skippedListView,
        QObject *parent ) : QObject( parent ) {
    _viewFactory = viewFactory;
    _availableListView = availableListView;
    _downloadedListView = downloadedListView;
    _skippedListView = skippedListView;
    bootstrapAllButtons();
}

void ContentManagementModel::onContentDownloadStarted( QString contentId ) {
    if( _availableDownloadViews.contains( contentId ) ) {
        _availableDownloadViews.value( contentId )->onDownloadStarted();
        _availableDownloadViews.value( contentId )->setState( Constants::CONTENT_DOWNLOAD_PENDING );
    }

    switchVisibleCancelDownloadAllButtons( isAtLeastOneItemDownloading() );
}

void ContentManagementModel::onContentDownloadProgress( QString contentId, int progress ) {
    if( _availableDownloadViews.contains( contentId ) ) {
        _availableDownloadViews.value( contentId )->setProgress( progress );
        _availableDownloadViews.value( contentId )->setState( Constants::CONTENT_DOWNLOAD_RUNNING );
    }
}

void ContentManagementModel::onContentDownloadCanceled( QString contentId ) {
    if( _availableDownloadViews.contains( contentId ) ) {
        _availableDownloadViews.value( contentId )->onDownloadCanceled();
        _availableDownloadViews.value( contentId )->setState( Constants::CONTENT_NEW );
    }

    switchVisibleCancelDownloadAllButtons( isAtLeastOneItemDownloading() );
}

void ContentManagementModel::onNewContentAvailable( IContent *content, ContentCategory::CategoryEnum category ) {
    //FAKE Deal
    /*  addAvailableItemView(content);
     addDownloadedItemView(content);
     addSkippedItemView(content);*/

    //REAL Deal
    switch( category ) {
        case ContentCategory::NEW :
            addAvailableItemView( content );
            break;

        case ContentCategory::DOWNLOADED :
            addDownloadedItemView( content );
            break;

        case ContentCategory::SKIPPED :
            addSkippedItemView( content );
            break;
    }
}

void ContentManagementModel::onRemoveContent( QString contentId ) {
    if( _availableDownloadViews.contains( contentId ) ) {
        AvailableDownloadView *view = _availableDownloadViews.take( contentId );
        _availableListView->removeContentItemView( view );
        view->deleteLater();

        if( _availableDownloadViews.count() == 0 )
            hideNewDownloadsAllButtons();
        else
            switchVisibleCancelDownloadAllButtons( isAtLeastOneItemDownloading() );

        return;
    }

    if( _downloadedViews.contains( contentId ) ) {
        DownloadedView *view = _downloadedViews.take( contentId );
        _downloadedListView->removeContentItemView( view );
        view->deleteLater();
        return;
    }

    if( _skippedViews.contains( contentId ) ) {
        SkippedView *view = _skippedViews.take( contentId );
        _skippedListView->removeContentItemView( view );
        view->deleteLater();

        if( _skippedViews.count() == 0 )
            hideSkippedAllButtons();

        return;
    }
}

void ContentManagementModel::onBulkNewContentAvailable( QList<IContent *>* content,
        ContentCategory::CategoryEnum category ) {
    //FAKE Deal
    /*bootStrapAvailableListView(content);
    bootStrapDownloadedListView(content);
    bootStrapSkippedListView(content);*/

    //REAL DEAL
    switch( category ) {
        case ContentCategory::NEW :
            bootStrapAvailableListView( content );
            break;

        case ContentCategory::DOWNLOADED :
            bootStrapDownloadedListView( content );
            break;

        case ContentCategory::SKIPPED :
            bootStrapSkippedListView( content );
            break;
    }
}

void ContentManagementModel::downloadAllNewDownloads() {
    foreach( AvailableDownloadView * view, _availableDownloadViews )

    if( view->getState() == Constants::CONTENT_NEW )
        emit download( view->guid() );
}

void ContentManagementModel::cancelAllRunningDownloads() {
    foreach( AvailableDownloadView * view, _availableDownloadViews )

    if( view->getState() == Constants::CONTENT_DOWNLOAD_RUNNING
            || view->getState() == Constants::CONTENT_DOWNLOAD_PENDING )
        emit cancelDownload( view->guid() );
}

void ContentManagementModel::skipAllAvailableDownloads() {
    foreach( AvailableDownloadView * view, _availableDownloadViews )

    if( view->getState() == Constants::CONTENT_NEW )
        emit skipDownload( view->guid() );
}

void ContentManagementModel::downloadAllSkippedDownloads() {
    foreach( SkippedView * view, _skippedViews )
    emit download( view->guid() );
}

void ContentManagementModel::deleteAllSkipedDownloads() {
    foreach( SkippedView * view, _skippedViews )
    emit deleteDownload( view->guid() );
}

void ContentManagementModel::bootStrapAvailableListView( QList<IContent *>* content ) {
    clearAvailableDownloadViews();
    foreach( IContent * contentItem , *content ) {
        addAvailableItemView( contentItem );
    }

    if( content->count() > 0 )
        showNewDownloadsAllButtons();
}

void ContentManagementModel::addAvailableItemView( IContent *content ) {
    AvailableDownloadView *itemView = _viewFactory->createAvailableDownloadView( content );
    itemView->setState( Constants::CONTENT_NEW );
    connect( itemView, SIGNAL( cancelSignal( QString ) ), this, SLOT( cancelClickedSlot( QString ) ) );
    connect( itemView, SIGNAL( downloadSignal( QString ) ), this, SLOT( downloadClickedSlot( QString ) ) );
    connect( itemView, SIGNAL( skipSignal( QString ) ), this, SLOT( skipClickedSlot( QString ) ) );
    _availableDownloadViews.insert( itemView->guid(), itemView );
    _availableListView->addContentItemView( _availableDownloadViews.count() - 1 , itemView );

    if( _availableDownloadViews.count() == 1 )
        showNewDownloadsAllButtons();
}

void ContentManagementModel::bootStrapDownloadedListView( QList<IContent *> *content ) {
    clearDownloadedViews();
    foreach( IContent * contentItem , *content ) {
        addDownloadedItemView( contentItem );
    }
}

void ContentManagementModel::addDownloadedItemView( IContent *content ) {
    DownloadedView *itemView = _viewFactory->createDownloadedView( content );
    _downloadedViews.insert( itemView->guid(), itemView );
    _downloadedListView->addContentItemView( _downloadedViews.count() - 1, itemView );
}

void ContentManagementModel::bootStrapSkippedListView( QList<IContent *> *content ) {
    clearSkippedViews();
    foreach( IContent * contentItem , *content ) {
        addSkippedItemView( contentItem );
    }

    if( content->count() > 0 )
        showSkippedAllButtons();
}

void ContentManagementModel::addSkippedItemView( IContent *content ) {
    SkippedView *itemView = _viewFactory->createSkippedView( content );
    connect( itemView, SIGNAL( downloadClicked( QString ) ), this, SLOT( downloadClickedSlot( QString ) ) );
    connect( itemView, SIGNAL( deleteClicked( QString ) ), this, SLOT( deleteClickedSlot( QString ) ) );
    _skippedViews.insert( itemView->guid(), itemView );
    _skippedListView->addContentItemView( _skippedViews.count() - 1, itemView );

    if( _skippedViews.count() == 1 )
        showSkippedAllButtons();
}

void ContentManagementModel::clearAvailableDownloadViews() {
    foreach( AvailableDownloadView * view, _availableDownloadViews ) {
        _availableListView->removeContentItemView( view );
        view->deleteLater();
    }
    _availableDownloadViews.clear();
    hideNewDownloadsAllButtons();
}

void ContentManagementModel::clearDownloadedViews() {
    foreach( DownloadedView * view, _downloadedViews ) {
        _downloadedListView->removeContentItemView( view );
        view->deleteLater();
    }
    _downloadedViews.clear();
}

void ContentManagementModel::clearSkippedViews() {
    foreach( SkippedView * view, _skippedViews ) {
        _skippedListView->removeContentItemView( view );
        view->deleteLater();
    }
    _skippedViews.clear();
    hideSkippedAllButtons();
}

void ContentManagementModel::bootstrapAllButtons() {
    _downloadAllNewButton = _viewFactory->createDownloadAllButton();
    connect( _downloadAllNewButton, SIGNAL( clicked() ), this, SLOT( downloadAllNewDownloads() ) );
    _availableListView->addButton( _downloadAllNewButton );
    _cancelAllButton = _viewFactory->createCancelAllButton();
    connect( _cancelAllButton, SIGNAL( clicked() ), this, SLOT( cancelAllRunningDownloads() ) );
    _availableListView->addButton( _cancelAllButton );
    _skipAllButton = _viewFactory->createSkipAllButton();
    connect( _skipAllButton, SIGNAL( clicked() ), this, SLOT( skipAllAvailableDownloads() ) );
    _availableListView->addButton( _skipAllButton );
    _downloadAllSkippedButton = _viewFactory->createDownloadAllButton();
    connect( _downloadAllSkippedButton, SIGNAL( clicked() ), this, SLOT( downloadAllSkippedDownloads() ) );
    _skippedListView->addButton( _downloadAllSkippedButton );
    _deleteAllButton = _viewFactory->createDeleteAllButton();
    connect( _deleteAllButton, SIGNAL( clicked() ), this, SLOT( deleteAllSkipedDownloads() ) );
    _skippedListView->addButton( _deleteAllButton );
    _downloadAllNewButton->hide();
    _cancelAllButton->hide();
    _skipAllButton->hide();
    _downloadAllSkippedButton->hide();
    _deleteAllButton->hide();
}

void ContentManagementModel::cancelClickedSlot( QString guid ) {
    emit cancelDownload( guid );
}

void ContentManagementModel::downloadClickedSlot( QString guid ) {
    emit download( guid );
}

void ContentManagementModel::skipClickedSlot( QString guid ) {
    emit skipDownload( guid );
}

void ContentManagementModel::deleteClickedSlot( QString guid ) {
    emit deleteDownload( guid );
}


void ContentManagementModel::showNewDownloadsAllButtons() {
    _downloadAllNewButton->show();
    _cancelAllButton->show();
    _skipAllButton->show();
    switchVisibleCancelDownloadAllButtons( isAtLeastOneItemDownloading() );
}

void ContentManagementModel::showSkippedAllButtons() {
    _downloadAllSkippedButton->show();
    _deleteAllButton->show();
}

void ContentManagementModel::hideNewDownloadsAllButtons() {
    _downloadAllNewButton->hide();
    _cancelAllButton->hide();
    _skipAllButton->hide();
}

void ContentManagementModel::hideSkippedAllButtons() {
    _downloadAllSkippedButton->hide();
    _deleteAllButton->hide();
}

void ContentManagementModel::switchVisibleCancelDownloadAllButtons( bool isAtLeastOneItemDownloading ) {
    if( isAtLeastOneItemDownloading ) {
        _downloadAllNewButton->hide();
        _cancelAllButton->show();
    } else {
        _cancelAllButton->hide();
        _downloadAllNewButton->show();
    }
}

bool ContentManagementModel::isAtLeastOneItemDownloading() {
    foreach( AvailableDownloadView * view, _availableDownloadViews )

    if( view->getState() == Constants::CONTENT_DOWNLOAD_RUNNING
            || view->getState() == Constants::CONTENT_DOWNLOAD_PENDING )
        return true;

    return false;
}

