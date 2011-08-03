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
#ifndef CONSTANTS_H
#define CONSTANTS_H

//QT Includes
#include <QString>
#include <QColor>
#include <QStringList>

namespace RedBullPlayer {
    namespace Container {
        class Constants {
            public:
                Constants() {};
                // General Contstns
                static QString GENERAL_TITLE_FONT;
                static QString GENERAL_TITLE_NOMAINREGIONFOCUS_COLOR;
                static QString GENERAL_TITLE_NOMAINREGIONFOCUS_COLOR_HTMLHEXCODE;

                // Function Select View
                static QString FUNCTION_SELECT_NAVIGATION_LEFT_ARROW_URL;
                static QString FUNCTION_SELECT_NAVIGATION_RIGHT_ARROW_URL;
                static QString FUNCTION_SELECT_IMAGE_DIR;
                static QString FUNCTION_SELECT_TV_URL;
                static QString FUNCTION_SELECT_TV_TITLE;
                static QString FUNCTION_SELECT_MEDIA_URL;
                static QString FUNCTION_SELECT_MEDIA_TITLE;
                static QString FUNCTION_SELECT_ABOUT_URL;
                static QString FUNCTION_SELECT_ABOUT_TITLE;
                static QString FUNCTION_SELECT_NEWS_URL;
                static QString FUNCTION_SELECT_NEWS_TITLE;
                static QString FUNCTION_SELECT_SERVUSTVB2B_URL;
                static QString FUNCTION_SELECT_SERVUSTVB2B_TITLE;
                static QString FUNCTION_SELECT_SETTINGS_URL;
                static QString FUNCTION_SELECT_SETTINGS_TITLE;
                static int FUNCTION_SELECT_ANIMATION_LENGTH;
                static QString FUNCTION_SELECT_CIRCLE;

                // Playlist UI
                static int PLAYLISTUI_ARROW_WIDTH;
                static int PLAYLISTUI_ARROW_HEIGHT;
                static int PLAYLISTUI_ARROW_MARGIN;
                static int ARROW_ALPHA;
                static QString PLAYLISTUI_IMAGE_DIR;
                static QString PLAYLISTUI_NAVIGATION_LEFT_ARROW_URL;
                static QString PLAYLISTUI_NAVIGATION_LEFT_ARROW_DISABLED_URL;
                static QString PLAYLISTUI_NAVIGATION_RIGHT_ARROW_URL;
                static QString PLAYLISTUI_NAVIGATION_RIGHT_ARROW_DISABLED_URL;
                static int PLAYLISTUI_ITEM_COUNT;
                static int PLAYLISTUI_ITEM_GAP;
                static float PLAYLISTUI_ITEM_RATIO;
                static int PLAYLISTUI_ANIMATION_LENGTH;

                static QString PLAYLISTUI_COVERFLOW_ICON;
                static QString PLAYLISTUI_COVERFLOW_ICON_CHECKED;
                static QString PLAYLISTUI_COVERFLOW_ICON_DISABLED;

                static QString PLAYLISTUI_GRID_VIEW_ICON;
                static QString PLAYLISTUI_GRID_VIEW_ICON_CHECKED;
                static QString PLAYLISTUI_GRID_VIEW_ICON_DISABLED;

                static QString PLAYLISTUI_LIST_VIEW_ICON;
                static QString PLAYLISTUI_LIST_VIEW_ICON_CHECKED;
                static QString PLAYLISTUI_LIST_VIEW_ICON_DISABLED;

                static QString PLAYLISTUI_PLUS_ICON;
                static QString PLAYLISTUI_PLUS_ICON_MOUSEOVER;
                static QString PLAYLISTUI_PLUS_ICON_DISABLED;

                static QString PLAYLISTUI_MINUS_ICON;
                static QString PLAYLISTUI_MINUS_ICON_MOUSEOVER;
                static QString PLAYLISTUI_MINUS_ICON_DISABLED;

