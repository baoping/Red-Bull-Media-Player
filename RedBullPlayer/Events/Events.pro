include( "../../globals.pri" )

# Fix Deps and linker flags for win32 cross compile
x-win32:include("../../x-win32-dll.pri")
QT += network \
    xml \
    xmlpatterns \
    svg \
    sql
TARGET = Events
TEMPLATE = lib
DEFINES += EVENTS_LIBRARY
SOURCES += InternetConnectionStatusChangedEvent.cpp \
    FunctionSelectedEvent.cpp \
    Playlist/PlaylistItemSelectedEvent.cpp \
    CentralUIWidgetChangedEvent.cpp \
    ShellWidgetMovedEvent.cpp \
    SnapshotCaptureEnabledEvent.cpp \
    Player/PlayerStoppedEvent.cpp \
    Player/PlayerStateChangedEvent.cpp \
    SeekBar/SeekBarPlayHeadChangedEvent.cpp \
    Player/PlayerExitClickedEvent.cpp \
    Player/PlayerVisibilityChangedEvent.cpp \
    ChapterSelect/ChapterSelectedEvent.cpp \
    Player/VideoScreenMouseMovedEvent.cpp \
    Player/PlayerCloseButtonClickedEvent.cpp \
    KeyPressedEvent.cpp \
    ControllBar/ControllBarVisibilityChangedEvent.cpp \
    Player/FullscreenStatusChangedEvent.cpp \
    Player/PlayerForwardSpeedChangedEvent.cpp \
    Player/PlayerBackwardSpeedChangedEvent.cpp \
    SnapshotCreator/SnapshotCreationRequestedEvent.cpp \
    SnapshotCreator/SnapshotCreatedEvent.cpp \
    Player/PlayerVolumeChangedEvent.cpp \
    Playlist/PlaylistNavigationKeyPressedEvent.cpp \
    FocusedPlayListItemChangedEvent.cpp \
    Playlist/PlaylistCreatedEvent.cpp \
    Playlist/PlaylistContentTypeChangedEvent.cpp \
    ControllBar/ControllBarMuteButtonClickedEvent.cpp \
    MainUIRegionFocusChangedEvent.cpp \
    MediaLibrary/CoverImageChangedEvent.cpp \
    SnapshotCreator/SnapshotCreatedResponseData.cpp \
    MediaLibrary/CoverImageRequestedEvent.cpp \
    MediaLibrary/CancelCoverImageCreationEvent.cpp \
    SnapshotCreator/CancelSnapshotCreationEvent.cpp \
    CentralFocusChangedEvent.cpp \
    Playlist/PlaylistNextItemSelectedEvent.cpp \
    Playlist/PlaylistItemDropEvent.cpp \
    Playlist/EndOfPlaylistEvent.cpp \
    Playlist/PlaylistPreviousItemSelectedEvent.cpp \
    Playlist/MediaItemContentChangedEvent.cpp \
    Playlist/PlaylistClearEvent.cpp \
    Playlist/PlaylistItemRemovedEvent.cpp \
    Playlist/AddedItemToPlaylistEvent.cpp \
    Playlist/PlaylistGenerationFinishedEvent.cpp \
    Player/PlayerAspectRatioChangedEvent.cpp \
    ControllBar/UpdateControllBarModelEvent.cpp \
    SeekBar/UpdateSeekBarModelEvent.cpp \
    ChapterSelect/UpdateChapterSelectModelEvent.cpp
