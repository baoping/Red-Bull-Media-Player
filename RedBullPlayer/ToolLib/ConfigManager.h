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
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QSettings>
#include <QStringList>
#include <QUrl>
#include <QFileInfo>
#include <QImage>
#include <QUuid>
#include <QDateTime>
#include <QList>

#include "Configuration/TVChannel.h"

namespace RedBullPlayer {
    namespace Tools {
        class ConfigManager : public QObject {
                Q_OBJECT

            public:
                static ConfigManager* instance();

                //Common
                QString getApiKey() const {
                    return _apiKey;
                }

                //WhiteList
                bool isInWhiteList( QString fileName );
                bool isInWhiteList( QFileInfo fileInfo );

                bool isInVideoWhiteList( QString fileName );
                bool isInVideoWhiteList( QFileInfo fileInfo );

                bool isInAudioWhiteList( QString fileName );
                bool isInAudioWhiteList( QFileInfo fileInfo );

                bool isInImageWhiteList( QString fileName );
                bool isInImageWhiteList( QFileInfo fileInfo );

                bool isInDocumentWhiteList( QString fileName );
                bool isInDocumentWhiteList( QFileInfo fileInfo );

                bool isPdfDocument( QString fileName );
                bool isPdfDocument( QFileInfo fileInfo );

                bool isPowerpointDocument( QString fileName );
                bool isPowerpointDocument( QFileInfo fileInfo );

                bool isWordDocument( QString fileName );
                bool isWordDocument( QFileInfo fileInfo );

                bool isExcelDocument( QString fileName );
                bool isExcelDocument( QFileInfo fileInfo );

                bool isOtherDocument( QString fileName );
                bool isOtherDocument( QFileInfo fileInfo );

                QString getVideoWhiteList() const {
                    return _videoWhiteListString;
                }

                QString getAudioWhiteList() const {
                    return _audioWhiteListString;
                }

                QString getImageWhiteList() const {
                    return _imageWhiteListString;
                }

                QString getPdfDocumentWhiteList() const {
                    return _documentPdfWhiteListString;
                }

                QString getPowerpointDocumentWhiteList() const {
                    return _documentPowerpointWhiteListString;
                }

                QString getWordDocumentWhiteList() const {
                    return _documentWordWhiteListString;
                }

                QString getExcelDocuemntWhiteList() const {
                    return _documentExcelWhiteListString;
                }

                QString getOtherDocuemntWhiteList() const {
                    return _documentOtherWhiteListString;
                }

                QString getVideoNameFilter();
                QString getAudioNameFilter();
                QString getImageNameFilter();
                QString getDocumentNameFilter();
                QString getAllNameFilter();

                void setVideoWhiteList( QString videoWhiteList );
                void setAudioWhiteList( QString audioWhiteList );
                void setImageWhiteList( QString imageWhiteList );
                void setPdfDocuemntWhiteList( QString pdfDocumentWhiteList );
                void setPowerpointDocuemntWhiteList( QString powerpointDocuemntWhiteList );
                void setWordDocumetWhiteList( QString wordDocumentWhiteList );
                void setExcelDocumentWhiteList( QString excelDocumentWhiteList );
                void setOtherDocumentWhiteList( QString otherDocumentWhiteList );

                //Directories
                QString getMediaDirectory() const {
                    return _mediaDirectory;
                }

                QList<QString> mediaDirectories() const {
                    return _mediaDirectories;
                }

                void setMediaDirectories( QList<QString> mediaDirectories );
                QString getDeleteDirectory() const {
                    return _deleteDirectory;
                }

                QString getLastDialogDirectory() const {
                    return _lastDialogDirectory;
                }

                QString getTempDirectory() const {
                    return _tempDirectory;
                }

                void setLastDialogDirectory( QString directory );
                QString snapshotDestDir() const {
                    return _snapshotDestDir;
                }

                //Snapshot
                QString genericFileName() const {
                    return _genericFileName;
                }

                QString fileExt() const {
                    return _fileExt;
                }

                //ChapterSelect
                int shortVideoUpperLimit() const {
                    return _shortVideoUpperLimit;
                }

                int shortVideoChapterDuration() const {
                    return _shortVideoChapterDuration;
                }

                int middleVideoUpperLimit() const {
                    return _middleVideoUpperLimit;
                }

                int middleVideoChapterDuration() const {
                    return _middleVideoChapterDuration;
                }

                int longVideoChapterDuration() const {
                    return _longVideoChapterDuration;
                }

                //Playlist
                bool loopPlaylist() const {
                    return _loopPlaylist;
                }

                void setLoopPlaylist( bool loop );
                int staticContentLoopInterval() const {
                    return _staticContentLoopInterval;
                }

                void setStaticContentLoopInterval( int seconds );

                //About
                QString aboutTitle() const {
                    return _aboutTitle;
                }

                QString aboutFile() const {
                    return _aboutFile;
                }

                QString licenceTitle() const {
                    return _licenceTitle;
                }