                static QString PLAYLISTUI_DELETE_YES_ICON_URL;
                static QString PLAYLISTUI_DELETE_NO_ICON_URL;
                static QString PLAYLISTUI_DELETE_BIN_ICON_URL;
                static QString PLAYLISTUI_DELETE_BG_URL;
                static QString PLAYLISTUI_SURE;
                static QString PLAYLISTUI_SURE_ONLYPLAYLIST;

                // Controll Bar
                static int CONTROLL_BAR_WIDTH;
                static int CONTROLL_BAR_HEIGHT;
                static int CONTROLL_BAR_BOTTOM; // distance between screen bottom end and controllbar bottom end
                static QColor CONTROLL_BAR_BG_COLOR;
                static int CONTROLL_BAR_ANIMATION_LENGTH;
                static QString CONTROLL_BAR_IMAGE_DIR;
                static QString CONTROLL_BAR_BACKWARD;
                static QString CONTROLL_BAR_PLAY;
                static QString CONTROLL_BAR_PAUSE;
                static QString CONTROLL_BAR_FORWARD;
                static QString CONTROLL_BAR_PREVIEW;
                static QString CONTROLL_BAR_SNAPSHOT;
                static QString CONTROLL_BAR_FULLSCREEN;
                static QString CONTROLL_BAR_RESOLUTION;
                static QString CONTROLL_BAR_VOLUME;
                static QString CONTROLL_BAR_BACKGROUND;
                static int CONTROLL_BAR_HIDE_PERIOD;
                static QString CONTROLL_BAR_BUFFERING_TITLE;

                static QString SEEK_BAR_IMAGE_DIR;
                static QString SEEK_BAR_BG_BORDER;
                static QString SEEK_BAR_PLAY_HEAD;
                static QString SEEK_BAR_BUFFERING_ANIMATION;
                static int SEEK_BAR_DRAGGING_TIMEOUT;

                // Volume Bar
                static int VOLUME_BAR_WIDTH;
                static int VOLUME_BAR_HEIGHT;
                static QColor VOLUME_BAR_INACTIVE_COLOR;
                static QColor VOLUME_BAR_ACTIVE_COLOR;
                static QColor VOLUME_BAR_MOUSE_OVER_COLOR;
                static int VOLUME_BAR_START_LEVEL;

                static QString TOP_UI_IMAGE_DIR;
                static QString TOP_UI_LOGO_FILENAME;
                static QString TOP_UI_EXIT_FILENAME;
                static QString TOP_UI_BG_FILENAME;
                static QString TOP_UI_OVERLAY_IMAGE;
                static QString TOP_UI_TOPLINE_IMAGE;

                static QString SEARCH_UI_IMAGE_DIR;
                static QString SEARCH_UI_SEARCHBOX_FILENAME;
                static QString SEARCH_UI_BG_FILENAME;
                static QString SEARCH_UI_FLOWER_BG_FILENAME;
                static QString SEARCH_UI_SEARCHBOX_BG_FILENAME;
                static QString SEARCH_UI_SEARCHBOX_HEAD_FILENAME;
                static QString SEARCH_UI_SEARCHBOX_COVER_FILENAME;
                static QString SEARCH_UI_SEARCHBOX_X;
                static QString SEARCH_UI_SEARCHBOX_X_PRESSED;
                static QString SEARCH_UI_GRADIENT_LINE;

                static QString ROOT_UI_IMAGE_DIR;
                static QString ROOT_UI_BACKGROUNDIMAGE;
                static QString ROOT_UI_HORIZONTAL_GRADIENT_LINE;
                static QString ROOT_UI_VERTICAL_GRADIENT_LINE;
                static QString ROOT_UI_TEXT_DIR;
                static QString DISKSPACE_FULL_TEXT;

