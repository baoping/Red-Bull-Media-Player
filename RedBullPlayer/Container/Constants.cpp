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
#include "Constants.h"

using namespace RedBullPlayer::Container;

QString Constants::GENERAL_TITLE_FONT = "Tahoma";
QString Constants::GENERAL_TITLE_NOMAINREGIONFOCUS_COLOR = "color: rgb(49,53,63);";
QString Constants::GENERAL_TITLE_NOMAINREGIONFOCUS_COLOR_HTMLHEXCODE = "#31353F";

QString Constants::FUNCTION_SELECT_NAVIGATION_LEFT_ARROW_URL = "media/FunctionSelectModule/images/leftArrow.png";
QString Constants::FUNCTION_SELECT_NAVIGATION_RIGHT_ARROW_URL = "media/FunctionSelectModule/images/rightArrow.png";
QString Constants::FUNCTION_SELECT_IMAGE_DIR = "media/FunctionSelectModule/images/";
QString Constants::FUNCTION_SELECT_TV_URL = "tv.png";
QString Constants::FUNCTION_SELECT_TV_TITLE = "TV";
QString Constants::FUNCTION_SELECT_MEDIA_URL = "videoplayer.png";
QString Constants::FUNCTION_SELECT_MEDIA_TITLE = "PLAYER";
QString Constants::FUNCTION_SELECT_ABOUT_URL = "about.png";
QString Constants::FUNCTION_SELECT_ABOUT_TITLE = "ABOUT";
QString Constants::FUNCTION_SELECT_NEWS_URL = "news.png";
QString Constants::FUNCTION_SELECT_NEWS_TITLE = "NEWS";
QString Constants::FUNCTION_SELECT_SERVUSTVB2B_URL = "servusTvB2B.png";
QString Constants::FUNCTION_SELECT_SERVUSTVB2B_TITLE = "SERVUSTV";
QString Constants::FUNCTION_SELECT_SETTINGS_URL = "settings.png";
QString Constants::FUNCTION_SELECT_SETTINGS_TITLE = "SETTINGS";
int Constants::FUNCTION_SELECT_ANIMATION_LENGTH = 1200;
QString Constants::FUNCTION_SELECT_CIRCLE = "circle.png";

int Constants::PLAYLISTUI_ARROW_WIDTH = 30;
int Constants::PLAYLISTUI_ARROW_HEIGHT = 40;
int Constants::PLAYLISTUI_ARROW_MARGIN = 25;
int Constants::ARROW_ALPHA = 200;
QString Constants::PLAYLISTUI_NAVIGATION_LEFT_ARROW_URL = "leftArrow.png";
QString Constants::PLAYLISTUI_NAVIGATION_LEFT_ARROW_DISABLED_URL = "leftArrowDisabled.png";
QString Constants::PLAYLISTUI_NAVIGATION_RIGHT_ARROW_URL = "rightArrow.png";
QString Constants::PLAYLISTUI_NAVIGATION_RIGHT_ARROW_DISABLED_URL = "rightArrowDisabled.png";
int Constants::PLAYLISTUI_ITEM_COUNT = 7;
int Constants::PLAYLISTUI_ITEM_GAP = 5;
float Constants::PLAYLISTUI_ITEM_RATIO = 0.75;
int Constants::PLAYLISTUI_ANIMATION_LENGTH = 500;
QString Constants::PLAYLISTUI_IMAGE_DIR = "media/PlaylistUIModule/images/";

QString Constants::PLAYLISTUI_COVERFLOW_ICON            = "coverflow.png";
QString Constants::PLAYLISTUI_COVERFLOW_ICON_CHECKED    = "activecoverflow.png";
QString Constants::PLAYLISTUI_COVERFLOW_ICON_DISABLED   = "darkcoverflow.png";

QString Constants::PLAYLISTUI_GRID_VIEW_ICON            = "gridview.png";
QString Constants::PLAYLISTUI_GRID_VIEW_ICON_CHECKED    = "activegridview.png";
QString Constants::PLAYLISTUI_GRID_VIEW_ICON_DISABLED   = "darkgridview.png";

QString Constants::PLAYLISTUI_LIST_VIEW_ICON            = "listview.png";
QString Constants::PLAYLISTUI_LIST_VIEW_ICON_CHECKED    = "activelistview.png";
QString Constants::PLAYLISTUI_LIST_VIEW_ICON_DISABLED   = "darklistview.png";

QString Constants::PLAYLISTUI_PLUS_ICON             = "+.png";
QString Constants::PLAYLISTUI_PLUS_ICON_MOUSEOVER   = "active+.png";
QString Constants::PLAYLISTUI_PLUS_ICON_DISABLED    = "+verdunkelt.png";
QString Constants::PLAYLISTUI_MINUS_ICON            = "-.png";
QString Constants::PLAYLISTUI_MINUS_ICON_MOUSEOVER  = "active-.png";
QString Constants::PLAYLISTUI_MINUS_ICON_DISABLED   = "-_verdunkelt.png";

QString Constants::PLAYLISTUI_DELETE_YES_ICON_URL = "yes.png";
QString Constants::PLAYLISTUI_DELETE_NO_ICON_URL = "no.png";
QString Constants::PLAYLISTUI_DELETE_BIN_ICON_URL = "recycle_bin.png";
QString Constants::PLAYLISTUI_DELETE_BG_URL = "background.png";
QString Constants::PLAYLISTUI_SURE = "Are you sure you want to move the file %1 to the deleted items folder?";
QString Constants::PLAYLISTUI_SURE_ONLYPLAYLIST = "Are you sure you want to remove the file %1 from the playlist?";

