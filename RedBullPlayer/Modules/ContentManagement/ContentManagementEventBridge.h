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
#ifndef CONTENTMANAGEMENTEVENTBRIDGE_H
#define CONTENTMANAGEMENTEVENTBRIDGE_H

#include <QObject>

#include "ContentManagementModel.h"
#include "../../Events/ContentManagement/BulkNewContentAvailableEvent.h"
#include "../../Events/ContentManagement/ContentDownloadStartedEvent.h"
#include "../../Events/ContentManagement/ContentDownloadProgressEvent.h"
#include "../../Events/ContentManagement/ContentDownloadCanceledEvent.h"
#include "../../Events/ContentManagement/NewContentAvailableEvent.h"
#include "../../Events/ContentManagement/RemoveContentEvent.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class ContentManagementEventBridge : public QObject {
                    Q_OBJECT
                public:
                    ContentManagementEventBridge( ContentManagementModel *model, QObject *parent = 0 );

                protected:
                    bool event( QEvent* event );

                    void onBulkNewContentAvailable( RedBullPlayer::Events::BulkNewContentAvailableEvent *e );
                    void onContentDownloadStarted( RedBullPlayer::Events::ContentDownloadStartedEvent *e );
                    void onContentDownloadProgress( RedBullPlayer::Events::ContentDownloadProgressEvent *e );
                    void onContentDownloadCanceled( RedBullPlayer::Events::ContentDownloadCanceledEvent *e );
                    void onNewContentAvailable( RedBullPlayer::Events::NewContentAvailableEvent *e );
                    void onRemoveContent( RedBullPlayer::Events::RemoveContentEvent *e );

                protected slots:
                    void emitInitiateContentDownload( QString contentId );
                    void emitSkipContentEvent( QString contentId );
                    void emitCancelContentDownload( QString contentId );
                    void emitDeleteContentDownload( QString contentId );

                private:
                    ContentManagementModel *_model;
            };
        }
    }
}

#endif // CONTENTMANAGEMENTEVENTBRIDGE_H