HEADERS += Events_global.h \
    EventTypes.h \
    InternetConnectionStatusChangedEvent.h \
    FunctionSelectedEvent.h \
    Playlist/PlaylistItemSelectedEvent.h \
    CentralUIWidgetChangedEvent.h \
    CentralUIRegionResizedEvent.h \
    ShellWidgetMovedEvent.h \
    ControllBar/ControllBarPlayButtonClickedEvent.h \
    ControllBar/ControllBarBackwardButtonClickedEvent.h \
    ControllBar/ControllBarPauseButtonClickedEvent.h \
    ControllBar/ControllBarForwardButtonClickedEvent.h \
    ControllBar/ControllBarPreviewButtonClickedEvent.h \
    ControllBar/ControllBarSnapshotButtonClickedEvent.h \
    ControllBar/ControllBarFullscreenButtonClickedEvent.h \
    SnapshotCaptureEnabledEvent.h \
    Player/PlayerTimeChangedEvent.h \
    Player/PlayerStoppedEvent.h \
    Player/PlayerStateChangedEvent.h \
    SeekBar/SeekBarPlayHeadChangedEvent.h \
    Player/PlayerExitClickedEvent.h \
    Player/PlayerVisibilityChangedEvent.h \
    ChapterSelect/ChapterSelectedEvent.h \
    Player/VideoScreenMouseMovedEvent.h \
    Player/PlayerCloseButtonClickedEvent.h \
    KeyPressedEvent.h \
    ControllBar/ControllBarVisibilityChangedEvent.h \
    Player/FullscreenStatusChangedEvent.h \
    Player/PlayerForwardSpeedChangedEvent.h \
    Player/PlayerBackwardSpeedChangedEvent.h \
    SnapshotCreator/SnapshotCreationRequestedEvent.h \
    SnapshotCreator/SnapshotCreatedEvent.h \
    Player/PlayerVolumeChangedEvent.h \
    Playlist/PlaylistNavigationKeyPressedEvent.h \
    FocusedPlayListItemChangedEvent.h \
    Playlist/PlaylistCreatedEvent.h \
    Playlist/PlaylistContentTypeChangedEvent.h \
    Player/PlayerTrackStartedEvent.h \
    ControllBar/ControllBarMuteButtonClickedEvent.h \
    MainUIRegionFocusChangedEvent.h \
    MediaLibrary/CoverImageChangedEvent.h \
    SnapshotCreator/SnapshotCreationRequestData.h \
    SnapshotCreator/SnapshotCreatedResponseData.h \
    MediaLibrary/CoverImageRequestedEvent.h \
    MediaLibrary/CancelCoverImageCreationEvent.h \
    SnapshotCreator/CancelSnapshotCreationEvent.h \
    CentralFocusChangedEvent.h \
    Playlist/PlaylistNextItemSelectedEvent.h \
    Playlist/PlaylistItemDropEvent.h \
    MediaLibrary/ChapterImageRequestEvent.h \
    MediaLibrary/ChapterImageCreatedEvent.h \
    MediaLibrary/CancelChapterImageRequestEvent.h \
    Playlist/EndOfPlaylistEvent.h \
    Playlist/SearchChangedEvent.h \
    Playlist/PlaylistPreviousItemSelectedEvent.h \
    Playlist/MediaDiscoveredEvent.h \
    Playlist/PlaylistItemRemovedEvent.h \
    Playlist/MediaItemContentChangedEvent.h \
    Playlist/PlaylistClearEvent.h \
    RequestAlbumCoverImageEvent.h \
    MetaDataChangedEvent.h \
    Playlist/AddedItemToPlaylistEvent.h \
    Playlist/PlaylistGenerationFinishedEvent.h \
    Player/PlayerAspectRatioChangedEvent.h \
    ControllBar/ControllBarResolutionButtonClickedEvent.h \
    Playlist/StartupPlaylistGenerationStartedEvent.h \
    Playlist/StartupPlaylistGenerationFinishedEvent.h \
    Playlist/SearchPlaylistGenerationStartedEvent.h \
    Playlist/SearchEndedEvent.h \
    Playlist/CachedPlaylistRestoredEvent.h \
    Playlist/MediaVanishedEvent.h \
    Playlist/MediaRemovedEvent.h \
    Playlist/RemovedItemFromPlaylistEvent.h \
    TaskManager/TaskExecuteRequestEvent.h \
    TaskManager/TaskCancelRequestEvent.h \
    RssFeed/RssFeedParseRequestEvent.h \
    RssFeed/RssFeedParseResponseEvent.h \
    RssFeed/RssFeedPlaylistCreationRequestEvent.h \
    StateDisplay/StatusChangedEvent.h \
    StateDisplay/HoverBoxChangeVisibilityEvent.h \
    StateDisplay/HoverBoxHasFocusEvent.h \
    TaskManager/TaskRepositoryTaskAddedEvent.h \
    TaskManager/TaskRepositoryTaskRemovedEvent.h \
    TaskManager/TaskStartedEvent.h \
    TaskManager/TaskFinishedEvent.h \
    TaskManager/TaskCanceledEvent.h \
    Player/BufferingAnimationChangeVisibilityEvent.h \
    MediaWatcher/MediaWatcherAddExclusionEvent.h \
    MediaWatcher/MediaWatcherFileAddedEvent.h \
    MediaWatcher/MediaWatcherFileRemovedEvent.h \
    MediaWatcher/MediaWatcherEvents.h \
    MediaWatcher/MediaWatcherRemoveExclusionEvent.h \
    Player/PlayerMuteEvent.h \
    ControllBar/UpdateControllBarModelEvent.h \
    SeekBar/UpdateSeekBarModelEvent.h \
    ChapterSelect/UpdateChapterSelectModelEvent.h \
    Player/PlayerPositionChangedEvent.h \
    Player/PlayerRequestTimeChangeEvent.h \
    Statistics/CreateStatisticEntryEvent.h \
    Statistics/PlayerStartedStatisticsEntryEvent.h \
    Statistics/PodCastViewStatisticEntryEvent.h \
    UserSettings/ShowUserSettingsDialogEvent.h \
    UserSettings/UserSettingsChangedEvent.h \
    Update/UpdateAvailableForInstallEvent.h \
    Playlist/DocumentOpenedEvent.h \
    TaskManager/TaskProgressEvent.h \
    ContentManagement/NewContentAvailableEvent.h \
    ContentManagement/AvailableContentCountChangedEvent.h \
    ContentManagement/BulkNewContentAvailableEvent.h \
    ContentManagement/RemoveContentEvent.h \
    ContentManagement/InitiateContentDownloadEvent.h \
    ContentManagement/SkipContentEvent.h \
    ContentManagement/CancelContentDownloadEvent.h \
    ContentManagement/ContentDownloadStartedEvent.h \
    ContentManagement/ContentDownloadProgressEvent.h \
    ContentManagement/ContentDownloadFinishedEvent.h \
    ContentManagement/ContentDownloadCanceledEvent.h \
    StateDisplay/RequestHoverBoxChangeVisibilityEvent.h \
    ContentManagement/BanContentEvent.h \
    UserSettings/FileExtensionsSettingsChangedEvent.h \
    UserSettings/ScanedDirectoriesSettingsChangedEvent.h
win32_libs = $$DESTDIR/Container.dll \
    $$DESTDIR/Interfaces.dll \
    $$DESTDIR/ToolLib.dll
unix_libs = $$DESTDIR/libContainer.so \
    $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libToolLib.so
macx_libs = $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
    $$DESTDIR/liblog4qt-0.3.1.dylib

include("../../libref.pri")