                static int CHAPTER_SELECT_WIDTH;
                static int CHAPTER_SELECT_HEIGHT;
                static int CHAPTER_SELECT_BOTTOM;
                static QColor CHAPTER_SELECT_BG_COLOR;
                static int CHAPTER_SELECT_ARROW_WIDTH;
                static int CHAPTER_SELECT_ARROW_HEIGHT;
                static QString CHAPTER_SELECT_IMAGE_DIR;
                static QString CHAPTER_SELECT_LEFT_ARROW_URL;
                static QString CHAPTER_SELECT_LEFT_ARROW_DISABLED_URL;
                static QString CHAPTER_SELECT_RIGHT_ARROW_URL;
                static QString CHAPTER_SELECT_RIGHT_ARROW_DISABLED_URL;
                static int CHAPTER_SELECT_ITEMS_COUNT;
                static int CHAPTER_SELECT_ITEMS_MARGIN;
                static QColor CHAPTER_SELECT_ACTIVE_ITEM_BG_COLOR;
                static float CHAPTER_SELECT_ANIMATION_SPEED;
                static QString CHAPTER_SELECT_DEFAULT_PREVIEW_IMAGE;
                static QString CHAPTER_SELECT_BACKGROUND_IMAGE;

                static QString BOTTOM_UI_IMAGE_DIR;
                static QString BOTTOM_UI_BG_URL;
                static QString BOTTOM_UI_RESIZE_URL;
                static QString BOTTOM_UI_OVERLAY_IMAGE;
                static QString BOTTOM_UI_UPPERGRADIENT_URL;
                static QString BOTTOM_UI_LOWERGRADIENT_URL;

                static QString CENTRAL_UI_IMAGE_DIR;
                static QString CENTRAL_UI_OVERLAY_IMAGE;
                static QString CENTRAL_UI_PLAYLIST_BACKGROUND_UNFOCUSED_COLOR;
                static QString CENTRAL_UI_PLAYLIST_BACKGROUND_FOCUSED_COLOR;
                static QString CENTRAL_UI_BACKGROUND_IMAGE;

                static QString PLAYLER_CLOSE_BUTTON_IMAGE_DIR;
                static QString PLAYLER_CLOSE_BUTTON_X_URL;
                static QString PLAYLER_CLOSE_BUTTON_BACK_2_PLAYLIST;
                static int PLAYLER_CLOSE_BUTTON_TOP;
                static int PLAYLER_CLOSE_BUTTON_RIGHT;

                static float PLAYER_FORWARD_RATES[6];
                static int PLAYER_FORWARD_RATES_COUNT;
                static float PLAYER_BACKWARD_RATES[6];
                static int PLAYER_BACKWARD_RATES_COUNT;
                static int PLAYER_BACKWARD_STEP;
                static int PLAYER_BACKWARD_TIMER_PERIOD;
                static int PLAYER_FORWARD_STEP;
                static int PLAYER_FORWARD_TIMER_PERIOD;
                static int PLAYER_AUDIO_BACKWARD_STEP;
                static int PLAYER_AUDIO_FORWARD_STEP;

                static QString PLAYER_UI_IMAGE_DIR;
                static QString PLAYER_UI_IMAGE_ANIMATION;

                static QString PLAYLIST_CREATOR_IMAGE_DIR;
                static QString PLAYLIST_CREATOR_NEWS_PLACEHOLDER;
                static QString PLAYLIST_CREATOR_ABOUT_PLACEHOLDER;
                static QString PLAYLIST_CREATOR_LICENSE_PLACEHOLDER;
                static QString PLAYLIST_CREATOR_GENERAL_SETTINGS_PLACEHOLDER;
                static QString PLAYLIST_CREATOR_MEDIA_SETTINGS_PLACEHOLDER;

                static QString SNAPSHOT_CRETATION_STATE_ELEMENT;
                static QString SNAPSHOT_CRETATION_SNAPSHOTCREATED_ELEMENT;
                static QString SNAPSHOT_CRETATION_REQUESTID_ATTRIBUTE;
                static QString SNAPSHOT_CRETATION_FILENAME_ATTRIBUTE;
                static QString SNAPSHOT_CRETATION_ERROR_ELEMENT;
                static QString SNAPSHOT_CRETATION_STATE_READY_VALUE;
                static QString SNAPSHOT_CRETATION_STATE_BUSY_VALUE;