QColor Constants::CONTROLL_BAR_BG_COLOR = QColor( 0, 0, 0, 255 );
int Constants::CONTROLL_BAR_WIDTH = 550;
int Constants::CONTROLL_BAR_HEIGHT = 110;
int Constants::CONTROLL_BAR_BOTTOM = 25;
int Constants::CONTROLL_BAR_ANIMATION_LENGTH = 2000;
QString Constants::CONTROLL_BAR_IMAGE_DIR = "media/ControllBarModule/images/";
QString Constants::CONTROLL_BAR_BACKWARD = "backward.png";
QString Constants::CONTROLL_BAR_PLAY = "play.png";
QString Constants::CONTROLL_BAR_PAUSE = "pause.png";
QString Constants::CONTROLL_BAR_FORWARD = "forward.png";
QString Constants::CONTROLL_BAR_PREVIEW = "preview.png";
QString Constants::CONTROLL_BAR_SNAPSHOT = "snapshot.png";
QString Constants::CONTROLL_BAR_FULLSCREEN = "fullscreen.png";
QString Constants::CONTROLL_BAR_RESOLUTION = "resolution.png";
QString Constants::CONTROLL_BAR_VOLUME = "volume.png";
QString Constants::CONTROLL_BAR_BACKGROUND = "controlBarBackground.png";
int Constants::CONTROLL_BAR_HIDE_PERIOD = 3000;
QString Constants::CONTROLL_BAR_BUFFERING_TITLE = "Buffering ...";

QString Constants::SEEK_BAR_IMAGE_DIR = "media/SeekBarModule/images/";
//QString Constants::SEEK_BAR_BG_BORDER = "bg.svg";
QString Constants::SEEK_BAR_BG_BORDER = "seekBarBorder.png";
QString Constants::SEEK_BAR_PLAY_HEAD = "playhead.png";
QString Constants::SEEK_BAR_BUFFERING_ANIMATION = "bufferAnimation.gif";
int Constants::SEEK_BAR_DRAGGING_TIMEOUT = 500;

QString Constants::TOP_UI_IMAGE_DIR = "media/TopUIModule/images/";
QString Constants::TOP_UI_LOGO_FILENAME = "redbull_logo.png";
QString Constants::TOP_UI_EXIT_FILENAME = "exit.png";
QString Constants::TOP_UI_BG_FILENAME =  "bg.png";
QString Constants::TOP_UI_OVERLAY_IMAGE = "blackOverlay.png";
QString Constants::TOP_UI_TOPLINE_IMAGE = "linie.png";

QString Constants::SEARCH_UI_IMAGE_DIR = "media/SearchUIModule/images/";
QString Constants::SEARCH_UI_SEARCHBOX_FILENAME = "searchpane.png";
QString Constants::SEARCH_UI_BG_FILENAME = "bg.png";
QString Constants::SEARCH_UI_FLOWER_BG_FILENAME = "SearchUiFlower.png";
QString Constants::SEARCH_UI_SEARCHBOX_BG_FILENAME = "search_Textbox_Background_Repeat_x.png";
QString Constants::SEARCH_UI_SEARCHBOX_HEAD_FILENAME = "searchBox_Head.png";
QString Constants::SEARCH_UI_SEARCHBOX_COVER_FILENAME = "SearchCover_Shorter.png";
QString Constants::SEARCH_UI_SEARCHBOX_X = "x.png";
QString Constants::SEARCH_UI_SEARCHBOX_X_PRESSED = "activex.png";
QString Constants::SEARCH_UI_GRADIENT_LINE = "linie.png";

QString Constants::ROOT_UI_IMAGE_DIR = "media/RootModule/images/";
QString Constants::ROOT_UI_TEXT_DIR = "media/RootModule/text/";
QString Constants::DISKSPACE_FULL_TEXT = "diskspace_full";
QString Constants::ROOT_UI_BACKGROUNDIMAGE = "Blume_Big.png";
QString Constants::ROOT_UI_HORIZONTAL_GRADIENT_LINE = "linie.png";
QString Constants::ROOT_UI_VERTICAL_GRADIENT_LINE = "rand_seitlich.png";

QColor Constants::CHAPTER_SELECT_BG_COLOR = QColor( 0, 0, 0, 255 );
int Constants::CHAPTER_SELECT_WIDTH = 550;
int Constants::CHAPTER_SELECT_HEIGHT = 60;
int Constants::CHAPTER_SELECT_BOTTOM = 5;
int Constants::CHAPTER_SELECT_ARROW_WIDTH = 20;
int Constants::CHAPTER_SELECT_ARROW_HEIGHT = 30;
QString Constants::CHAPTER_SELECT_IMAGE_DIR = "media/ChapterSelectModule/images/";
QString Constants::CHAPTER_SELECT_LEFT_ARROW_URL = "leftArrow.png";
QString Constants::CHAPTER_SELECT_LEFT_ARROW_DISABLED_URL = "leftArrowDisabled.png";
QString Constants::CHAPTER_SELECT_RIGHT_ARROW_URL = "rightArrow.png";
QString Constants::CHAPTER_SELECT_RIGHT_ARROW_DISABLED_URL = "rightArrowDisabled.png";
int Constants::CHAPTER_SELECT_ITEMS_COUNT = 5;
int Constants::CHAPTER_SELECT_ITEMS_MARGIN = 5;
QColor Constants::CHAPTER_SELECT_ACTIVE_ITEM_BG_COLOR = QColor( 221, 32, 68 );
float Constants::CHAPTER_SELECT_ANIMATION_SPEED = 0.7;
QString Constants::CHAPTER_SELECT_DEFAULT_PREVIEW_IMAGE = "DefaultChapterPreviewPlaceholder.png";
QString Constants::CHAPTER_SELECT_BACKGROUND_IMAGE = "chapterSelectBackground.png";

