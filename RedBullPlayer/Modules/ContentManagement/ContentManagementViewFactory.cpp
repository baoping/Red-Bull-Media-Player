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
#include "ContentManagementViewFactory.h"

#include <math.h>

#include "../../Container/Constants.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;

ContentManagementViewFactory::ContentManagementViewFactory( IStyleFactory *styleFactory, QWidget *viewsParent, QObject *parent )
    : QObject( parent ) {
    _viewsParent = viewsParent;
    _styleFactory = styleFactory;
}


IContentListView *ContentManagementViewFactory::createContentListView() {
    IContentListView *listView = new ContentListView( 18, _viewsParent );
    return listView;
}

IContentListView *ContentManagementViewFactory::createNewDownloadsListView() {
    IContentListView *listView = new ContentListView( 18, _viewsParent );
    listView->setHeader( ConfigManager::instance()->downloadedItemHeaderTitle(),
                         ConfigManager::instance()->downloadedSizeHeaderTitle(),
                         "" );
    listView->setFirstHeaderWidth( 219 ); //Magic Number to align Table Header with Items

    if( _styleFactory != NULL ) {
        bool ok;
        ITextStyle *headerStyle = _styleFactory->getTextStyle( "ContentManagement_Header_Title", &ok );

        if( ok )
            listView->setHeaderStyle( headerStyle );
    }

    return listView;
}

IContentListView *ContentManagementViewFactory::createSkippedListView() {
    IContentListView *listView = new ContentListView( 18, _viewsParent );
    listView->setHeader( ConfigManager::instance()->downloadedItemHeaderTitle(),
                         ConfigManager::instance()->downloadedSizeHeaderTitle(),
                         "" );
    listView->setFirstHeaderWidth( 265 ); //Magic Number to align Table Header with Items

    if( _styleFactory != NULL ) {
        bool ok;
        ITextStyle *headerStyle = _styleFactory->getTextStyle( "ContentManagement_Header_Title", &ok );

        if( ok )
            listView->setHeaderStyle( headerStyle );
    }

    return listView;
}

IContentListView *ContentManagementViewFactory::createDownloadedListView() {
    IContentListView *listView = new ContentListView( 18, _viewsParent );
    listView->setHeader( ConfigManager::instance()->downloadedItemHeaderTitle(),
                         ConfigManager::instance()->downloadedSizeHeaderTitle(),
                         ConfigManager::instance()->downloadedDateHeaderTitle() );

    if( _styleFactory != NULL ) {
        bool ok;
        ITextStyle *headerStyle = _styleFactory->getTextStyle( "ContentManagement_Header_Title", &ok );

        if( ok )
            listView->setHeaderStyle( headerStyle );
    }

    return listView;
}

AvailableDownloadView *ContentManagementViewFactory::createAvailableDownloadView( IContent *content ) {
    AvailableDownloadView *itemView = new AvailableDownloadView( content->guid(), _viewsParent );
    resolveContentItemViewStyle( itemView );
    itemView->setTitle( content->name() );
    itemView->setSize( resolveSizeString( content->fileSize() ) );
    itemView->setToolTipText( resolveToolTipString( content ) );
    return itemView;
}

DownloadedView *ContentManagementViewFactory::createDownloadedView( IContent *content ) {
    DownloadedView *itemView = new DownloadedView( content->guid(), _viewsParent );
    resolveContentItemViewStyle( itemView );
    itemView->setTitle( content->name() );
    itemView->setSize( resolveSizeString( content->fileSize() ) );
    itemView->setDate( resolveDateString( content->date() ) );
    itemView->setToolTipText( resolveToolTipString( content ) );
    return itemView;
}

SkippedView *ContentManagementViewFactory::createSkippedView( IContent *content ) {
    SkippedView *itemView = new SkippedView( content->guid(), _viewsParent );
    resolveContentItemViewStyle( itemView );
    itemView->setTitle( content->name() );
    itemView->setSize( resolveSizeString( content->fileSize() ) );
    itemView->setToolTipText( resolveToolTipString( content ) );
    return itemView;
}

QString ContentManagementViewFactory::resolveSizeString( uint size ) {
    uint sizeMB = ( uint )round( size / ( 1024 * 1024 ) );
    return QString( "%1 MB" ).arg( sizeMB );
}

QString ContentManagementViewFactory::resolveDateString( QDateTime date ) {
    return date.toString( "yyyy-MM-dd" );
}

QString ContentManagementViewFactory::resolveToolTipString( IContent *content ) {
    return QString( "Title: %1 \nFilename: %2" ).arg( content->name() ).arg( content->fileName() );
}

void ContentManagementViewFactory::resolveContentItemViewStyle( IContentItemView *itemView, QString textStyleName ) {
    if( _styleFactory == NULL )
        return;

    bool ok;
    ITextStyle *textStyle = _styleFactory->getTextStyle( textStyleName, &ok );

    if( ok )
        itemView->setTextStyle( textStyle );
}

CommandButton *ContentManagementViewFactory::createCancelAllButton() {
    CommandButton *cancelAllButton = new CommandButton( _viewsParent, Constants::CONTENT_MAMAGEMENT_IMAGE_DIR +
            Constants::CONTENT_MAMAGEMENT_CANCEL_ALL_BUTTON );
    cancelAllButton->setMouseOverImage( Constants::CONTENT_MAMAGEMENT_IMAGE_DIR
                                        + Constants::CONTENT_MAMAGEMENT_CANCEL_ALL_BUTTON_PRESSED );
    return cancelAllButton;
}

CommandButton *ContentManagementViewFactory::createSkipAllButton() {
    CommandButton *skipAllButton = new CommandButton( _viewsParent, Constants::CONTENT_MAMAGEMENT_IMAGE_DIR +
            Constants::CONTENT_MAMAGEMENT_SKIP_ALL_BUTTON );
    skipAllButton->setMouseOverImage( Constants::CONTENT_MAMAGEMENT_IMAGE_DIR +
                                      Constants::CONTENT_MAMAGEMENT_SKIP_ALL_BUTTON_PRESSED );
    return skipAllButton;
}

CommandButton *ContentManagementViewFactory::createDownloadAllButton() {
    CommandButton *downloadAllButton = new CommandButton( _viewsParent, Constants::CONTENT_MAMAGEMENT_IMAGE_DIR +
            Constants::CONTENT_MAMAGEMENT_DOWNLOAD_ALL_BUTTON );
    downloadAllButton->setMouseOverImage( Constants::CONTENT_MAMAGEMENT_IMAGE_DIR +
                                          Constants::CONTENT_MAMAGEMENT_DOWNLOAD_ALL_BUTTON_PRESSED );
    return downloadAllButton;
}

CommandButton *ContentManagementViewFactory::createDeleteAllButton() {
    CommandButton *deleteAllButton = new CommandButton( _viewsParent, Constants::CONTENT_MAMAGEMENT_IMAGE_DIR +
            Constants::CONTENT_MAMAGEMENT_DELETE_ALL_BUTTON );
    deleteAllButton->setMouseOverImage( Constants::CONTENT_MAMAGEMENT_IMAGE_DIR +
                                        Constants::CONTENT_MAMAGEMENT_DELETE_ALL_BUTTON_PRESSED );
    return deleteAllButton;
}
