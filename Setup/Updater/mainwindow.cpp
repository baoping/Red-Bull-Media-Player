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
#include "mainwindow.h"

#include <QDebug>
#include <QThread>
#include <QtCore/QUrl>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>
#include <QtXml/QDomDocument>
#include <QMessageBox>
#include <QSettings>

using namespace RedBullPlayer::Updater;
using namespace RedBullPlayer::Tools::Network;

MainWindow::MainWindow( QWidget *parent )
    : QWidget( parent ) {
    /*QSettings* s = new QSettings( "config.ini", QSettings::IniFormat, parent );

    _playerVersion = new PlayerVersion( this, "version.nfo" );

    _downloader = new DownloadUtil( this );



    #if defined(Q_OS_MAC)
        QString dir = s->value("Playlist/macBaseDir", ".").toString();
    #else
        QString dir = s->value("Playlist/baseDir", ".").toString();
    #endif

    _downloader->setTargetDirectory(dir);

    _versionServiceClient = new VersionServiceClient( this );
    QObject::connect(_versionServiceClient,SIGNAL(responseReceived(QString)),
                     SLOT(slotResponseReceived(QString)),
                     Qt::QueuedConnection);
     QObject::connect(_versionServiceClient,SIGNAL(downloadFinished(QString)),
                     SLOT(slotDownloadFinished(QString)),
                     Qt::QueuedConnection);


    QVBoxLayout *layout = new QVBoxLayout(this);

    _button = new QPushButton();
    QObject::connect(_button,SIGNAL(clicked()),SLOT(slotButtonClicked()),Qt::QueuedConnection);

    _downloadButton = new QPushButton();
    QObject::connect(_downloadButton,SIGNAL(clicked()),SLOT(slotDownloadButtonClicked()),Qt::QueuedConnection);

    _readVersionFileButton = new QPushButton();
    QObject::connect(_readVersionFileButton,SIGNAL(clicked()),SLOT(slotReadVersionFileButtonClicked()),Qt::QueuedConnection);

    _fetchCastButton = new QPushButton();
    QObject::connect(_fetchCastButton,SIGNAL(clicked()),SLOT(slotfetchCastButtonClicked()),Qt::QueuedConnection);

    _button->setText("CheckForUpdate");
    _downloadButton->setText("DownloadFile");
    _readVersionFileButton->setText("Version");
    _fetchCastButton->setText("PodCatcher");

    _serverVersionLabel = new QLabel();
    _downloadUrlLabel = new QLabel();
    _isFullUpdateLabel = new QLabel();
    _clientIsUpToDateLabel = new QLabel();
    _fileHashLabel = new QLabel();
    _clientIdLabel = new QLabel();

    _httpStatusCodeLabel = new QLabel;

    layout->addWidget(_button);
    layout->addWidget(_httpStatusCodeLabel);
    layout->addWidget(_serverVersionLabel);
    layout->addWidget(_downloadUrlLabel);
    layout->addWidget(_isFullUpdateLabel);
    layout->addWidget(_clientIsUpToDateLabel);
    layout->addWidget(_fileHashLabel);
    layout->addWidget(_clientIdLabel);
    layout->addWidget(_downloadButton);
    layout->addWidget(_readVersionFileButton);
    layout->addWidget(_fetchCastButton);

    setLayout(layout);*/
}


MainWindow::~MainWindow() {
}

void MainWindow::slotResponseReceived( QString someString ) {
    /*_httpStatusCodeLabel->setText( QString("%1").arg(_versionServiceClient->getHttpStatusCode()) );

    _downloadUrlLabel->setText(_versionServiceClient->getDownloadUrl());
    _serverVersionLabel->setText(_versionServiceClient->getServerVersion());
    _isFullUpdateLabel->setText(QString(_versionServiceClient->getIsFullUpdate()?"True":"False"));
    _clientIsUpToDateLabel->setText(QString(_versionServiceClient->getClientIsUpToDate()?"True":"False"));
    _fileHashLabel->setText(_versionServiceClient->getFileHash());
    _clientIdLabel->setText(_versionServiceClient->getClientId());*/
}

void MainWindow::slotButtonClicked() {
    _versionServiceClient->checkForUpdate();
}

void MainWindow::slotDownloadButtonClicked() {
    //_versionServiceClient->downloadFile("c:\\kelkes.jpg");
}

void MainWindow::slotDownloadFinished( QString fileName ) {
    QMessageBox::information( this, "DownloadFinished", fileName );
}

void MainWindow::slotReadVersionFileButtonClicked() {
    QMessageBox::information( this, "PlayerVersion", _playerVersion->getVersionString() );
}

void MainWindow::slotfetchCastButtonClicked() {
    // _catcher->fetch();
    // _downloader->downloadFile(QUrl("http://woodenspears.com/wp-content/uploads/2009/07/megan-fox-panties.jpg"));
}