                static QString MEDIA_TYPE_VIDEO_NAME;
                static QString MEDIA_TYPE_AUDIO_NAME;
                static QString MEDIA_TYPE_IMAGE_NAME;
                static QString MEDIA_TYPE_TV_NAME;
                static QString MEDIA_TYPE_DOCUMENT_NAME;

                static QString MEDIADIRECTORY_PATH_PLACEHOLDER;

                // Metadata XML
                static QString METADATA_RECEIVE_METADATARECEIVED_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_title_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_artist_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_genre_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_copyright_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_album_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_trackNumber_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_description_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_rating_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_date_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_setting_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_url_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_language_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_nowPlaying_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_publisher_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_encodedBy_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_artworkUrl_ELEMENT;
                static QString METADATA_RECEIVE_METADATARECEIVED_trackID_ELEMENT;



                // List View
                static QString LISTVIEW_PLAYLIST_IMAGE_DIR;
                static QString LISTVIEW_PLAYLIST_MOVIE_ICON;
                static QString LISTVIEW_PLAYLIST_PICTURE_ICON;
                static QString LISTVIEW_PLAYLIST_MUSIC_ICON;
                static QString LISTVIEW_PLAYLIST_NEW_ICON;
                static QString LISTVIEW_PLAYLIST_HEADER_BACKGROUND_IMAGE;
                static QString LISTVIEW_PLAYLIST_MAINFOCUS_OVERLAY;
                static QString LISTVIEW_PLAYLIST_FONT_NAME;
                static int LISTVIEW_PLAYLIST_FONT_SIZE;
                static int LISTVIEW_PLAYLIST_FONT_SIZE_HEADER;
                static int LISTVIEW_PLAYLIST_ITEM_HEIGHT;
                static int LISTVIEW_PLAYLIST_COLUMN_PADDING;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ICON;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_TITLE;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ARTIST;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ALBUM;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_CREATED_AT;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_LENGTH;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_DOWNLOAD;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_IS_NEW;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_FILENAME;
                static int LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_FILESIZE;
                static int LISTVIEW_PLAYLIST_PADDING_TO_NAVIGATION_ARROW;
                static QString LISTVIEW_PLAYLIST_COLUMN_TITLE_HEADER_TEXT;
                static QString LISTVIEW_PLAYLIST_COLUMN_ARTIST_HEADER_TEXT;
                static QString LISTVIEW_PLAYLIST_COLUMN_ALBUM_HEADER_TEXT;
                static QString LISTVIEW_PLAYLIST_COLUMN_CREATEDAT_HEADER_TEXT;
                static QString LISTVIEW_PLAYLIST_COLUMN_LENGTH_HEADER_TEXT;
                static QString LISTVIEW_PLAYLIST_COLUMN_DOWNLOAD_HEADER_TEXT;
                static QString LISTVIEW_PLAYLIST_COLUMN_ISNEW_HEADER_TEXT;
                static QString LISTVIEW_PLAYLIST_COLUMN_FILENAME_HEADER_TEXT;
                static QString LISTVIEW_PLAYLIST_COLUMN_FILESIZE_HEADER_TEXT;
                static QString LISTVIEW_PLAYLIST_DATE_DISPLAY_FORMAT;
                static int LISTVIEW_PLAYLIST_SPACE_BETWEEN_PAGES;
                static int LISTVIEW_PLAYLIST_ICON_HEIGHT_PROCENT_FROM_BOUNDINGBOX;
                static int LISTVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH;