                QString licenceFile() const {
                    return _licenceFile;
                }

                //UserSettingsDialog
                bool showDialogOnStart() const {
                    return _showDialogOnStart;
                }

                void setShowDialogOnStart( bool value );
                QString collectFile() const {
                    return _collectFile;
                }

                QString collectCheckFile() const {
                    return _collectCheckFile;
                }

                QString contentFile() const {
                    return _contentFile;
                }

                QString contentCheckFile() const {
                    return _contentCheckFile;
                }

                QString infoFile() const {
                    return _infoFile;
                }


                //GridPlayList
                int numberColumnsFormat4To3() const {
                    return _numberColumnsFormat4To3;
                }

                int numberRowsFormat4To3() const {
                    return _numberRowsFormat4To3;
                }

                int numberColumnsFormat16To9() const {
                    return _numberColumnsFormat16To9;
                }

                int numberRowsFormat16To9() const {
                    return _numberRowsFormat16To9;
                }

                // USB Stick
                QString getUSBStickName() const {
                    return _usbStickName;
                }

                // Media Library
                QString getMediaLibraryFileName() const {
                    return _mediaLibraryFileName;
                }

                float coverImageGenerationPostition() const {
                    return _coverImageGenerationPostition;
                }

                //PodCast
                bool podCastAvailable() const {
                    return _podCastAvailable;
                }

                QUrl getPodCastUrl() const {
                    return _podCastUrl;
                }

                bool alternativPodCastBehavior() const {
                    return _alternativPodCastBehavior;
                }

                int podCastPollInterval() const {
                    return _podCastPollInterval;
                }

                //RSSFeed
                bool rssFeedAvailable() const {
                    return _rssFeedAvailable;
                }

                QUrl getRssFeedUrl() const {
                    return _rssFeedUrl;
                }

                bool alternativeNewsBehavior() const {
                    return _alternativeNewsBehavior;
                }

                //TV - Channel
                int tvChannelCount() const {
                    return _tvChannels->count();
                }

                RedBullPlayer::Tools::Configuration::TV::TVChannel* tvChannelAt( int index ) {
                    return _tvChannels->at( index );
                }

                bool isASXStream( QString url );

                //Logging
                bool loggingEnabled() const {
                    return _loggingEnabled;
                }

                //Update behavior
                QDateTime getLastCheck() const {
                    return _lastCheck;
                }

                void setLastCheck( QDateTime lastCheck );
                int getInterval() const {
                    return _interval;
                }

                QString getSkipped() const {
                    return _skipped;
                }

                void setSkipped( QString skipped );

                //Newsticker
                bool newsTickerOn() const {
                    return _newsTickerOn;
                }

                //CoverViewPlaylist
                int numberItemsCoverViewFormat4To3() const {
                    return _numberItemsCoverViewFormat4To3;
                }

                int numberItemsCoverViewFormat16To9() const {
                    return _numberItemsCoverViewFormat16To9;
                }

                //StyleFactory
                QString styleConfigurationFileName() const {
                    return _styleConfigurationFileName;
                }

                //UI
                int hoverBoxWidth() const {
                    return _hoverBoxWidth;
                }

                int hoverBoxBorderWidth() const {
                    return _hoverBoxBorderWidth;
                }

                int hoverBoxContentRegionHeight() const {
                    return _hoverBoxContentRegionHeight;
                }

                QString stateDisplayIOHeader() const {
                    return _stateDisplayIOHeader;
                }

                QString stateDisplayDownloadHeader() const {
                    return _stateDisplayDownloadHeader;
                }

                QString stateDisplayUpdateHeader() const {
                    return _stateDisplayUpdateHeader;
                }

                QString windowStartState() const {
                    return _windowStartState;
                }

                void setWindowStartState( QString state );
                QString stateDisplayDiskSpaceHeader() const {
                    return _stateDisplayDiskSpaceHeader;
                }

                QString stateDisplayInternetAvailabilityHeader() const {
                    return _stateDisplayInternetAvailabilityHeader;
                }

                bool frameless() const {
                    return _frameless;
                }

                //InternetConnectionChecker
                QString referenceHost() const {
                    return _referenceHost;
                }

                int checkPeriod() const {
                    return _checkPeriod;
                }

                //Statistics
                QString getStatisticServerUrl() const {
                    return _statisticsServerUrl;
                }

                //Update
                QString getUpdateServerUrl() const {
                    return _updateServiceUrl;
                }

                //ServusTvB2B Button
                bool displayServusTvB2bButton() const {
                    return _displayServusTvB2bButton;
                }

                QString getServusTvB2BUrl() const {
                    return _servusTvB2BUrl;
                }

                //Settings
                QString mediaSettingsTitle() const {
                    return _mediaSettingsTitle;
                }

                QString generalSettingsTitle() const {
                    return _generalSettingsTitle;
                }

                QString rootPathReplacementText() const {
                    return _rootPathReplacementText;
                }