QString Constants::BOTTOM_UI_IMAGE_DIR = "media/BottomUIModule/images/";
QString Constants::BOTTOM_UI_BG_URL = "BottomUi_background_RepeatX.png";
QString Constants::BOTTOM_UI_UPPERGRADIENT_URL = "Bottom_UI_Upper_Gradient.png";
QString Constants::BOTTOM_UI_LOWERGRADIENT_URL = "Bottom_UI_Upper_Gradient.png";
QString Constants::BOTTOM_UI_RESIZE_URL = "resize.png";
QString Constants::BOTTOM_UI_OVERLAY_IMAGE = "blackOverlay.png";

QString Constants::CENTRAL_UI_IMAGE_DIR = "media/CentralUIModule/images/";
QString Constants::CENTRAL_UI_OVERLAY_IMAGE = "blackOverlay.png";
QString Constants::CENTRAL_UI_PLAYLIST_BACKGROUND_UNFOCUSED_COLOR = "background-color:rgb(1, 1, 14);";
QString Constants::CENTRAL_UI_PLAYLIST_BACKGROUND_FOCUSED_COLOR = "background-color: rgb(0,2,30);";
QString Constants::CENTRAL_UI_BACKGROUND_IMAGE = "CentralUIBackground.png";


QString Constants::PLAYLER_CLOSE_BUTTON_IMAGE_DIR = "media/PlayerCloseButtonModule/images/";
QString Constants::PLAYLER_CLOSE_BUTTON_X_URL = "x.png";
QString Constants::PLAYLER_CLOSE_BUTTON_BACK_2_PLAYLIST = "back2playlist.png";
int Constants::PLAYLER_CLOSE_BUTTON_TOP = 5;
int Constants::PLAYLER_CLOSE_BUTTON_RIGHT = 5;

float Constants::PLAYER_FORWARD_RATES[6] = {1, 2, 4, 8, 16, 32};
int Constants::PLAYER_FORWARD_RATES_COUNT = 6;
float Constants::PLAYER_BACKWARD_RATES[6] = {1, 2, 4, 8, 16, 32};
int Constants::PLAYER_BACKWARD_RATES_COUNT = 6;
int Constants::PLAYER_BACKWARD_STEP = 500;
int Constants::PLAYER_BACKWARD_TIMER_PERIOD = 500;
int Constants::PLAYER_FORWARD_STEP = 500;
int Constants::PLAYER_FORWARD_TIMER_PERIOD = 500;
int Constants::PLAYER_AUDIO_BACKWARD_STEP = -5000;
int Constants::PLAYER_AUDIO_FORWARD_STEP = 5000;

QString Constants::PLAYER_UI_IMAGE_DIR = "media/PlayerModule/images/";
QString Constants::PLAYER_UI_IMAGE_ANIMATION = "RedBull TV Jingle 56frames 1loop.gif" ;

QString Constants::PLAYLIST_CREATOR_IMAGE_DIR = "media/PlaylistCreatorModule/images/";
QString Constants::PLAYLIST_CREATOR_NEWS_PLACEHOLDER = "news_placeholder.png";
QString Constants::PLAYLIST_CREATOR_ABOUT_PLACEHOLDER = "about.png";
QString Constants::PLAYLIST_CREATOR_LICENSE_PLACEHOLDER = "license.png";
QString Constants::PLAYLIST_CREATOR_GENERAL_SETTINGS_PLACEHOLDER = "generalSettings.png";
QString Constants::PLAYLIST_CREATOR_MEDIA_SETTINGS_PLACEHOLDER = "mediaSettings.png";


QString Constants::SNAPSHOT_CRETATION_STATE_ELEMENT = "state";
QString Constants::SNAPSHOT_CRETATION_STATE_READY_VALUE = "readyForInput";
QString Constants::SNAPSHOT_CRETATION_STATE_BUSY_VALUE = "busy";
QString Constants::SNAPSHOT_CRETATION_ERROR_ELEMENT = "error";

QString Constants::SNAPSHOT_CRETATION_SNAPSHOTCREATED_ELEMENT = "snapshotCreated";
QString Constants::SNAPSHOT_CRETATION_REQUESTID_ATTRIBUTE = "requestId";
QString Constants::SNAPSHOT_CRETATION_FILENAME_ATTRIBUTE = "fileName";

QString Constants::MEDIA_TYPE_VIDEO_NAME = "Video";
QString Constants::MEDIA_TYPE_AUDIO_NAME = "Audio";
QString Constants::MEDIA_TYPE_IMAGE_NAME = "Image";
QString Constants::MEDIA_TYPE_TV_NAME = "TV-Channel";
QString Constants::MEDIA_TYPE_DOCUMENT_NAME = "Document";

QString Constants::MEDIADIRECTORY_PATH_PLACEHOLDER = "[MediaDirectory]";

QString Constants::METADATA_RECEIVE_METADATARECEIVED_ELEMENT = "metadataReceived";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_title_ELEMENT = "title";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_artist_ELEMENT = "artist";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_genre_ELEMENT = "genre";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_copyright_ELEMENT = "copyright";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_album_ELEMENT = "album";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_trackNumber_ELEMENT = "trackNumber";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_description_ELEMENT = "description";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_rating_ELEMENT = "rating";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_date_ELEMENT = "date";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_setting_ELEMENT = "setting";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_url_ELEMENT = "url";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_language_ELEMENT = "language";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_nowPlaying_ELEMENT = "nowPlaying";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_publisher_ELEMENT = "publisher";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_encodedBy_ELEMENT = "encodedBy";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_artworkUrl_ELEMENT = "artworkUrl";
QString Constants::METADATA_RECEIVE_METADATARECEIVED_trackID_ELEMENT = "trackID";