                // Grid View
                static int GRIDVIEW_PLAYLIST_ROWS;
                static int GRIDVIEW_PLAYLIST_COLUMS;
                static int GRIDVIEW_PLAYLIST_ITEM_PADDING;
                static int GRIDVIEW_PLAYLIST_ITEM_TITLE_HEIGHT_PROCENT;
                static int GRIDVIEW_PLAYLIST_ITEM_ISNEW_ICON_WIDTH_PROCENT_FROM_THUMBNAIL_WIDTH;
                static int GRIDVIEW_PLAYLIST_DISTANCE_FROM_NAVIGATION_KEY;
                static int GRIDVIEW_PLAYLIST_DISTANCE_BETWEEN_PAGES;
                static int GRIDVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH;
                static QString GRIDVIEW_PLAYLIST_IMAGE_DIR;
                static QString GRIDVIEW_PLAYLIST_OVERLAYPATTERN;
                static QString GRIDVIEW_PLAYLIST_MAINFOCUS_OVERLAY;
                static QString GRIDVIEW_PLAYLIST_MOVIE_ICON;
                static QString GRIDVIEW_PLAYLIST_PICTURE_ICON;
                static QString GRIDVIEW_PLAYLIST_MUSIC_ICON;
                static QString GRIDVIEW_PLAYLIST_NEW_ICON;
                static QString GRIDVIEW_PLAYLIST_DEFAULT_MOVIE_THUMBNAIL;
                static QString GRIDVIEW_PLAYLIST_DEFAULT_MUSIC_THUMBNAIL;

                static QString GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_PDF_THUMBNAIL;
                static QString GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_POWERPOINT_THUMBNAIL;
                static QString GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_WORD_THUMBNAIL;
                static QString GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_EXCEL_THUMBNAIL;
                static QString GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_OTHER_THUMBNAIL;

                static int GRIDVIEW_PLAYLIST_ITEM_TITLE_MARGIN_TOP;
                static QString GRIDVIEW_PLAYLISTITEM_BORDER_TOP;
                static QString GRIDVIEW_PLAYLISTITEM_BORDER_LEFT;
                static QString GRIDVIEW_PLAYLISTITEM_BORDER_BOTTOM;
                static QString GRIDVIEW_PLAYLISTITEM_BORDER_RIGHT;

                // Coverview
                static QString COVERVIEW_PLAYLIST_IMAGE_DIR;
                static QString COVERVIEW_PLAYLIST_TV_ICON;
                static int COVERVIEW_PLAYLIST_TV_ICON_TOP;
                static int COVERVIEW_PLAYLIST_TV_ICON_BOTTOM;
                static int COVERVIEW_PLAYLIST_TV_ICON_LEFT;
                static int COVERVIEW_PLAYLIST_TV_ICON_RIGHT;
                static QString COVERVIEW_PLAYLIST_NEWS_ICON;
                static int COVERVIEW_PLAYLIST_NEWS_TOP;
                static int COVERVIEW_PLAYLIST_NEWS_BOTTOM;
                static int COVERVIEW_PLAYLIST_NEWS_LEFT;
                static int COVERVIEW_PLAYLIST_NEWS_RIGHT;
                static QString COVERVIEW_PLAYLIST_DEFAULT_MOVIE_THUMBNAIL;
                static QString COVERVIEW_PLAYLIST_DEFAULT_MUSIC_THUMBNAIL;
                static QString COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_PDF_THUMBNAIL;
                static QString COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_POWERPOINT_THUMBNAIL;
                static QString COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_WORD_THUMBNAIL;
                static QString COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_EXCEL_THUMBNAIL;
                static QString COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_OTHER_THUMBNAIL;
                static QString COVERVIEW_PLAYLIST_NEW_ICON;
                static QString COVERVIEW_PLAYLIST_MOVIE_ICON;
                static QString COVERVIEW_PLAYLIST_PICTURE_ICON;
                static QString COVERVIEW_PLAYLIST_MUSIC_ICON;
                static QString COVERVIEW_PLAYLIST_OVERLAYPATTERN;
                static QString COVERVIEW_PLAYLIST_MAINFOCUS_OVERLAY;
                static int COVERVIEW_PLAYLIST_ITEM_CONTENTTYPE_ICON_SIZE_PROCENT;
                static int COVERVIEW_PLAYLIST_ITEM_ISNEW_ICON_SIZE_PROCENT;

