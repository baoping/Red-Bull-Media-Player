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
#ifndef XMLNEWSFEEDMODULE_H
#define XMLNEWSFEEDMODULE_H

#include "../../Interfaces/IXmlNewsFeed.h"

// Qt includes
#include <QObject>
#include <QDomElement>
#include <QEvent>
#include <QUuid>
#include <QTimer>

// Project includes
#include "RssFeedParserQueue.h"
#include "RssFeedParserEventBridge.h"
#include "RssFeedToNewsConverter.h"

#include "../../Events/RssFeed/RssFeedParseResponseEvent.h"
#include "../../Events/InternetConnectionStatusChangedEvent.h"
#include "../../Interfaces/IInternetConnectionChecker.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/RssFeed/IRssFeed.h"
#include "../../Interfaces/MediaLibrary/INews.h"
#include "../../Interfaces/MediaLibrary/INewsImage.h"
#include "../../Events/UserSettings/UserSettingsChangedEvent.h"
#include "../../PlayerShell/Shell.h"

using namespace RedBullPlayer::PlayerShell;


namespace RedBullPlayer {
    namespace Modules {
        namespace XmlNewsFeed {

            class XmlNewsFeedModule : public IXmlNewsFeed {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::XmlNewsFeed::IXmlNewsFeed )
                public:
                    XmlNewsFeedModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "XmlNewsFeed";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                protected:
                    bool event( QEvent * e );

                    void onRssFeedParseResponse( RedBullPlayer::Events::RssFeedParseResponseEvent* ev );
                    void onInternetConnectionStatusChanged( RedBullPlayer::Events::InternetConnectionStatusChangedEvent* ev );
                    void onUserSettingsChanged( RedBullPlayer::Events::UserSettingsChangedEvent *event );

                private:
                    Shell* _shell;

                    RssFeedParserQueue* _parserQueue;
                    RssFeedParserEventBridge* _parserEventBridge;
                    RssFeedToNewsConverter* _converter;

                    RedBullPlayer::Tools::RSS::IRssFeed* _currentFeed;
                    int _currentFeedItemIndex;
                    bool _newFeedItem;
                    bool _internetAvailable;
                    bool _allowDownload;

                    QUuid _lastRequestGuid;

                    QTimer* _checkTimer;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                private: //functions
                    bool resolveMediaLibrary();
                    void doRequest();
                    void saveRssFeedToDb( RedBullPlayer::Tools::RSS::IRssFeed* rssFeed );
                    void processNextRssFeedItem();
                    void resolveConnectivityChanged();


                private slots:
                    void onTimer();
                    void saveNewsToDb( RedBullPlayer::Modules::MediaLibrary::INews* news, QList<RedBullPlayer::Modules::MediaLibrary::INewsImage*>* newsImages );
            };
        }
    }
}

#endif // XMLNEWSFEEDMODULE_H
