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
#ifndef EVENTTYPES_H
#define EVENTTYPES_H

#include <QEvent>

namespace RedBullPlayer {
    namespace Events {
        // Numbers must be between 1001 and 65534
        const QEvent::Type PlayerStopped = ( QEvent::Type )1001;
        const QEvent::Type NewsItemChanged = ( QEvent::Type )1002;
        const QEvent::Type InternetConnectionStatusChanged = ( QEvent::Type )1003;
        const QEvent::Type FunctionSelected = ( QEvent::Type )1004;
        const QEvent::Type PlaylistItemSelected = ( QEvent::Type )1005;
        const QEvent::Type CentralUIWidgetChanged = ( QEvent::Type )1006;
        const QEvent::Type CentralUIRegionResized = ( QEvent::Type )1007;
        const QEvent::Type ShellWidgetMoved = ( QEvent::Type )1008;
        const QEvent::Type ControllBarBackwardButtonClicked = ( QEvent::Type )1009;
        const QEvent::Type ControllBarPlayButtonClicked = ( QEvent::Type )1010;
        const QEvent::Type ControllBarPauseButtonClicked = ( QEvent::Type )1011;
        const QEvent::Type ControllBarForwardButtonClicked = ( QEvent::Type )1012;
        const QEvent::Type ControllBarPreviewButtonClicked = ( QEvent::Type )1013;
        const QEvent::Type ControllBarSnapshotButtonClicked = ( QEvent::Type )1014;
        const QEvent::Type ControllBarFullscreenButtonClicked = ( QEvent::Type )1015;
        const QEvent::Type ControllBarMuteButtonClicked = ( QEvent::Type )1016;
        const QEvent::Type PlayerStateChanged = ( QEvent::Type )1017;
        const QEvent::Type SnapshotCaptureEnabled = ( QEvent::Type )1018;
        const QEvent::Type PlayerTimeChanged = ( QEvent::Type )1019;
        const QEvent::Type SeekBarPlayHeadChanged = ( QEvent::Type )1020;
        const QEvent::Type PlayerExitClicked = ( QEvent::Type )1021;
        const QEvent::Type PlayerVisibilityChanged = ( QEvent::Type )1022;
        const QEvent::Type ChapterSelected = ( QEvent::Type )1023;
        const QEvent::Type VideoScreenMouseMoved = ( QEvent::Type )1024;
        const QEvent::Type PlayerCloseButtonClicked = ( QEvent::Type )1025;
        const QEvent::Type KeyPressed = ( QEvent::Type )1026;
        const QEvent::Type ControllBarVisibilityChanged = ( QEvent::Type )1027;
        const QEvent::Type FullscreenStatusChanged = ( QEvent::Type )1028;
        const QEvent::Type PlayerBackwardSpeedChanged = ( QEvent::Type )1029;
        const QEvent::Type PlayerForwardSpeedChanged = ( QEvent::Type )1030;
        const QEvent::Type SnapshotCreationRequested = ( QEvent::Type )1031;
        const QEvent::Type SnapshotCreated = ( QEvent::Type )1032;
        const QEvent::Type PlayerVolumeChanged = ( QEvent::Type )1035;
        const QEvent::Type FocusedPlaylistItemChanged = ( QEvent::Type )1036;
        const QEvent::Type PlaylistNavigationKeyPressed = ( QEvent::Type )1037;
        const QEvent::Type PlaylistCreated = ( QEvent::Type )1038;
        const QEvent::Type PlaylistContentTypeChanged = ( QEvent::Type )1039;
        const QEvent::Type PlayerTrackStarted = ( QEvent::Type )1040;
        const QEvent::Type MainUIRegionFocusChanged = ( QEvent::Type )1041;
        const QEvent::Type CoverImageChanged = ( QEvent::Type )1042;
        const QEvent::Type CoverImageRequested = ( QEvent::Type )1043;
        const QEvent::Type CancelCoverImageCreation = ( QEvent::Type )1044;
        const QEvent::Type CancelSnapshotCreation = ( QEvent::Type )1045;
        const QEvent::Type MediaMetaChanged = ( QEvent::Type )1047;
        const QEvent::Type PlaylistNextItemSelected = ( QEvent::Type )1049;
        const QEvent::Type CentralFocusChanged = ( QEvent::Type )1050;
        const QEvent::Type PlaylistItemDrop = ( QEvent::Type )1051;
        const QEvent::Type ChapterImageRequest = ( QEvent::Type )1052;
        const QEvent::Type ChapterImageCreated = ( QEvent::Type )1053;
        const QEvent::Type CancelChapterImageRequest = ( QEvent::Type )1054;
        const QEvent::Type EndOfPlaylist = ( QEvent::Type )1055;
        const QEvent::Type SearchChanged = ( QEvent::Type )1056;
        const QEvent::Type PlaylistPreviousItemSelected = ( QEvent::Type )1057;
        const QEvent::Type MediaDiscovered = ( QEvent::Type )1058;
        const QEvent::Type MediaItemContentChanged = ( QEvent::Type )1059;
        const QEvent::Type RequestAlbumCoverImage = ( QEvent::Type )1060;
        const QEvent::Type MetaDataChanged = ( QEvent::Type )1062;
        const QEvent::Type PlaylistClear = ( QEvent::Type )1063;
        const QEvent::Type PlaylistItemRemoved = ( QEvent::Type )1064;
        const QEvent::Type AddedItemToPlaylist = ( QEvent::Type )1065;
        const QEvent::Type PlaylistGenerationFinished = ( QEvent::Type )1066;
        const QEvent::Type PlayerAspectRatioChanged = ( QEvent::Type )1067;
        const QEvent::Type ControllBarResolutionButtonClicked = ( QEvent::Type )1068;
        const QEvent::Type CachedPlaylistRestored = ( QEvent::Type )1069;
        const QEvent::Type StartupPlaylistGenerationStarted = ( QEvent::Type )1070;
        const QEvent::Type SearchEnded = ( QEvent::Type )1071;
        const QEvent::Type StartupPlaylistGenerationFinished = ( QEvent::Type )1072;
        const QEvent::Type SearchPlaylistGenerationStarted = ( QEvent::Type )1073;
        const QEvent::Type MediaVanished = ( QEvent::Type )1074;
        const QEvent::Type MediaRemoved = ( QEvent::Type )1075;
        const QEvent::Type RemovedItemFromPlaylist = ( QEvent::Type )1076;
        const QEvent::Type IOProcessFinished = ( QEvent::Type )1077;