                // News Module
                static QString NEWS_DETAIL_VIEW_IMAGE_DIR;
                static QString NEWS_DETAIL_VIEW_TEXT_DIR;
                static QString NEWS_DETAIL_VIEW_STYLE;
                static QString NEWS_DETAIL_VIEW_BG_URL;
                static QString NEWS_DETAIL_VIEW_LINK_TEXT;
                static QString NEWS_DETAIL_VIEW_CLOSE_BTN;
                static int NEWS_DETAIL_VIEW_CLOSE_BTN_RIGHT;
                static int NEWS_DETAIL_VIEW_CLOSE_BTN_TOP;

                static QString NEWSTICKER_IMAGE_DIR;
                static QString NEWSTICKER_BACKGROUND_IMAGE;

                // About View
                static QString ABOUT_DETAIL_VIEW_TEXT_DIR;
                static QString ABOUT_DETAIL_VIEW_STYLE;

                // Update Checker Module
                static QString PLAYERUPDATECHECKER_IMAGE_DIR;
                static QString PLAYERUPDATECHECKER_TEXT_DIR;
                static QString PLAYERUPDATECHECKER_BACKGROUND_IMAGE;
                static QString PLAYERUPDATECHECKER_UPDATE_ALERT_ICON;
                static QString PLAYERUPDATECHECKER_UPDATE_ALERT_LATER;
                static QString PLAYERUPDATECHECKER_UPDATE_ALERT_LATER_PRESSED;
                static QString PLAYERUPDATECHECKER_UPDATE_ALERT_NO;
                static QString PLAYERUPDATECHECKER_UPDATE_ALERT_NO_PRESSED;
                static QString PLAYERUPDATECHECKER_UPDATE_ALERT_YES;
                static QString PLAYERUPDATECHECKER_UPDATE_ALERT_YES_PRESSED;
                static QString PLAYERUPDATECHECKER_UPDATE_ALERT_OK;
                static QString PLAYERUPDATECHECKER_UPDATE_ALERT_OK_PRESSED;
                static QString PLAYERUPDATECHECKER_DIALOG_NEW;
                static QString PLAYERUPDATECHECKER_DIALOG_FINISHED;
                static QString PLAYERUPDATECHECKER_DIALOG_DISKSPACE;

                //StatusDisplay
                static QString STATUSDISPLAY_IMAGE_DIR;
                static QString STATUSDISPLAY_TEXT_DIR;
                static QString STATUSDISPLAY_FILETASKS_ICON;
                static QString STATUSDISPLAY_APPLICATIONUPDATE_ICON;
                static QString STATUSDISPLAY_CONTENTDOWNLOAD_ICON;
                static QString STATUSDISPLAY_FILETASKS_DISABLED_ICON;
                static QString STATUSDISPLAY_APPLICATIONUPDATE_DISABLED_ICON;
                static QString STATUSDISPLAY_CONTENTDOWNLOAD_DISABLED_ICON;
                static int STATUSDISPLAY_HOVERBOX_HIDE_TIMEOUT;
                static int STATUSDISPLAY_HOVERBOX_WIDTH;
                static int STATUSDISPLAY_HOVERBOX_MAX_HEIGHT;
                static QString STATUSDISPLAY_CANCEL_ALL_BUTTON;
                static QString STATUSDISPLAY_CANCEL_ALL_BUTTON_PRESSED;
                static QString STATUSDISPLAY_CANCEL_BUTTON;
                static QString STATUSDISPLAY_CANCEL_BUTTON_PRESSED;
                static QString STATUSDISPLAY_CIRCLE_PROGRESS_ANIMATION;
                static QString STATUSDISPLAY_DISK_SPACE_ICON;
                static QString STATUSDISPLAY_DISK_SPACE_PROGRESS_ICON;
                static QString STATUSDISPLAY_DISK_SPACE_SHOW_MEDIA;
                static QString STATUSDISPLAY_DISK_SPACE_SHOW_MEDIA_PRESSED;
                static QString STATUSDISPLAY_ONLINE_ICON;
                static QString STATUSDISPLAY_OFFLINE_ICON;
                static QString STATUSDISPLAY_ONLINE_TXT_FILE;
                static QString STATUSDISPLAY_OFFLINE_TXT_FILE;

