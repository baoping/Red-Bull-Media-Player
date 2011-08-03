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
#include "ConfigManager.h"

#include <QDir>
#include <QFile>
#include <QPixmap>
#include <QDebug>

#include "Network/DownloadUtil.h"
#include "helper.h"

using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Tools::Configuration::TV;

ConfigManager* ConfigManager::_instance = 0;

ConfigManager::ConfigManager( QObject* parent ) : QObject( parent ) {
    _playerSettings = new QSettings( "config.ini", QSettings::IniFormat, this );
    LoadSettings();
}

ConfigManager::~ConfigManager() {
    _videoWhiteList->clear();
    delete _videoWhiteList;
    _audioWhiteList->clear();
    delete _audioWhiteList;
    _imageWhiteList->clear();
    delete _imageWhiteList;
    _documentPdfWhiteList->clear();
    delete _documentPdfWhiteList;
    _documentPowerpointWhiteList->clear();
    delete _documentPowerpointWhiteList;
    _documentWordWhiteList->clear();
    delete _documentWordWhiteList;
    _documentExcelWhiteList->clear();
    delete _documentExcelWhiteList;
    _documentOtherWhiteList->clear();
    delete _documentOtherWhiteList;

    while ( !_tvChannels->isEmpty() )
        delete _tvChannels->takeFirst();

    delete _tvChannels;

    if ( _playerSettings ) {
        _playerSettings->sync();
        _playerSettings->deleteLater();
    }
}

ConfigManager* ConfigManager::instance() {
    if ( _instance == 0 ) {
        _instance = new ConfigManager( );
    }

    return _instance;
}

void ConfigManager::LoadSettings() {
    LoadPlayerSettings();
    LoadUpdateSettings();
}

void ConfigManager::LoadPlayerSettings() {
    bool ok;
    //WhiteList
    _videoWhiteList = new QStringList( _playerSettings->value( "WhiteList/video", "" )
                                       .toString()
                                       .toLower()
                                       .split( "/" ) );
    _videoWhiteListString = _playerSettings->value( "WhiteList/video", "" ).toString();
    _audioWhiteList = new QStringList( _playerSettings->value( "WhiteList/audio", "" )
                                       .toString()
                                       .toLower()
                                       .split( "/" ) );
    _audioWhiteListString = _playerSettings->value( "WhiteList/audio", "" ).toString();
    _imageWhiteList = new QStringList( _playerSettings->value( "WhiteList/image", "" )
                                       .toString()
                                       .toLower()
                                       .split( "/" ) );
    _imageWhiteListString = _playerSettings->value( "WhiteList/image", "" ).toString();
    _documentPdfWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentPdf", "" )
            .toString()
            .toLower()
            .split( "/" ) );
    _documentPdfWhiteListString = _playerSettings->value( "WhiteList/documentPdf", "" ).toString();
    _documentPowerpointWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentPowerpoint", "" )
            .toString()
            .toLower()
            .split( "/" ) );
    _documentPowerpointWhiteListString = _playerSettings->value( "WhiteList/documentPowerpoint", "" ).toString();
    _documentWordWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentWord", "" )
            .toString()
            .toLower()
            .split( "/" ) );
    _documentWordWhiteListString = _playerSettings->value( "WhiteList/documentWord", "" ).toString();
    _documentExcelWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentExcel", "" )
            .toString()
            .toLower()
            .split( "/" ) );
    _documentExcelWhiteListString = _playerSettings->value( "WhiteList/documentExcel", "" ).toString();
    _documentOtherWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentOther", "" )
            .toString()
            .toLower()
            .split( "/" ) );
    _documentOtherWhiteListString = _playerSettings->value( "WhiteList/documentOther", "" ).toString();
    //Common
    _apiKey = _playerSettings->value( "System/APIKey", "" ).toString();
    QString baseDir;
    //Directory
#if defined(Q_OS_MAC)
    baseDir = _playerSettings->value( "Playlist/macBaseDir", "." ).toString();
#else
    baseDir = _playerSettings->value( "Playlist/baseDir", "." ).toString();
#endif
    QStringList dirList = baseDir.split( '|', QString::SkipEmptyParts );
    _mediaDirectory = dirList.at( 0 );

    for( int i = 0; i < dirList.count(); i++ ) {
        QString temp = dirList.at( i );
        temp = QDir( temp ).absolutePath();
        _mediaDirectories.append( temp );
    }

