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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>

#include "../UpdateServices/VersionServiceClient.h"
#include "../UpdateServices/PlayerVersion.h"
#include "../ToolLib/Network/DownloadUtil.h"


class QLabel;
class QPushButton;
class QNetworkAccessManager;
class QNetworkReply;

namespace RedBullPlayer {
    namespace Updater {

        class MainWindow : public QWidget {
                Q_OBJECT
            public:
                MainWindow( QWidget *parent = 0 );
                ~MainWindow();

            private slots:
                void slotResponseReceived( QString someString );
                void slotButtonClicked();
                void slotDownloadButtonClicked();
                void slotReadVersionFileButtonClicked();
                void slotDownloadFinished( QString fileName );
                void slotfetchCastButtonClicked();
            private:
                QPushButton *_button;
                QPushButton *_downloadButton;
                QLabel *_httpCodeLabel;
                QLabel *_downloadUrlLabel;
                QLabel *_fileHashLabel;
                QLabel *_serverVersionLabel;
                QLabel *_clientIsUpToDateLabel;
                QLabel *_isFullUpdateLabel;
                QLabel *_httpStatusCodeLabel;
                QLabel *_clientIdLabel;
                QPushButton *_readVersionFileButton;
                QPushButton *_fetchCastButton;

                PlayerVersion* _playerVersion;
                VersionServiceClient *_versionServiceClient;
                RedBullPlayer::Tools::Network::DownloadUtil* _downloader;

        };
    }
}

#endif // MAINWINDOW_H