                //NewsDetailView
                static int NEWSDETAILVIEW_WIDTH_PROCENT_PLAYLISTREGION;
                static int NEWSDETAILVIEW_HEIGHT_PROCENT_PLAYLISTREGION;

                //SettingsDetailView
                static QString SETTINGS_DETAIL_VIEW_IMAGE_DIR;
                static QString SETTINGS_DETAIL_VIEW_TEXT_DIR;
                static QString SETTINGS_DETAIL_VIEW_EXIT_BUTTON;
                static QString SETTINGS_DETAIL_VIEW_EXIT_BUTTON_PRESSED;
                static QString SETTINGS_DETAIL_VIEW_GENERAL_BACKGROUND;
                static QString SETTINGS_DETAIL_VIEW_MEDIA_BACKGROUND;
                static int SETTINGS_DETAIL_VIEW_MARGIN_BOTTOM;
                static int SETTINGS_DETAIL_VIEW_MARGIN_TOP;
                static QString SETTINGS_DETAIL_VIEW_GENERAL_SETUP_FILE;
                static QString SETTINGS_DETAIL_VIEW_MEDIA_SETUP_FILE;
                static QString SETTINGS_DETAIL_VIEW_MEDIA_ICON;
                static QString SETTINGS_DETAIL_VIEW_GENERAL_ICON;
                static float SETTINGS_DETAIL_VIEW_ICON_WIDTH_RATIO;
                static int SETTINGS_DETAIL_VIEW_ICON_MARGIN_TOP;
                static int SETTINGS_DETAIL_VIEW_ICON_MARGIN_RIGHT;
                static QString SETTINGS_DETAIL_VIEW_HELP_ICON;
                static QString SETTINGS_DETAIL_VIEW_HELP_ICON_HOVER;
                static QString SETTINGS_DETAIL_VIEW_ADD_BUTTON;
                static QString SETTINGS_DETAIL_VIEW_ADD_BUTTON_DISABLED;
                static QString SETTINGS_DETAIL_VIEW_REMOVE_BUTTON;
                static QString SETTINGS_DETAIL_VIEW_REMOVE_BUTTON_DISABLED;

                //ContentManagementModule
                static QString CONTENT_MAMAGEMENT_IMAGE_DIR;
                static QString CONTENT_MAMAGEMENT_DOWNLOAD_BUTTON;
                static QString CONTENT_MAMAGEMENT_DOWNLOAD_BUTTON_PRESSED;
                static QString CONTENT_MAMAGEMENT_SKIP_BUTTON;
                static QString CONTENT_MAMAGEMENT_SKIP_BUTTON_PRESSED;
                static QString CONTENT_MAMAGEMENT_CANCEL_BUTTON;
                static QString CONTENT_MAMAGEMENT_CANCEL_BUTTON_PRESSED;
                static QString CONTENT_MAMAGEMENT_CANCEL_ALL_BUTTON;
                static QString CONTENT_MAMAGEMENT_CANCEL_ALL_BUTTON_PRESSED;
                static QString CONTENT_MAMAGEMENT_DELETE_BUTTON;
                static QString CONTENT_MAMAGEMENT_DELETE_BUTTON_PRESSED;
                static QString CONTENT_MAMAGEMENT_DELETE_ALL_BUTTON;
                static QString CONTENT_MAMAGEMENT_DELETE_ALL_BUTTON_PRESSED;
                static QString CONTENT_MAMAGEMENT_DOWNLOAD_ALL_BUTTON;
                static QString CONTENT_MAMAGEMENT_DOWNLOAD_ALL_BUTTON_PRESSED;
                static QString CONTENT_MAMAGEMENT_SKIP_ALL_BUTTON;
                static QString CONTENT_MAMAGEMENT_SKIP_ALL_BUTTON_PRESSED;