int Constants::VOLUME_BAR_HEIGHT = 25;
int Constants::VOLUME_BAR_WIDTH = 55;
QColor Constants::VOLUME_BAR_INACTIVE_COLOR = QColor( 150, 150, 150 );
QColor Constants::VOLUME_BAR_ACTIVE_COLOR = QColor( 224, 0, 62 );
QColor Constants::VOLUME_BAR_MOUSE_OVER_COLOR = QColor( 224, 0, 62 );
int Constants::VOLUME_BAR_START_LEVEL = 3;

QString Constants::LISTVIEW_PLAYLIST_IMAGE_DIR = "media/ListViewPlaylistModule/images/";;
QString Constants::LISTVIEW_PLAYLIST_MOVIE_ICON = "movie_Icon.png";
QString Constants::LISTVIEW_PLAYLIST_PICTURE_ICON = "foto_Icon.png";
QString Constants::LISTVIEW_PLAYLIST_MUSIC_ICON = "music_Icon.png";
QString Constants::LISTVIEW_PLAYLIST_NEW_ICON = "new_Icon.png";
QString Constants::LISTVIEW_PLAYLIST_HEADER_BACKGROUND_IMAGE = "ListView_Header_Background_repeat_x.png";
QString Constants::LISTVIEW_PLAYLIST_MAINFOCUS_OVERLAY = "blackOverlay.png";
QString Constants::LISTVIEW_PLAYLIST_FONT_NAME = "Tahoma";
int Constants::LISTVIEW_PLAYLIST_FONT_SIZE = 10;
int Constants::LISTVIEW_PLAYLIST_FONT_SIZE_HEADER = 12;
int Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT = 20;
int Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING = 5;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ICON = 3;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_TITLE = 25;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ARTIST = 12;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ALBUM = 11;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_CREATED_AT = 11;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_LENGTH = 6;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_DOWNLOAD = 7;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_IS_NEW = 2;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_FILENAME = 18;
int Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_FILESIZE = 5;
int Constants::LISTVIEW_PLAYLIST_PADDING_TO_NAVIGATION_ARROW = 5;
QString Constants::LISTVIEW_PLAYLIST_COLUMN_TITLE_HEADER_TEXT = "TITLE";
QString Constants::LISTVIEW_PLAYLIST_COLUMN_ARTIST_HEADER_TEXT = "ARTIST";
QString Constants::LISTVIEW_PLAYLIST_COLUMN_ALBUM_HEADER_TEXT = "ALBUM";
QString Constants::LISTVIEW_PLAYLIST_COLUMN_CREATEDAT_HEADER_TEXT = "ADDED";
QString Constants::LISTVIEW_PLAYLIST_COLUMN_LENGTH_HEADER_TEXT = "LENGTH";
QString Constants::LISTVIEW_PLAYLIST_COLUMN_DOWNLOAD_HEADER_TEXT = "DOWNLOAD";
QString Constants::LISTVIEW_PLAYLIST_COLUMN_ISNEW_HEADER_TEXT = "FRESH?";
QString Constants::LISTVIEW_PLAYLIST_COLUMN_FILENAME_HEADER_TEXT = "FILENAME";
QString Constants::LISTVIEW_PLAYLIST_COLUMN_FILESIZE_HEADER_TEXT = "FILESIZE";
QString Constants::LISTVIEW_PLAYLIST_DATE_DISPLAY_FORMAT = "dd.MM.yyyy hh:mm";
int Constants::LISTVIEW_PLAYLIST_SPACE_BETWEEN_PAGES = 30;
int Constants::LISTVIEW_PLAYLIST_ICON_HEIGHT_PROCENT_FROM_BOUNDINGBOX = 60;
int Constants::LISTVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH = 1000;

int Constants::GRIDVIEW_PLAYLIST_ROWS = 3;
int Constants::GRIDVIEW_PLAYLIST_COLUMS = 7;
int Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING = 5;
int Constants::GRIDVIEW_PLAYLIST_ITEM_TITLE_HEIGHT_PROCENT = 20;
int Constants::GRIDVIEW_PLAYLIST_ITEM_ISNEW_ICON_WIDTH_PROCENT_FROM_THUMBNAIL_WIDTH = 35;
int Constants::GRIDVIEW_PLAYLIST_DISTANCE_FROM_NAVIGATION_KEY = 20;
int Constants::GRIDVIEW_PLAYLIST_DISTANCE_BETWEEN_PAGES = 30;
int Constants::GRIDVIEW_PLAYLIST_SCROLL_ANIMATION_LENGTH = 1000;
QString Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR = "media/GridViewPlaylistModule/images/";
QString Constants::GRIDVIEW_PLAYLIST_OVERLAYPATTERN = "blueOverlay.png";
QString Constants::GRIDVIEW_PLAYLIST_MAINFOCUS_OVERLAY = "blackOverlay.png";
QString Constants::GRIDVIEW_PLAYLIST_MOVIE_ICON = "movie_Icon.png";
QString Constants::GRIDVIEW_PLAYLIST_PICTURE_ICON = "foto_Icon.png";
QString Constants::GRIDVIEW_PLAYLIST_MUSIC_ICON = "music_Icon.png";
QString Constants::GRIDVIEW_PLAYLIST_NEW_ICON = "new_Icon.png";
QString Constants::GRIDVIEW_PLAYLIST_DEFAULT_MOVIE_THUMBNAIL = "DefaultMovieThumbnail.png";
QString Constants::GRIDVIEW_PLAYLIST_DEFAULT_MUSIC_THUMBNAIL = "DefaultMusicPicture.png";