#if defined(Q_OS_MAC)
    _deleteDirectory = _playerSettings->value( "Playlist/macDeleteDir", "." ).toString();
#else
    _deleteDirectory = _playerSettings->value( "Playlist/deleteDir", "." ).toString();
#endif
    _lastDialogDirectory = _playerSettings->value( "Playlist/lastDialogDirectory", "" ).toString();
    _tempDirectory = _playerSettings->value( "General/tempDir", "./temp/" ).toString();
    _mediaDirectory = QDir( _mediaDirectory ).absolutePath();
    _tempDirectory = QDir( _tempDirectory ).absolutePath();

    if( !_lastDialogDirectory.isEmpty() )
        _lastDialogDirectory = QDir( _lastDialogDirectory ).absolutePath();

#if defined(Q_OS_MAC)
    _snapshotDestDir = _playerSettings->value( "Player/MacSnapshotDestDir", "../../../Snapshots/" ).toString();
#else
    _snapshotDestDir = _playerSettings->value( "Player/SnapshotDestDir", "../Snapshots/" ).toString();
#endif
    _snapshotDestDir = QDir( _snapshotDestDir ).absolutePath();
    mkDirIfNotExists( _mediaDirectory );
    mkDirIfNotExists( _deleteDirectory );
    mkDirIfNotExists( _tempDirectory );
    mkDirIfNotExists( _snapshotDestDir );
    //SnapShot
    _genericFileName = _playerSettings->value( "Player/SnapshotGenericFileName", "snapshot_" ).toString();
    _fileExt = _playerSettings->value( "Player/SnapshotExtension", ".png" ).toString();
    //ChapterSelect
    _shortVideoUpperLimit = _playerSettings->value( "ChapterSelect/shortVideoUpperLimit", 300 ).toInt();
    _shortVideoChapterDuration = _playerSettings->value( "ChapterSelect/shortVideoChapterDuration", 5 ).toInt();
    _middleVideoUpperLimit = _playerSettings->value( "ChapterSelect/middleVideoUpperLimit", 1800 ).toInt();
    _middleVideoChapterDuration = _playerSettings->value( "ChapterSelect/middleVideoChapterDuration", 60 ).toInt();
    _longVideoChapterDuration = _playerSettings->value( "ChapterSelect/longVideoChapterDuration", 300 ).toInt();
    //Playlist
    _loopPlaylist =  _playerSettings->value( "Playlist/loopPlaylist", false ).toBool();
    _staticContentLoopInterval = _playerSettings->value( "Playlist/staticContentLoopInterval", 5 ).toInt();
    //About
    _aboutTitle = _playerSettings->value( "Playlist/aboutTitle", "" ).toString();
    _aboutFile = _playerSettings->value( "Playlist/aboutTextFileUrl", "" ).toString();
    _licenceTitle = _playerSettings->value( "Playlist/licenceTitle", "" ).toString();
    _licenceFile = _playerSettings->value( "Playlist/licenceTextFileUrl", "" ).toString();
    //UserSettingsDialog
    QString temp = _playerSettings->value( "UserSettings/showOnStart", "" ).toString();
    _showDialogOnStart = ( temp.isEmpty() ) ? true : ( temp == "true" );
    _collectFile = _playerSettings->value( "UserSettings/collectFile", "" ).toString();
    _collectCheckFile = _playerSettings->value( "UserSettings/collectCheckFile", "" ).toString();
    _contentFile = _playerSettings->value( "UserSettings/contentFile", "" ).toString();
    _contentCheckFile = _playerSettings->value( "UserSettings/contentCheckFile", "" ).toString();
    _infoFile = _playerSettings->value( "UserSettings/infoFile", "" ).toString();
    //GridPlayList
    _numberColumnsFormat4To3 = _playerSettings->value( "GridViewPlaylist/numberColumnsFormat4To3", 5 ).toInt();
    _numberRowsFormat4To3 = _playerSettings->value( "GridViewPlaylist/numberRowsFormat4To3", 2 ).toInt();
    _numberColumnsFormat16To9 = _playerSettings->value( "GridViewPlaylist/numberColumnsFormat16To9", 6 ).toInt();
    _numberRowsFormat16To9 = _playerSettings->value( "GridViewPlaylist/numberRowsFormat16To9", 2 ).toInt();
    // USB Stick
    _usbStickName = _playerSettings->value( "USBStickStick/USBStickName", "RedBull" ).toString();
    // Media Library
    _mediaLibraryFileName = QDir::cleanPath( QString( "%1/%2" ).arg( _mediaDirectory ).arg( _playerSettings->value( "MediaLibrary/MediaLibraryDatabaseFileName", "MediaLibrary.db" ).toString() ) );
    QString coverImageGenerationPostition = _playerSettings->value( "MediaLibrary/CoverImageGenerationPosition", "0.1" ).toString();
    _coverImageGenerationPostition = coverImageGenerationPostition.toFloat( &ok );

    if ( !ok )
        _coverImageGenerationPostition = 0.1;

    //TV
    _tvChannels = new QList<TVChannel*>();
    TVChannel* c1 = new TVChannel();
    c1->setName( _playerSettings->value( "Playlist/channel1Name", "" ).toString() );
    c1->setUrl( Helper::decodeUrl( _playerSettings->value( "Playlist/channel1Url", "" ).toString().toUtf8() ) );
    c1->setImage( new QImage( _playerSettings->value( "Playlist/channel1Thumbnail", "" ).toString() ) );
    /*TVChannel* c2 = new TVChannel();
    c2->setName( _playerSettings->value("Playlist/channel2Name","").toString() );
    c2->setUrl( Helper::decodeUrl(_playerSettings->value("Playlist/channel2Url","").toString().toUtf8()) );
    c2->setImage( new QImage(_playerSettings->value("Playlist/channel2Thumbnail","").toString()) );*/
    _tvChannels->append( c1 );
    //_tvChannels->append( c2 );
    //PodCast
    QString podCastStr = _playerSettings->value( "OnlineContent/podCast", "" ).toString();
    _podCastAvailable =  !podCastStr.isEmpty();

    if ( _podCastAvailable ) {
        QByteArray urlArray = podCastStr.toLatin1();
        _podCastUrl = QUrl::fromEncoded( urlArray, QUrl::TolerantMode );
    }

    _podCastPollInterval = _playerSettings->value( "OnlineContent/podCastPollInterval", 20000 ).toInt();
    _alternativPodCastBehavior = _playerSettings->value( "OnlineContent/alternativPodCastBehavior", false ).toBool();
    //RSSFeed
    QString rssFeedStr = _playerSettings->value( "OnlineContent/rssFeed", "" ).toString();
    _rssFeedAvailable =  !rssFeedStr.isEmpty();

    if ( _rssFeedAvailable ) {
        QByteArray urlArray = rssFeedStr.toLatin1();
        _rssFeedUrl = QUrl::fromEncoded( urlArray, QUrl::TolerantMode );
    }

    _alternativeNewsBehavior = _playerSettings->value( "OnlineContent/alternativeNewsBehavior", false ).toBool();
    //Newsticker
    int newsTickerOn = _playerSettings->value( "NewsTicker/Enabled", 0 ).toInt( &ok );
    _newsTickerOn = ( ok ) && ( newsTickerOn == 1 );
    //CoverViewPlaylist
    _numberItemsCoverViewFormat16To9 = _playerSettings->value( "CoverViewPlaylist/numberItemsFormat16To9", 7 ).toInt( &ok );
    _numberItemsCoverViewFormat4To3 = _playerSettings->value( "CoverViewPlaylist/numberItemsFormat4To3", 5 ).toInt( &ok );
    //StyleFactory
