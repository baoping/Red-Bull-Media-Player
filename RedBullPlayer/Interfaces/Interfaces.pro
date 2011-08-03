include("../../globals.pri")

# Fix Deps and linker flags for win32 cross compile
x-win32:include("../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = Interfaces
TEMPLATE = lib
DEFINES += INTERFACES_LIBRARY
SOURCES += Player/IMediaModel.cpp
HEADERS += Interfaces_global.h \
    IXmlNewsFeed.h \
    IRootModule.h \
    INewsTicker.h \
    IInternetConnectionChecker.h \
    MediaLibrary/IMediaLibrary.h \
    MediaLibrary/IMediaDB.h \
    MediaLibrary/IMediaType.h \
    MediaLibrary/ITag.h \
    MediaLibrary/ITagRepository.h \
    IBottomUI.h \
    IFunctionSelect.h \
    MediaLibrary/IMedia.h \
    MediaLibrary/IMediaRepository.h \
    ICentralUI.h \
    IPlaylistUI.h \
    IPlaylistItem.h \
    Player/IPlayer.h \
    IControllBar.h \
    ISeekBar.h \
    ITopUI.h \
    ISearchUI.h \
    IChapterSelect.h \
    IChapter.h \
    IPlaylistCreator.h \
    IPlayerCloseButton.h \
    IVolumeBar.h \
    PlaylistObjects/IPlaylist.h \
    PlaylistObjects/IPlaylistItem.h \
    PlaylistObjects/IPlaylistCreator.h \
    ISnapshotCreator.h \
    ISnapshotCreationRequest.h \
    IListViewPlaylist.h \
    IGridViewPlaylist.h \
    ICoverViewPlaylist.h \
    IPlaylist.h \
    MediaLibrary/ISnapshotRepository.h \
    MediaLibrary/ISnapshot.h \
    IPodCatcher.h \
    INewsDetailView.h \
    MediaLibrary/IMediaMetadataHandler.h \
    IAboutDetailView.h \
    MediaLibrary/ISearchEngine.h \
    IAlbumCoverCollector.h \
    IPlayerUpdateChecker.h \
    StyleFactory/ITextStyle.h \
    StyleFactory/IStyleFactory.h \
    TaskManager/ITask.h \
    TaskManager/ITaskManager.h \
    RssFeed/IRssFeed.h \
    RssFeed/IRssFeedItem.h \
    RssFeed/IRssFeedImageDescriptor.h \
    StateDisplay/IStateDisplay.h \
    StateDisplay/IStateView.h \
    StateDisplay/ITaskView.h \
    StateDisplay/ITaskPresenter.h \
    StateDisplay/ITaskModel.h \
    StateDisplay/ITaskViewFactory.h \
    StateDisplay/IStateViewFactory.h \
    FileSystem/IFileSystem.h \
    FileSystem/IStorage.h \
    FileSystem/IStorageReservation.h \
    TaskManager/Types.h \
    Enumerations/TaskStateEnum.h \
    Enumerations/TaskResultEnum.h \
    Enumerations/TaskPurposeEnum.h \
    MediaLibrary/INews.h \
    MediaLibrary/INewsImage.h \
    MediaLibrary/INewsRepository.h \
    MediaWatcher/IMediaWatcher.h \
    StateDisplay/IDiskSpaceModel.h \
    StateDisplay/IDiskSpacePresenter.h \
    StateDisplay/IStatePresenter.h \
    MediaLibrary/ISystemMetaDataRepository.h \
    Enumerations/MediaPlayerStateEnum.h \
    Player/IMediaModel.h \
    MediaLibrary/IStatisticClientCacheRepository.h \
    MediaLibrary/IStatisticEntry.h \
    Statistics/IStatistics.h \
    Statistics/IUrlGenerator.h \
    UserSettings/IUserSettings.h \
    UpdateInstaller/IUpdateInstaller.h \
    UserSettings/ISettingsDetailView.h \
    UserSettings/ISettingsViewFactory.h \
    UserSettings/ISettingsSectionView.h \
    UserSettings/ISettingsSettingView.h \
    UserSettings/ISetupSettingObject.h \
    UserSettings/ISetupSubSectionObject.h \
    UserSettings/ISetupSectionObject.h \
    UserSettings/ISetupFileParser.h \
    UserSettings/ISettingsSubSectionView.h \
    UserSettings/ISettingsPresenter.h \
    UserSettings/ISettingsBooleanSettingView.h \
    UserSettings/ISettingsScanDirectorySettingView.h \
    UserSettings/ISettingsFileExtensionsSettingView.h \
    UserSettings/ISettingsAddDirectorySettingView.h \
    UserSettings/ISettingsModel.h \
    ContentManagement/IContentManagement.h \
    MediaLibrary/IContent.h \
    MediaLibrary/IContentRepository.h \
    UserSettings/ISettingsIntegerSettingView.h \
    Enumerations/ContentCategoryEnum.h \
    ContentManagement/IContentListView.h \
    ContentManagement/IContentItemView.h \
    StateDisplay/IInternetAvailabilityModel.h \
    StateDisplay/IInternetAvailabilityPresenter.h
unix_libs = $$DESTDIR/libContainer.so
win32_libs = $$DESTDIR/Container.dll
macx_libs = $$DESTDIR/libContainer.1.0.0.dylib

# include("../../macbinaries.pri")
include("../../libref.pri")