QString Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_PDF_THUMBNAIL = "pdf_placeholder.png";
QString Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_POWERPOINT_THUMBNAIL = "powerpoint_placeholder.png";
QString Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_WORD_THUMBNAIL = "word_placeholder.png";
QString Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_EXCEL_THUMBNAIL = "excel_placeholder.png";
QString Constants::GRIDVIEW_PLAYLIST_DEFAULT_DOCUMENT_OTHER_THUMBNAIL = "placeholder_others.png";

int Constants::GRIDVIEW_PLAYLIST_ITEM_TITLE_MARGIN_TOP = 5;
QString Constants::GRIDVIEW_PLAYLISTITEM_BORDER_TOP = "GridViewItem_border_top.png";
QString Constants::GRIDVIEW_PLAYLISTITEM_BORDER_LEFT = "GridViewItem_border_Left.png";
QString Constants::GRIDVIEW_PLAYLISTITEM_BORDER_BOTTOM = "GridViewItem_border_bottom.png";
QString Constants::GRIDVIEW_PLAYLISTITEM_BORDER_RIGHT = "GridViewItem_border_right.png";

QString Constants::COVERVIEW_PLAYLIST_IMAGE_DIR = "media/CoverViewPlaylistModule/images/";
QString Constants::COVERVIEW_PLAYLIST_TV_ICON = "tvframe.png";
int Constants::COVERVIEW_PLAYLIST_TV_ICON_TOP = 30;
int Constants::COVERVIEW_PLAYLIST_TV_ICON_BOTTOM = 20;
int Constants::COVERVIEW_PLAYLIST_TV_ICON_LEFT = 5;
int Constants::COVERVIEW_PLAYLIST_TV_ICON_RIGHT = 5;
QString Constants::COVERVIEW_PLAYLIST_NEWS_ICON = "news.png";
int Constants::COVERVIEW_PLAYLIST_NEWS_TOP = 20;
int Constants::COVERVIEW_PLAYLIST_NEWS_BOTTOM = 20;
int Constants::COVERVIEW_PLAYLIST_NEWS_LEFT = 5;
int Constants::COVERVIEW_PLAYLIST_NEWS_RIGHT = 5;
QString Constants::COVERVIEW_PLAYLIST_DEFAULT_MOVIE_THUMBNAIL = "DefaultMovieThumbnail.png";
QString Constants::COVERVIEW_PLAYLIST_DEFAULT_MUSIC_THUMBNAIL = "DefaultMusicPicture.png";

QString Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_PDF_THUMBNAIL = "pdf_placeholder.png";
QString Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_POWERPOINT_THUMBNAIL = "powerpoint_placeholder.png";
QString Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_WORD_THUMBNAIL = "word_placeholder.png";
QString Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_EXCEL_THUMBNAIL = "excel_placeholder.png";
QString Constants::COVERVIEW_PLAYLIST_DEFAULT_DOCUMENT_OTHER_THUMBNAIL = "placeholder_others.png";

QString Constants::COVERVIEW_PLAYLIST_NEW_ICON = "new_Icon.png";
QString Constants::COVERVIEW_PLAYLIST_MOVIE_ICON = "movie_Icon.png";
QString Constants::COVERVIEW_PLAYLIST_PICTURE_ICON = "foto_Icon.png";
QString Constants::COVERVIEW_PLAYLIST_MUSIC_ICON = "music_Icon.png";
QString Constants::COVERVIEW_PLAYLIST_OVERLAYPATTERN = "blueOverlay.png";
QString Constants::COVERVIEW_PLAYLIST_MAINFOCUS_OVERLAY = "blackOverlay.png";
int Constants::COVERVIEW_PLAYLIST_ITEM_CONTENTTYPE_ICON_SIZE_PROCENT = 13;
int Constants::COVERVIEW_PLAYLIST_ITEM_ISNEW_ICON_SIZE_PROCENT = 27;

QString Constants::NEWS_DETAIL_VIEW_IMAGE_DIR = "media/NewsDetailViewModule/images/";
QString Constants::NEWS_DETAIL_VIEW_TEXT_DIR = "media/NewsDetailViewModule/text/";
QString Constants::NEWS_DETAIL_VIEW_STYLE = "style";
QString Constants::NEWS_DETAIL_VIEW_BG_URL = "newsbig.png";
QString Constants::NEWS_DETAIL_VIEW_LINK_TEXT = "See full story";
QString Constants::NEWS_DETAIL_VIEW_CLOSE_BTN = "x.png";
int Constants::NEWS_DETAIL_VIEW_CLOSE_BTN_RIGHT = 20;
int Constants::NEWS_DETAIL_VIEW_CLOSE_BTN_TOP = 20;

QString Constants::ABOUT_DETAIL_VIEW_TEXT_DIR = "media/AboutDetailViewModule/text/";
QString Constants::ABOUT_DETAIL_VIEW_STYLE = "style";

QString Constants::NEWSTICKER_IMAGE_DIR = "media/NewsTickerModule/images/";
QString Constants::NEWSTICKER_BACKGROUND_IMAGE = "NewsTickerBackground.png";