#if defined(Q_OS_WIN32)
    _styleConfigurationFileName = _playerSettings->value( "UI/styleConfigurationFileWin", "styles_Win.xml" ).toString();
#elif defined(Q_OS_MAC)
    _styleConfigurationFileName = _playerSettings->value( "UI/styleConfigurationFileMacx", "styles_Macx.xml" ).toString();
#elif defined(Q_OS_LINUX)
    _styleConfigurationFileName = _playerSettings->value( "UI/styleConfigurationFileLinux", "styles_Linux.xml" ).toString();
#endif
    //UI
    _hoverBoxWidth = _playerSettings->value( "UI/hoverBoxWidth", 295 ).toInt( &ok );
    _hoverBoxBorderWidth = _playerSettings->value( "UI/hoverBoxBorderWidth", 1 ).toInt( &ok );
    _hoverBoxContentRegionHeight = _playerSettings->value( "UI/hoverBoxContentRegionHeight", 400 ).toInt( &ok );
    _stateDisplayIOHeader = _playerSettings->value( "UI/stateDisplayIOHeader", "IO-Operations" ).toString();
    _stateDisplayDownloadHeader = _playerSettings->value( "UI/stateDisplayDownloadHeader", "Downloads" ).toString();
    _stateDisplayUpdateHeader = _playerSettings->value( "UI/stateDisplayUpdateHeader", "Player-Update" ).toString();
    _stateDisplayDiskSpaceHeader = _playerSettings->value( "UI/stateDisplayDiskSpaceHeader", "USB-Stick" ).toString();
    _stateDisplayInternetAvailabilityHeader = _playerSettings->value( "UI/stateDisplayInternetAvailableHeader", "Internet Available" ).toString();
    _frameless = ( _playerSettings->value( "frameLess", "no" ).toString() == "yes" );
    _windowStartState = _playerSettings->value( "windowStartState" ).toString();
    //Logging
    int loggingEnabled = _playerSettings->value( "Logging/Enabled", 0 ).toInt( &ok );
    _loggingEnabled = ( ok ) && ( loggingEnabled == 1 );
    //InternetConnectionChecker
    _referenceHost = _playerSettings->value( "InternetConnectionChecker/ReferenceHost", "www.google.com" ).toString();
    _checkPeriod = _playerSettings->value( "InternetConnectionChecker/CheckPeriod", 20000 ).toInt();
    // Statistics
    _statisticsServerUrl = _playerSettings->value( "Statistics/statisticsServerUrl", "" ).toString();
    // ServusTV B2B
    int displayServusTvButton = _playerSettings->value( "ServusTvB2B/buttonDisplay", 0 ).toInt( &ok );
    _displayServusTvB2bButton = ( ok ) && ( displayServusTvButton == 1 );
    _servusTvB2BUrl = _playerSettings->value( "ServusTvB2B/servusTvB2BUrl", "" ).toString();
    //Settings
    _mediaSettingsTitle = _playerSettings->value( "Settings/mediaSettingsTitle", "Media Settings" ).toString();
    _generalSettingsTitle = _playerSettings->value( "Settings/generalSettingsTitle", "General Settings" ).toString();
    _rootPathReplacementText = _playerSettings->value( "Settings/rootPathReplacementText", "Stick Root" ).toString();
    //User Initiated Downloads
    _newDownloadsTabTitle = _playerSettings->value( "UserInitiatedDownload/newDownloadsTabTitle", "Available Downloads" ).toString();
    _downloadedTabTitle = _playerSettings->value( "UserInitiatedDownload/downloadedTabTitle", "Downloaded" ).toString();
    _skippedTabTitle = _playerSettings->value( "UserInitiatedDownload/skippedDownloadsTabTitle", "Skipped" ).toString();
    _downloadedItemHeaderTitle = _playerSettings->value( "UserInitiatedDownload/downloadedItemHeaderTitle", "Item" ).toString();
    _downloadedSizeHeaderTitle = _playerSettings->value( "UserInitiatedDownload/downloadedSizeHeaderTitle", "Size" ).toString();
    _downloadedDateHeaderTitle = _playerSettings->value( "UserInitiatedDownload/downloadedDateHeaderTitle", "Downloaded on" ).toString();
}