        const QEvent::Type RssFeedParseRequest = ( QEvent::Type )1093;
        const QEvent::Type RssFeedParseResponse = ( QEvent::Type )1094;
        const QEvent::Type RssFeedPlaylistCreationRequest = ( QEvent::Type )1095;

        const QEvent::Type BufferingAnimationChangeVisibility = ( QEvent::Type )1099;

        // TaskManager Events
        const QEvent::Type TaskStarted = ( QEvent::Type )10001;
        const QEvent::Type TaskFinished = ( QEvent::Type )10002;
        const QEvent::Type TaskCanceled = ( QEvent::Type )10003;
        const QEvent::Type TaskRepositoryTaskAdded = ( QEvent::Type )10004;
        const QEvent::Type TaskRepositoryTaskRemoved = ( QEvent::Type )10005;
        const QEvent::Type TaskProgress = ( QEvent::Type )10006;
        const QEvent::Type TaskExecuteRequest = ( QEvent::Type )10007;
        const QEvent::Type TaskCancelRequest = ( QEvent::Type )10009;


        // MediaWatcher Events
        const QEvent::Type MediaWatcherAddExclusion = ( QEvent::Type )10101;
        const QEvent::Type MediaWatcherRemoveExclusion = ( QEvent::Type )10102;
        const QEvent::Type MediaWatcherFileAdded = ( QEvent::Type )10103;
        const QEvent::Type MediaWatcherFileRemoved = ( QEvent::Type )10104;

        //ControllBar
        const QEvent::Type PlayerMute = ( QEvent::Type )10200;
        const QEvent::Type UpdateControllBarModel = ( QEvent::Type )10201;

        //SeekBar
        const QEvent::Type UpdateSeekBarModel = ( QEvent::Type )10300;

        //ChaperSelect
        const QEvent::Type UpdateChapterSelectModel = ( QEvent::Type )10400;

        //Player
        const QEvent::Type PlayerPositionChanged = ( QEvent::Type )10500;
        const QEvent::Type PlayerRequestTimeChange = ( QEvent::Type )10501;


        // Statistics
        const QEvent::Type CreateStatisticsEntry = ( QEvent::Type )10600;

        //UserSettings
        const QEvent::Type ShowUserSettingsDialog = ( QEvent::Type )10700;
        const QEvent::Type UserSettingsChanged = ( QEvent::Type )10701;
        const QEvent::Type FileExtensionsSettingsChanged = ( QEvent::Type )10702;
        const QEvent::Type ScanedDirectoriesSettingsChanged = ( QEvent::Type )10703;

        // UpdateEvents
        const QEvent::Type UpdateAvailableForInstall = ( QEvent::Type )10800;

        //Documents
        const QEvent::Type DocumentOpened = ( QEvent::Type )10900;

        //ContentManagement
        const QEvent::Type AvailableContentCountChanged = ( QEvent::Type )11000;
        const QEvent::Type NewContentAvailable = ( QEvent::Type )11001;
        const QEvent::Type BulkNewContentAvailable = ( QEvent::Type )11002;
        const QEvent::Type RemoveContent = ( QEvent::Type )11003;
        const QEvent::Type InitiateContentDownload = ( QEvent::Type )11004;
        const QEvent::Type SkipContent = ( QEvent::Type )11005;
        const QEvent::Type CancelContentDownload = ( QEvent::Type )11006;
        const QEvent::Type ContentDownloadStarted = ( QEvent::Type )11007;
        const QEvent::Type ContentDownloadProgress = ( QEvent::Type )11008;
        const QEvent::Type ContentDownloadFinished = ( QEvent::Type )11009;
        const QEvent::Type ContentDownloadCanceled = ( QEvent::Type )11010;
        const QEvent::Type BanContent = ( QEvent::Type )11011;

        //StateDisplay Events
        const QEvent::Type StatusChanged = ( QEvent::Type )12000;
        const QEvent::Type HoverBoxChangeVisibility = ( QEvent::Type )12001;
        const QEvent::Type HoverBoxHasFocus = ( QEvent::Type )12002;
        const QEvent::Type RequestHoverBoxChangeVisibility = ( QEvent::Type )12003;
    }
}
#endif // EVENTTYPES_H