                //Logging
                static QString LOGGER_ROOT; //RootLogger

                //Custom Logger
                static QString LOGGER_PODCATCHER;
                static QString LOGGER_ALBUMCOVERCOLLECTOR;
                static QString LOGGER_DOWNLOADUTIL;
                static QString LOGGER_EXTERNALSNAPSHOTCREATOR;
                static QString LOGGER_SNAPSHOTCREATOR;
                static QString LOGGER_PLAYLISTCREATOR;
                static QString LOGGER_MEDIALIBRARY;
                static QString LOGGER_CHAPTERSELECT;
                static QString LOGGER_ABOUTPLAYLISTCREATOR;
                static QString LOGGER_PLAYERUPDATECHECKER;
                static QString LOGGER_SEARCHUICREATOR;
                static QString LOGGER_VERSIONSERVICECLIENT;
                static QString LOGGER_LISTVIEWPLAYLIST;
                static QString LOGGER_COVERVIEWPLAYLIST;
                static QString LOGGER_GRIDVIEWPLAYLIST;
                static QString LOGGER_PLAYER;
                static QString LOGGER_TOOLLIB;
                static QString LOGGER_CONTROLBAR;
                static QString LOGGER_PLAYERCLOSEBUTTON;
                static QString LOGGER_SEEKBAR;
                static QString LOGGER_VOLUMEBAR;
                static QString LOGGER_PLAYLISTUI;
                static QString LOGGER_CENTRALUI;
                static QString LOGGER_SHELL;
                static QString LOGGER_EVENTAGGREGATOR;
                static QString LOGGER_QTVLC;
                static QString LOGGER_RSS;
                static QString LOGGER_BOTTOMUI;
                static QString LOGGER_TOPUI;
                static QString LOGGER_TASKMANAGER;
                static QString LOGGER_STATEDISPLAY;
                static QString LOGGER_ABOUTDETAILVIEW;
                static QString LOGGER_STATISTICS;
                static QString LOGGER_UPDATEINSTALLER;
                static QString LOGGER_UPDATER;
                static QString LOGGER_SETTINGSDETAILVIEW;
                static QString LOGGER_CONTENTMANAGEMENT;
                //Known Logger
                static QStringList KNOWN_LOGGER;

                //AlbumCoverCollector
                static QString AMAZON_KEY;
                static QString AMAZON_SECRECT_KEY;

                // Statistics
                static QString STATISTIC_EVENT_TYPE_PLAYER_STARTED;
                static QString STATISTIC_EVENT_TYPE_PLAYER_STARTED_URL_VALUE;

                static QString STATISTIC_EVENT_TYPE_PODCAST_VIEW;
                static QString STATISTIC_EVENT_TYPE_PODCAST_VIEW_URL_VALUE;

                static QString STATISTIC_DATA_URL_KEY;
                static QString STATISTIC_DATE_URL_KEY;
                static QString STATISTIC_TYPE_URL_KEY;
                static QString STATISTIC_SERVER_URL_KEY;
                static QString STATISTIC_API_KEY_KEY;

                static QString STATISTIC_URL_TEMPLATE;

                // Update
                static QString UPDATE_SERVER_URL_KEY;
                static QString UPDATE_ID_URL_KEY;
                static QString UPDATE_OS_URL_KEY;
                static QString UPDATE_VERSION_URL_KEY;
                static QString UPDATE_URL_TEMPLATE;
                static QString UPDATE_API_KEY_KEY;
                static QString UPDATE_AVAILABLE_MARKER_FILENAME;

                //ContentManagement
                static QString CONTENT_NEW;
                static QString CONTENT_DOWNLOAD_RUNNING;
                static QString CONTENT_DOWNLOAD_PENDING;
                static QString CONTENT_DOWNLOAD_FINISHED;
                static QString CONTENT_SKIPPED;
                static QString CONTENT_BANNED;

                // FileSystem ralated
                static quint64 MINIMUM_FREE_BYTETS;
        };
    }
}

#endif // CONSTANTS_H