void ConfigManager::LoadUpdateSettings() {
    _updateServiceUrl = _playerSettings->value( "PlayerUpdate/updateServiceUrl", "" ).toString();
    //Behavior
    QString lastCheck = _playerSettings->value( "PlayerUpdate/lastCheck", "" ).toString();

    if ( lastCheck.isEmpty() )
        _lastCheck = QDateTime::currentDateTime().addDays( -1000 );
    else
        _lastCheck = QDateTime::fromString( lastCheck, Qt::ISODate );

    _skipped = _playerSettings->value( "PlayerUpdate/skipped", "" ).toString();
    bool ok;
    int interval = _playerSettings->value( "PlayerUpdate/interval", 0 ).toInt( &ok );

    if ( !ok )
        interval = 0;

    _interval = interval;
}

/******************** WHITE LIST ********************/

bool ConfigManager::isInWhiteList( QString fileName ) {
    return isInVideoWhiteList( fileName ) || isInAudioWhiteList( fileName ) || isInImageWhiteList( fileName ) || isInDocumentWhiteList( fileName );
}

bool ConfigManager::isInVideoWhiteList( QString fileName ) {
    return isInVideoWhiteList( QFileInfo( fileName ) );
}

bool ConfigManager::isInAudioWhiteList( QString fileName ) {
    return isInAudioWhiteList( QFileInfo( fileName ) );
}