                //User Initiated Downloads
                QString newDownloadsTabTitle() const {
                    return _newDownloadsTabTitle;
                }

                QString downloadedTabTitle() const {
                    return _downloadedTabTitle;
                }

                QString skippedDownloadsTabTitle() const {
                    return _skippedTabTitle;
                }

                QString downloadedItemHeaderTitle() const {
                    return _downloadedItemHeaderTitle;
                }

                QString downloadedSizeHeaderTitle() const {
                    return _downloadedSizeHeaderTitle;
                }

                QString downloadedDateHeaderTitle() const {
                    return _downloadedDateHeaderTitle;
                }


            private:
                ConfigManager( QObject* parent = 0 );
                ~ConfigManager();

                void LoadSettings();
                void LoadPlayerSettings();
                void LoadUpdateSettings();

                QSettings* _playerSettings;

                //Common
                QString _apiKey;

                //WhiteList
                QStringList* _videoWhiteList;
                QStringList* _audioWhiteList;
                QStringList* _imageWhiteList;
                QStringList* _documentPdfWhiteList;
                QStringList* _documentPowerpointWhiteList;
                QStringList* _documentWordWhiteList;
                QStringList* _documentExcelWhiteList;
                QStringList* _documentOtherWhiteList;
                QString _videoWhiteListString;
                QString _audioWhiteListString;
                QString _imageWhiteListString;
                QString _documentPdfWhiteListString;
                QString _documentPowerpointWhiteListString;
                QString _documentWordWhiteListString;
                QString _documentExcelWhiteListString;
                QString _documentOtherWhiteListString;

                //Directories
                QString _mediaDirectory;
                QList<QString> _mediaDirectories;
                QString _deleteDirectory;
                QString _lastDialogDirectory;
                QString _tempDirectory;
                QString _snapshotDestDir;

                //Snapshot
                QString _genericFileName;
                QString _fileExt;

                //ChapterSelect
                int _shortVideoUpperLimit;
                int _shortVideoChapterDuration;
                int _middleVideoUpperLimit;
                int _middleVideoChapterDuration;
                int _longVideoChapterDuration;

                //Playlist
                bool _loopPlaylist;
                int _staticContentLoopInterval;

                //GridPlayList

                int _numberColumnsFormat4To3;
                int _numberRowsFormat4To3;
                int _numberColumnsFormat16To9;
                int _numberRowsFormat16To9;

                //About
                QString _aboutTitle;
                QString _aboutFile;
                QString _licenceTitle;
                QString _licenceFile;

                //UserSettingsDialog
                bool _showDialogOnStart;
                QString _collectFile;
                QString _collectCheckFile;
                QString _contentFile;
                QString _contentCheckFile;
                QString _infoFile;

                // USB Stick
                QString _usbStickName;

                //Media Library
                QString _mediaLibraryFileName;
                float _coverImageGenerationPostition;

                //PodCast
                bool _podCastAvailable;
                QUrl _podCastUrl;
                bool _alternativPodCastBehavior;
                int _podCastPollInterval;

                //RSSFeed
                bool _rssFeedAvailable;
                QUrl _rssFeedUrl;
                bool _alternativeNewsBehavior;

                //TV - Channel
                QList<RedBullPlayer::Tools::Configuration::TV::TVChannel*>* _tvChannels;

                //NewsTicker
                bool _newsTickerOn;

                //CoverViewPlaylist
                int _numberItemsCoverViewFormat4To3;
                int _numberItemsCoverViewFormat16To9;

                //StyleFactory
                QString _styleConfigurationFileName;
                //UI
                int _hoverBoxWidth;
                int _hoverBoxBorderWidth;
                int _hoverBoxContentRegionHeight;
                QString _stateDisplayIOHeader;
                QString _stateDisplayDownloadHeader;
                QString _stateDisplayUpdateHeader;
                bool _frameless;
                QString _windowStartState;
                QString _stateDisplayDiskSpaceHeader;
                QString _stateDisplayInternetAvailabilityHeader;

                //Logging
                bool _loggingEnabled;

                //Update
                QString _updateServiceUrl;

                //Update behavior
                QDateTime _lastCheck;
                int _interval;
                QString _skipped;

                //Tools
                void mkDirIfNotExists( QString dir );

                //InternetConnectionChecker
                QString _referenceHost;
                int _checkPeriod;

                // Statistics
                QString _statisticsServerUrl;

                //ServusTv B2B Button
                bool _displayServusTvB2bButton;
                QString _servusTvB2BUrl;

                //Settings
                QString _mediaSettingsTitle;
                QString _generalSettingsTitle;
                QString _rootPathReplacementText;

                //User Initiated Downloads
                QString _newDownloadsTabTitle;
                QString _downloadedTabTitle;
                QString _skippedTabTitle;
                QString _downloadedItemHeaderTitle;
                QString _downloadedSizeHeaderTitle;
                QString _downloadedDateHeaderTitle;

                static ConfigManager* _instance;
        };
    }
}


#endif // CONFIGMANAGER_H