QString Constants::PLAYERUPDATECHECKER_IMAGE_DIR = "media/PlayerUpdateCheckerModule/images/";
QString Constants::PLAYERUPDATECHECKER_TEXT_DIR = "media/PlayerUpdateCheckerModule/text/";
QString Constants::PLAYERUPDATECHECKER_BACKGROUND_IMAGE = "background.png";
QString Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_ICON = "update_alert_icon.png";
QString Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_LATER = "update_alert_later.png";
QString Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_LATER_PRESSED = "update_alert_later_pressed.png";
QString Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_NO = "update_alert_no.png";
QString Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_NO_PRESSED = "update_alert_no_pressed.png";
QString Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_YES = "update_alert_yes.png";
QString Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_YES_PRESSED = "update_alert_yes_pressed.png";
QString Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_OK = "update_alert_ok.png";
QString Constants::PLAYERUPDATECHECKER_UPDATE_ALERT_OK_PRESSED = "update_alert_ok_pressed.png";
QString Constants::PLAYERUPDATECHECKER_DIALOG_NEW = "new_update";
QString Constants::PLAYERUPDATECHECKER_DIALOG_FINISHED = "download_finished";
QString Constants::PLAYERUPDATECHECKER_DIALOG_DISKSPACE = "disk_space";

QString Constants::STATUSDISPLAY_IMAGE_DIR = "media/StateDisplayModule/images/";
QString Constants::STATUSDISPLAY_TEXT_DIR = "media/StateDisplayModule/text/";
QString Constants::STATUSDISPLAY_FILETASKS_ICON = "filesTaskStatusIcon.png";
QString Constants::STATUSDISPLAY_APPLICATIONUPDATE_ICON = "applicationUpdateStatusIcon.png";
QString Constants::STATUSDISPLAY_CONTENTDOWNLOAD_ICON = "contentDownloadStatusIcon.png";
QString Constants::STATUSDISPLAY_FILETASKS_DISABLED_ICON = "filesTaskStatusIconDisabled.png";
QString Constants::STATUSDISPLAY_APPLICATIONUPDATE_DISABLED_ICON = "applicationUpdateStatusIconDisabled.png";
QString Constants::STATUSDISPLAY_CONTENTDOWNLOAD_DISABLED_ICON = "contentDownloadStatusIconDisabled.png";
int Constants::STATUSDISPLAY_HOVERBOX_HIDE_TIMEOUT = 2000;
int Constants::STATUSDISPLAY_HOVERBOX_WIDTH = 300;
int Constants::STATUSDISPLAY_HOVERBOX_MAX_HEIGHT = 400;
QString Constants::STATUSDISPLAY_CANCEL_ALL_BUTTON = "cancel_all.png";
QString Constants::STATUSDISPLAY_CANCEL_ALL_BUTTON_PRESSED = "cancel_all_pressed.png";
QString Constants::STATUSDISPLAY_CANCEL_BUTTON = "cancel.png";
QString Constants::STATUSDISPLAY_CANCEL_BUTTON_PRESSED = "cancel_pressed.png";
QString Constants::STATUSDISPLAY_CIRCLE_PROGRESS_ANIMATION = "animated_Circle.gif";
QString Constants::STATUSDISPLAY_DISK_SPACE_ICON = "speicheranzeige_usb.png";
QString Constants::STATUSDISPLAY_DISK_SPACE_PROGRESS_ICON = "speicherstatusbalken.png";
QString Constants::STATUSDISPLAY_DISK_SPACE_SHOW_MEDIA = "show_media_button_unpressed.png";
QString Constants::STATUSDISPLAY_DISK_SPACE_SHOW_MEDIA_PRESSED = "show_media_button_pressed.png";
QString Constants::STATUSDISPLAY_ONLINE_ICON = "online.png";
QString Constants::STATUSDISPLAY_OFFLINE_ICON = "offline.png";
QString Constants::STATUSDISPLAY_ONLINE_TXT_FILE = "onlineText.txt";
QString Constants::STATUSDISPLAY_OFFLINE_TXT_FILE = "offlineText.txt";

//NewsDetailView
int Constants::NEWSDETAILVIEW_WIDTH_PROCENT_PLAYLISTREGION = 40;
int Constants::NEWSDETAILVIEW_HEIGHT_PROCENT_PLAYLISTREGION = 70;

//SettingsDetailView
QString Constants::SETTINGS_DETAIL_VIEW_IMAGE_DIR = "media/SettingsDetailViewModule/images/";
QString Constants::SETTINGS_DETAIL_VIEW_TEXT_DIR = "media/SettingsDetailViewModule/text/";
QString Constants::SETTINGS_DETAIL_VIEW_EXIT_BUTTON = "x.png";
QString Constants::SETTINGS_DETAIL_VIEW_EXIT_BUTTON_PRESSED = "x_active.png";
QString Constants::SETTINGS_DETAIL_VIEW_GENERAL_BACKGROUND = "background_general_settings.png";
QString Constants::SETTINGS_DETAIL_VIEW_MEDIA_BACKGROUND = "background_media_settings.png";
int Constants::SETTINGS_DETAIL_VIEW_MARGIN_BOTTOM = 30;
int Constants::SETTINGS_DETAIL_VIEW_MARGIN_TOP = 20;
QString Constants::SETTINGS_DETAIL_VIEW_GENERAL_SETUP_FILE = "generalSettingsSetup.xml";
QString Constants::SETTINGS_DETAIL_VIEW_MEDIA_SETUP_FILE = "mediaSettingsSetup.xml";
QString Constants::SETTINGS_DETAIL_VIEW_MEDIA_ICON = "media_icon.png";
QString Constants::SETTINGS_DETAIL_VIEW_GENERAL_ICON = "general_Icon.png";
float Constants::SETTINGS_DETAIL_VIEW_ICON_WIDTH_RATIO = 0.17;
int Constants::SETTINGS_DETAIL_VIEW_ICON_MARGIN_TOP = 25;
int Constants::SETTINGS_DETAIL_VIEW_ICON_MARGIN_RIGHT = 40;
QString Constants::SETTINGS_DETAIL_VIEW_HELP_ICON = "help_inactive.png";
QString Constants::SETTINGS_DETAIL_VIEW_HELP_ICON_HOVER = "help_active.png";
QString Constants::SETTINGS_DETAIL_VIEW_ADD_BUTTON = "add_aktive.png";
QString Constants::SETTINGS_DETAIL_VIEW_ADD_BUTTON_DISABLED = "add_inaktive.png";
QString Constants::SETTINGS_DETAIL_VIEW_REMOVE_BUTTON = "remove.png";
QString Constants::SETTINGS_DETAIL_VIEW_REMOVE_BUTTON_DISABLED = "remove_inaktiv.png";