bool ConfigManager::isInImageWhiteList( QString fileName ) {
    return isInImageWhiteList( QFileInfo( fileName ) );
}

bool ConfigManager::isInDocumentWhiteList( QString fileName ) {
    return isInDocumentWhiteList( QFileInfo( fileName ) );
}

bool ConfigManager::isPdfDocument( QString fileName ) {
    return isPdfDocument( QFileInfo( fileName ) );
}

bool ConfigManager::isPowerpointDocument( QString fileName ) {
    return isPowerpointDocument( QFileInfo( fileName ) );
}

bool ConfigManager::isWordDocument( QString fileName ) {
    return isWordDocument( QFileInfo( fileName ) );
}

bool ConfigManager::isExcelDocument( QString fileName ) {
    return isExcelDocument( QFileInfo( fileName ) );
}

bool ConfigManager::isOtherDocument( QString fileName ) {
    return isOtherDocument( QFileInfo( fileName ) );
}

bool ConfigManager::isInWhiteList( QFileInfo fileInfo ) {
    return isInVideoWhiteList( fileInfo ) ||
           isInAudioWhiteList( fileInfo ) ||
           isInImageWhiteList( fileInfo ) ||
           isInDocumentWhiteList( fileInfo );
}

bool ConfigManager::isInVideoWhiteList( QFileInfo fileInfo ) {
    return _videoWhiteList->contains( fileInfo.suffix().toLower() );
}

bool ConfigManager::isInAudioWhiteList( QFileInfo fileInfo ) {
    return _audioWhiteList->contains( fileInfo.suffix().toLower() );
}

bool ConfigManager::isInImageWhiteList( QFileInfo fileInfo ) {
    return _imageWhiteList->contains( fileInfo.suffix().toLower() );
}

bool ConfigManager::isInDocumentWhiteList( QFileInfo fileInfo ) {
    return isPdfDocument( fileInfo ) ||
           isPowerpointDocument( fileInfo ) ||
           isWordDocument( fileInfo ) ||
           isExcelDocument( fileInfo ) ||
           isOtherDocument( fileInfo );
}

bool ConfigManager::isPdfDocument( QFileInfo fileInfo ) {
    return _documentPdfWhiteList->contains( fileInfo.suffix().toLower() );
}

bool ConfigManager::isPowerpointDocument( QFileInfo fileInfo ) {
    return _documentPowerpointWhiteList->contains( fileInfo.suffix().toLower() );
}

bool ConfigManager::isWordDocument( QFileInfo fileInfo ) {
    return _documentWordWhiteList->contains( fileInfo.suffix().toLower() );
}

bool ConfigManager::isExcelDocument( QFileInfo fileInfo ) {
    return _documentExcelWhiteList->contains( fileInfo.suffix().toLower() );
}

bool ConfigManager::isOtherDocument( QFileInfo fileInfo ) {
    return _documentOtherWhiteList->contains( fileInfo.suffix().toLower() );
}

void ConfigManager::setVideoWhiteList( QString videoWhiteList ) {
    _videoWhiteListString = videoWhiteList;
    _playerSettings->setValue( "WhiteList/video", videoWhiteList );
    _videoWhiteList = new QStringList( _playerSettings->value( "WhiteList/video", "" )
                                       .toString()
                                       .toLower()
                                       .split( "/" ) );
}

void ConfigManager::setAudioWhiteList( QString audioWhiteList ) {
    _audioWhiteListString = audioWhiteList;
    _playerSettings->setValue( "WhiteList/audio", audioWhiteList );
    _audioWhiteList = new QStringList( _playerSettings->value( "WhiteList/audio", "" )
                                       .toString()
                                       .toLower()
                                       .split( "/" ) );
}

void ConfigManager::setImageWhiteList( QString imageWhiteList ) {
    _imageWhiteListString = imageWhiteList;
    _playerSettings->setValue( "WhiteList/image", imageWhiteList );
    _imageWhiteList = new QStringList( _playerSettings->value( "WhiteList/image", "" )
                                       .toString()
                                       .toLower()
                                       .split( "/" ) );
}

