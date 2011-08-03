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
#ifndef CONTENTMANAGEMENTVIEWFACTORY_H
#define CONTENTMANAGEMENTVIEWFACTORY_H

#include <QObject>

#include "ContentListView.h"
#include "AvailableDownloadView.h"
#include "DownloadedView.h"
#include "SkippedView.h"
#include "../../Interfaces/MediaLibrary/IContent.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"
#include "../../Widgets/CommandButton.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class ContentManagementViewFactory : public QObject {
                    Q_OBJECT

                public:
                    ContentManagementViewFactory( RedBullPlayer::Modules::StyleFactory::IStyleFactory *styleFactory
                                                  , QWidget *viewsParent, QObject *parent = 0 );

                    IContentListView *createContentListView();
                    IContentListView *createDownloadedListView();
                    IContentListView *createSkippedListView();
                    IContentListView *createNewDownloadsListView();
                    AvailableDownloadView *createAvailableDownloadView( RedBullPlayer::Modules::MediaLibrary::IContent *content );
                    DownloadedView *createDownloadedView( RedBullPlayer::Modules::MediaLibrary::IContent *content );
                    SkippedView *createSkippedView( RedBullPlayer::Modules::MediaLibrary::IContent *content );
                    RedBullPlayer::Widgets::CommandButton *createCancelAllButton();
                    RedBullPlayer::Widgets::CommandButton *createSkipAllButton();
                    RedBullPlayer::Widgets::CommandButton *createDownloadAllButton();
                    RedBullPlayer::Widgets::CommandButton *createDeleteAllButton();

                protected:
                    QString resolveSizeString( uint size );
                    void resolveContentItemViewStyle( RedBullPlayer::Modules::ContentManagement::IContentItemView *itemView,
                                                      QString textStyleName = "ContentManagement_Item_Label" );
                    QString resolveDateString( QDateTime date );
                    QString resolveToolTipString( RedBullPlayer::Modules::MediaLibrary::IContent *content );

                private:
                    QWidget *_viewsParent;
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;
            };
        }
    }
}

#endif // CONTENTMANAGEMENTVIEWFACTORY_H
