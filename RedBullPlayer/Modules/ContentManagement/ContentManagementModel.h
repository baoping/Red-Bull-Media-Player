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
#ifndef CONTENTMANAGEMENTMODEL_H
#define CONTENTMANAGEMENTMODEL_H

#include <QObject>
#include <QList>
#include <QHash>

#include "AvailableDownloadView.h"
#include "DownloadedView.h"
#include "SkippedView.h"
#include "ContentManagementViewFactory.h"
#include "../../Interfaces/ContentManagement/IContentListView.h"
#include "../../Interfaces/Enumerations/ContentCategoryEnum.h"
#include "../../Interfaces/MediaLibrary/IContent.h"
#include "../../Widgets/CommandButton.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class ContentManagementModel : public QObject {
                    Q_OBJECT
                public:
                    ContentManagementModel( ContentManagementViewFactory *viewFactory,
                                            IContentListView *availableListView,
                                            IContentListView *downloadedListView,
                                            IContentListView *skippedListView,
                                            QObject *parent = 0 );

                    void onBulkNewContentAvailable( QList<RedBullPlayer::Modules::MediaLibrary::IContent*>* content,
                                                    RedBullPlayer::Modules::ContentManagement::ContentCategory::CategoryEnum category );
                    void onContentDownloadStarted( QString contentId );
                    void onContentDownloadProgress( QString contentId, int progress );
                    void onContentDownloadCanceled( QString contentId );
                    void onNewContentAvailable( RedBullPlayer::Modules::MediaLibrary::IContent* content,
                                                RedBullPlayer::Modules::ContentManagement::ContentCategory::CategoryEnum category );
                    void onRemoveContent( QString contentId );

                signals:
                    void download( QString contentId );
                    void skipDownload( QString contentId );
                    void cancelDownload( QString contentId );
                    void deleteDownload( QString contentId );

                public slots:
                    void downloadAllNewDownloads();
                    void cancelAllRunningDownloads();
                    void skipAllAvailableDownloads();
                    void downloadAllSkippedDownloads();
                    void deleteAllSkipedDownloads();

                protected slots:
                    void downloadClickedSlot( QString guid );
                    void skipClickedSlot( QString guid );
                    void cancelClickedSlot( QString guid );
                    void deleteClickedSlot( QString guid );

                private:
                    void bootStrapAvailableListView( QList<RedBullPlayer::Modules::MediaLibrary::IContent *>* content );
                    void bootStrapDownloadedListView( QList<RedBullPlayer::Modules::MediaLibrary::IContent *>* content );
                    void bootStrapSkippedListView( QList<RedBullPlayer::Modules::MediaLibrary::IContent *>* content );

                    void clearAvailableDownloadViews();
                    void clearDownloadedViews();
                    void clearSkippedViews();

                    void addAvailableItemView( RedBullPlayer::Modules::MediaLibrary::IContent *content );
                    void addDownloadedItemView( RedBullPlayer::Modules::MediaLibrary::IContent *content );
                    void addSkippedItemView( RedBullPlayer::Modules::MediaLibrary::IContent *content );

                    void bootstrapAllButtons();

                    void hideNewDownloadsAllButtons();
                    void hideSkippedAllButtons();
                    void showNewDownloadsAllButtons();
                    void showSkippedAllButtons();

                    bool isAtLeastOneItemDownloading();
                    void switchVisibleCancelDownloadAllButtons( bool isAtLeastOneItemDownloading );

                    ContentManagementViewFactory *_viewFactory;

                    IContentListView *_availableListView;
                    QHash<QString, AvailableDownloadView *> _availableDownloadViews;

                    IContentListView *_downloadedListView;
                    QHash<QString, DownloadedView *> _downloadedViews;

                    IContentListView *_skippedListView;
                    QHash<QString, SkippedView *> _skippedViews;

                    RedBullPlayer::Widgets::CommandButton *_downloadAllNewButton;
                    RedBullPlayer::Widgets::CommandButton *_cancelAllButton;
                    RedBullPlayer::Widgets::CommandButton *_skipAllButton;
                    RedBullPlayer::Widgets::CommandButton *_downloadAllSkippedButton;
                    RedBullPlayer::Widgets::CommandButton *_deleteAllButton;
            };
        }
    }
}

#endif // CONTENTMANAGEMENTMODEL_H