void ConfigManager::setPdfDocuemntWhiteList( QString pdfDocumentWhiteList ) {
    _documentPdfWhiteListString = pdfDocumentWhiteList;
    _playerSettings->setValue( "WhiteList/documentPdf", pdfDocumentWhiteList );
    _documentPdfWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentPdf", "" )
            .toString()
            .toLower()
            .split( "/" ) );
}

void ConfigManager::setPowerpointDocuemntWhiteList( QString powerpointDocumentWhiteList ) {
    _documentPowerpointWhiteListString = powerpointDocumentWhiteList;
    _playerSettings->setValue( "WhiteList/documentPowerpoint", powerpointDocumentWhiteList );
    _documentPowerpointWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentPowerpoint", "" )
            .toString()
            .toLower()
            .split( "/" ) );
}

void ConfigManager::setWordDocumetWhiteList( QString wordDocumentWhiteList ) {
    _documentWordWhiteListString = wordDocumentWhiteList;
    _playerSettings->setValue( "WhiteList/documentWord", wordDocumentWhiteList );
    _documentWordWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentWord", "" )
            .toString()
            .toLower()
            .split( "/" ) );
}

void ConfigManager::setExcelDocumentWhiteList( QString excelDocumentWhiteList ) {
    _documentExcelWhiteListString = excelDocumentWhiteList;
    _playerSettings->setValue( "WhiteList/documentExcel", excelDocumentWhiteList );
    _documentExcelWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentExcel", "" )
            .toString()
            .toLower()
            .split( "/" ) );
}

void ConfigManager::setOtherDocumentWhiteList( QString otherDocumentWhiteList ) {
    _documentOtherWhiteListString = otherDocumentWhiteList;
    _playerSettings->setValue( "WhiteList/documentOther", otherDocumentWhiteList );
    _documentOtherWhiteList = new QStringList( _playerSettings->value( "WhiteList/documentOther", "" )
            .toString()
            .toLower()
            .split( "/" ) );
}

QString ConfigManager::getVideoNameFilter() {
    QString filter = "";
    int i;

    for( i = 0; i < _videoWhiteList->count(); i++ ) {
        filter += " *." + _videoWhiteList->at( i );
    }

    filter = filter.trimmed();
    return "Video (" + filter + ")";
}

QString ConfigManager::getAudioNameFilter() {
    QString filter = "";
    int i;

    for( i = 0; i < _audioWhiteList->count(); i++ ) {
        filter += " *." + _audioWhiteList->at( i );
    }

    filter = filter.trimmed();
    return "Audio (" + filter + ")";
}

QString ConfigManager::getImageNameFilter() {
    QString filter = "";
    int i;

    for( i = 0; i < _imageWhiteList->count(); i++ ) {
        filter += " *." + _imageWhiteList->at( i );
    }

    filter = filter.trimmed();
    return "Image (" + filter + ")";
}

QString ConfigManager::getDocumentNameFilter() {
    QString filter = "";
    int i;

    for( i = 0; i < _documentPdfWhiteList->count(); i++ ) {
        filter += " *." + _documentPdfWhiteList->at( i );
    }

    for( i = 0; i < _documentPowerpointWhiteList->count(); i++ ) {
        filter += " *." + _documentPowerpointWhiteList->at( i );
    }

    for( i = 0; i < _documentWordWhiteList->count(); i++ ) {
        filter += " *." + _documentWordWhiteList->at( i );
    }

    for( i = 0; i < _documentExcelWhiteList->count(); i++ ) {
        filter += " *." + _documentExcelWhiteList->at( i );
    }

    for( i = 0; i < _documentOtherWhiteList->count(); i++ ) {
        filter += " *." + _documentOtherWhiteList->at( i );
    }

    filter = filter.trimmed();
    return "Document (" + filter + ")";
}