//ContentManagement
QString Constants::CONTENT_MAMAGEMENT_IMAGE_DIR = "media/ContentManagementModule/images/";
QString Constants::CONTENT_MAMAGEMENT_DOWNLOAD_BUTTON = "download.png";
QString Constants::CONTENT_MAMAGEMENT_DOWNLOAD_BUTTON_PRESSED = "download_pressed.png";
QString Constants::CONTENT_MAMAGEMENT_SKIP_BUTTON = "skip.png";
QString Constants::CONTENT_MAMAGEMENT_SKIP_BUTTON_PRESSED = "skip_pressed.png";
QString Constants::CONTENT_MAMAGEMENT_CANCEL_BUTTON = "cancel.png";
QString Constants::CONTENT_MAMAGEMENT_CANCEL_BUTTON_PRESSED = "cancel_pressed.png";
QString Constants::CONTENT_MAMAGEMENT_CANCEL_ALL_BUTTON = "cancel_all.png";
QString Constants::CONTENT_MAMAGEMENT_CANCEL_ALL_BUTTON_PRESSED = "cancel_all_pressed.png";
QString Constants::CONTENT_MAMAGEMENT_DELETE_BUTTON = "delete.png";
QString Constants::CONTENT_MAMAGEMENT_DELETE_BUTTON_PRESSED = "delete_pressed.png";
QString Constants::CONTENT_MAMAGEMENT_DELETE_ALL_BUTTON = "delete_all.png";
QString Constants::CONTENT_MAMAGEMENT_DELETE_ALL_BUTTON_PRESSED = "delete_all_pressed.png";
QString Constants::CONTENT_MAMAGEMENT_DOWNLOAD_ALL_BUTTON = "download_all.png";
QString Constants::CONTENT_MAMAGEMENT_DOWNLOAD_ALL_BUTTON_PRESSED = "download_all_pressed.png";
QString Constants::CONTENT_MAMAGEMENT_SKIP_ALL_BUTTON = "skip_all.png";
QString Constants::CONTENT_MAMAGEMENT_SKIP_ALL_BUTTON_PRESSED = "skip_all_pressed.png";

//Logging
QString Constants::LOGGER_ROOT = "PlayerRoot";

QString Constants::LOGGER_PODCATCHER = "PodCatcher";
QString Constants::LOGGER_ALBUMCOVERCOLLECTOR = "AlbumCoverCollector";
QString Constants::LOGGER_DOWNLOADUTIL = "DownloadUtil";
QString Constants::LOGGER_EXTERNALSNAPSHOTCREATOR = "ExternalSnapshotCreator";
QString Constants::LOGGER_SNAPSHOTCREATOR = "SnapshotCreator";
QString Constants::LOGGER_PLAYLISTCREATOR = "PlayListCreator";
QString Constants::LOGGER_MEDIALIBRARY = "MediaLibrary";
QString Constants::LOGGER_CHAPTERSELECT = "ChapterSelect";
QString Constants::LOGGER_ABOUTPLAYLISTCREATOR = "AboutPlayListCreator";
QString Constants::LOGGER_PLAYERUPDATECHECKER = "PlayerUpdateChecker";
QString Constants::LOGGER_SEARCHUICREATOR = "SearchUICreator";
QString Constants::LOGGER_VERSIONSERVICECLIENT = "VersionServiceClient";
QString Constants::LOGGER_LISTVIEWPLAYLIST = "ListViewPlaylist";
QString Constants::LOGGER_COVERVIEWPLAYLIST = "CoverViewPlaylist";
QString Constants::LOGGER_GRIDVIEWPLAYLIST = "GridViewPlaylist";
QString Constants::LOGGER_PLAYER = "Player";
QString Constants::LOGGER_TOOLLIB = "ToolLib";
QString Constants::LOGGER_CONTROLBAR = "ControlBar";
QString Constants::LOGGER_PLAYERCLOSEBUTTON = "PlayerCloseButton";
QString Constants::LOGGER_SEEKBAR = "SeekBar";
QString Constants::LOGGER_VOLUMEBAR = "VolumeBar";
QString Constants::LOGGER_PLAYLISTUI = "PlaylistUI";
QString Constants::LOGGER_CENTRALUI = "CentralUI";
QString Constants::LOGGER_SHELL = "Shell";
QString Constants::LOGGER_EVENTAGGREGATOR = "EventAggregator";
QString Constants::LOGGER_QTVLC = "QtVLC";
QString Constants::LOGGER_RSS = "RSS";
QString Constants::LOGGER_BOTTOMUI = "BottomUI";
QString Constants::LOGGER_TOPUI = "TopUI";
QString Constants::LOGGER_TASKMANAGER = "TaskManager";
QString Constants::LOGGER_STATEDISPLAY = "StateDisplay";
QString Constants::LOGGER_ABOUTDETAILVIEW = "AboutDetailView";
QString Constants::LOGGER_STATISTICS = "Statistics";
QString Constants::LOGGER_UPDATEINSTALLER = "UpdateInstaller";
QString Constants::LOGGER_UPDATER = "Updater";
QString Constants::LOGGER_SETTINGSDETAILVIEW = "SettingsDetailView";
QString Constants::LOGGER_CONTENTMANAGEMENT = "ContentManagement";