QString ConfigManager::getAllNameFilter() {
    QString filter = "";
    int i;

    for( i = 0; i < _videoWhiteList->count(); i++ ) {
        filter += " *." + _videoWhiteList->at( i );
    }

    for( i = 0; i < _audioWhiteList->count(); i++ ) {
        filter += " *." + _audioWhiteList->at( i );
    }

    for( i = 0; i < _imageWhiteList->count(); i++ ) {
        filter += " *." + _imageWhiteList->at( i );
    }

    for( i = 0; i < _documentPdfWhiteList->count(); i++ ) {
        filter += " *." + _documentPdfWhiteList->at( i );
    }

    for( i = 0; i < _documentPowerpointWhiteList->count(); i++ ) {
        filter += " *." + _documentPowerpointWhiteList->at( i );
    }

    for( i = 0; i < _documentWordWhiteList->count(); i++ ) {
        filter += " *." + _documentWordWhiteList->at( i );
    }

    for( i = 0; i < _documentExcelWhiteList->count(); i++ ) {
        filter += " *." + _documentExcelWhiteList->at( i );
    }

    for( i = 0; i < _documentOtherWhiteList->count(); i++ ) {
        filter += " *." + _documentOtherWhiteList->at( i );
    }

    filter = filter.trimmed();
    return "All supported files (" + filter + ")";
}

/******************** DIRECTORY ********************/

void ConfigManager::setLastDialogDirectory( QString directory ) {
    _lastDialogDirectory = directory;
    _playerSettings->setValue( "Playlist/lastDialogDirectory", _lastDialogDirectory );
}

/******************** TV ********************/

bool ConfigManager::isASXStream( QString url ) {
    return ( QFileInfo( QUrl( url ).path() ).suffix().toLower() == "asx" );
}


/******************** Update behavior ********************/

void ConfigManager::setLastCheck( QDateTime lastCheck ) {
    _lastCheck = lastCheck;
    _playerSettings->setValue( "PlayerUpdate/LastCheck", _lastCheck.toString( Qt::ISODate ) );
}

void ConfigManager::setSkipped( QString skipped ) {
    _skipped = skipped;
    _playerSettings->setValue( "PlayerUpdate/skipped", _skipped );
}

/******************** UserSettings ********************/

void ConfigManager::setShowDialogOnStart( bool value ) {
    _showDialogOnStart = value;
    _playerSettings->setValue( "UserSettings/showOnStart", value ? "true" : "false" );
}


/******************* Config Save Values ***************/
void ConfigManager::setWindowStartState( QString state ) {
    _windowStartState = state;
    _playerSettings->setValue( "windowStartState", state );
}

void ConfigManager::setLoopPlaylist( bool loop ) {
    _loopPlaylist = loop;
    _playerSettings->setValue( "Playlist/loopPlaylist", loop );
}

void ConfigManager::setMediaDirectories( QList<QString> mediaDirectories ) {
    _mediaDirectories = mediaDirectories;
    QStringList mediaDirectoriesStringList;
    QStringList mediaDirectoriesStringListOtherOS;
    foreach( QString dir, mediaDirectories ) {
        mediaDirectoriesStringList.append( ( QDir::current().relativeFilePath( dir ) + "/" ) );
#if defined(Q_OS_MAC)
        mediaDirectoriesStringListOtherOS.append( ( ( QDir::current().relativeFilePath( dir ) ).remove( 0, 6 ) + "/" ) );
#else
        mediaDirectoriesStringListOtherOS.append( ( "../../" + QDir::current().relativeFilePath( dir )  + "/" ) );
#endif
    }
    QString mediaDirectoriesString = mediaDirectoriesStringList.join( "|" );
    QString mediaDirectoriesStringOtherOS = mediaDirectoriesStringListOtherOS.join( "|" );
#if defined(Q_OS_MAC)
    _playerSettings->setValue( "Playlist/macBaseDir", mediaDirectoriesString );
    _playerSettings->setValue( "Playlist/baseDir", mediaDirectoriesStringOtherOS );
#else
    _playerSettings->setValue( "Playlist/baseDir", mediaDirectoriesString );
    _playerSettings->setValue( "Playlist/macBaseDir", mediaDirectoriesStringOtherOS );
#endif
}

void ConfigManager::setStaticContentLoopInterval( int seconds ) {
    _staticContentLoopInterval = seconds;
    _playerSettings->setValue( "Playlist/staticContentLoopInterval", seconds );
}

/******************** UTIL ********************/

void ConfigManager::mkDirIfNotExists( QString dir ) {
    QDir directory = QDir( dir );

    if( !directory.exists() ) {
        bool success = directory.mkpath( directory.absolutePath() );

        if( !success ) {
            //log error
        }
    }
}