QStringList Constants::KNOWN_LOGGER = QStringList()
                                      << Constants::LOGGER_PODCATCHER
                                      << Constants::LOGGER_ALBUMCOVERCOLLECTOR
                                      << Constants::LOGGER_DOWNLOADUTIL
                                      << Constants::LOGGER_EXTERNALSNAPSHOTCREATOR
                                      << Constants::LOGGER_SNAPSHOTCREATOR
                                      << Constants::LOGGER_PLAYLISTCREATOR
                                      << Constants::LOGGER_MEDIALIBRARY
                                      << Constants::LOGGER_CHAPTERSELECT
                                      << Constants::LOGGER_ABOUTPLAYLISTCREATOR
                                      << Constants::LOGGER_PLAYERUPDATECHECKER
                                      << Constants::LOGGER_SEARCHUICREATOR
                                      << Constants::LOGGER_VERSIONSERVICECLIENT
                                      << Constants::LOGGER_LISTVIEWPLAYLIST
                                      << Constants::LOGGER_COVERVIEWPLAYLIST
                                      << Constants::LOGGER_GRIDVIEWPLAYLIST
                                      << Constants::LOGGER_PLAYER
                                      << Constants::LOGGER_TOOLLIB
                                      << Constants::LOGGER_CONTROLBAR
                                      << Constants::LOGGER_PLAYERCLOSEBUTTON
                                      << Constants::LOGGER_SEEKBAR
                                      << Constants::LOGGER_VOLUMEBAR
                                      << Constants::LOGGER_PLAYLISTUI
                                      << Constants::LOGGER_CENTRALUI
                                      << Constants::LOGGER_SHELL
                                      << Constants::LOGGER_EVENTAGGREGATOR
                                      << Constants::LOGGER_QTVLC
                                      << Constants::LOGGER_RSS
                                      << Constants::LOGGER_BOTTOMUI
                                      << Constants::LOGGER_TOPUI
                                      << Constants::LOGGER_TASKMANAGER
                                      << Constants::LOGGER_STATEDISPLAY
                                      << Constants::LOGGER_ABOUTDETAILVIEW
                                      << Constants::LOGGER_STATISTICS
                                      << Constants::LOGGER_UPDATEINSTALLER
                                      << Constants::LOGGER_UPDATER
                                      << Constants::LOGGER_SETTINGSDETAILVIEW
                                      << Constants::LOGGER_CONTENTMANAGEMENT;

//AlbumCoverCollector
QString Constants::AMAZON_KEY = "AKIAIO3HVKMHIXEPWOGQ";
QString Constants::AMAZON_SECRECT_KEY = "";

// Statistics
QString Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED = "PlayerStarted";
QString Constants::STATISTIC_EVENT_TYPE_PLAYER_STARTED_URL_VALUE = "playerstarted";


QString Constants::STATISTIC_EVENT_TYPE_PODCAST_VIEW = "PodCastView";
QString Constants::STATISTIC_EVENT_TYPE_PODCAST_VIEW_URL_VALUE = "podcastview";

//QString Constants::STATISTIC_PLAYERID_URL_KEY = "{#PlayerId}";
QString Constants::STATISTIC_DATE_URL_KEY = "{#date}";
QString Constants::STATISTIC_TYPE_URL_KEY = "{#type}";
QString Constants::STATISTIC_DATA_URL_KEY = "{#data}";
QString Constants::STATISTIC_SERVER_URL_KEY = "{#server}";
QString Constants::STATISTIC_API_KEY_KEY = "{#apikey}";

QString Constants::STATISTIC_URL_TEMPLATE = "{#server}/statistic?date={#date}&type={#type}&data={#data}&apikey={#apikey}";


QString Constants::UPDATE_SERVER_URL_KEY = "{#server}";

QString Constants::UPDATE_ID_URL_KEY = "{#id}";
QString Constants::UPDATE_OS_URL_KEY = "{#os}";
QString Constants::UPDATE_VERSION_URL_KEY = "{#version}";
QString Constants::UPDATE_API_KEY_KEY = "{#apikey}";
QString Constants::UPDATE_URL_TEMPLATE = "{#server}/version?id={#id}&os={#os}&version={#version}&apikey={#apikey}";
QString Constants::UPDATE_AVAILABLE_MARKER_FILENAME = "update_available_for_install_at";

//ContentManagement
QString Constants::CONTENT_NEW = "CONTENT_NEW";
QString Constants::CONTENT_DOWNLOAD_RUNNING = "CONTENT_DOWNLOAD_RUNNING";
QString Constants::CONTENT_DOWNLOAD_PENDING = "CONTENT_DOWNLOAD_PENDING";
QString Constants::CONTENT_DOWNLOAD_FINISHED = "CONTENT_DOWNLOAD_FINISHED";
QString Constants::CONTENT_BANNED = "CONTENT_BANNED";
QString Constants::CONTENT_SKIPPED = "CONTENT_SKIPPED";

// File System

quint64 Constants::MINIMUM_FREE_BYTETS = 1024 * 2024 * 5;
