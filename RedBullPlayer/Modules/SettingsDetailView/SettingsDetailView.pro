include( "../../../globals.pri" )
x-win32:include("../../../x-win32-dll.pri")
QT += network \
    xml \
    svg \
    sql
TARGET = SettingsDetailViewModule
TEMPLATE = lib
CONFIG += plugin
SOURCES += SettingsDetailViewModule.cpp \
    SettingsDetailViewUI.cpp \
    SetupFileParser/SettingsViewSetupFileParser.cpp \
    SettingsViewFactory.cpp \
    SectionView.cpp \
    SubSectionView.cpp \
    FileExtensionsSettingView.cpp \
    BooleanSettingView.cpp \
    ScanedDirectorySettingView.cpp \
    AddScanedDirectorySettingView.cpp \
    SettingsPresenter.cpp \
    FullScreenSettingModel.cpp \
    LoopPlaylistSettingModel.cpp \
    ContentDownloadSettingModel.cpp \
    CollectStatisticsSettingModel.cpp \
    VideoFileExtensionsSettingModel.cpp \
    AudioFileExtensionsSettingModel.cpp \
    ImageFileExtensionsSettingModel.cpp \
    TextFileExtensionsSettingModel.cpp \
    PresentationFileExtensionsSettingModel.cpp \
    SpreadsheetFileExtensionsSettingModel.cpp \
    ScanedDirectoriesSettingModel.cpp \
    ScanedDirectoriesSettingController.cpp \
    OthersFileExtensionsSettingModel.cpp \
    PdfFileExtensionsSettingModel.cpp \
    IntegerSettingView.cpp \
    LoopIntervalSettingModel.cpp \
    StringListComparer.cpp
HEADERS += SettingsDetailViewModule.h \
    SettingsDetailViewUI.h \
    SetupFileParser/SettingsViewSetupFileParser.h \
    SetupFileParser/SubSectionSetupObject.h \
    SetupFileParser/SectionSetupObject.h \
    SetupFileParser/SettingSetupObject.h \
    SettingsViewFactory.h \
    SectionView.h \
    SubSectionView.h \
    FileExtensionsSettingView.h \
    BooleanSettingView.h \
    ScanedDirectorySettingView.h \
    AddScanedDirectorySettingView.h \
    SettingsPresenter.h \
    FullScreenSettingModel.h \
    LoopPlaylistSettingModel.h \
    ContentDownloadSettingModel.h \
    CollectStatisticsSettingModel.h \
    VideoFileExtensionsSettingModel.h \
    AudioFileExtensionsSettingModel.h \
    ImageFileExtensionsSettingModel.h \
    TextFileExtensionsSettingModel.h \
    PresentationFileExtensionsSettingModel.h \
    SpreadsheetFileExtensionsSettingModel.h \
    ScanedDirectoriesSettingModel.h \
    ScanedDirectoriesSettingController.h \
    OthersFileExtensionsSettingModel.h \
    PdfFileExtensionsSettingModel.h \
    IntegerSettingView.h \
    LoopIntervalSettingModel.h \
    StringListComparer.h
win32_libs = $$DESTDIR/Interfaces.dll \
    $$DESTDIR/Container.dll \
    $$DESTDIR/PlayerShell.dll \
    $$DESTDIR/EventAggregator.dll \
    $$DESTDIR/Events.dll \
    $$DESTDIR/Widgets.dll \
    $$DESTDIR/ToolLib.dll
unix_libs = $$DESTDIR/libInterfaces.so \
    $$DESTDIR/libContainer.so \
    $$DESTDIR/libPlayerShell.so \
    $$DESTDIR/libEventAggregator.so \
    $$DESTDIR/libEvents.so \
    $$DESTDIR/libWidgets.so \
    $$DESTDIR/libToolLib.so
macx_libs = $$DESTDIR/libInterfaces.1.0.0.dylib \
    $$DESTDIR/libContainer.1.0.0.dylib \
    $$DESTDIR/libPlayerShell.1.0.0.dylib \
    $$DESTDIR/libEventAggregator.1.0.0.dylib \
    $$DESTDIR/libEvents.1.0.0.dylib \
    $$DESTDIR/libWidgets.1.0.0.dylib \
    $$DESTDIR/libToolLib.1.0.0.dylib \
		$$DESTDIR/libWidgets.1.0.0.dylib \
		$$DESTDIR/liblog4qt-0.3.1.dylib
include("../../../libref.pri")
include("../../../macbinaries.pri")
unix:copy_media.commands = test \
    -d \
    "$$DESTDIR/media/$$TARGET" \
    || \
    mkdir \
    -p \
    $$DESTDIR/media/$$TARGET \
    && \
    cp \
    -vr \
    media/$$TARGET \
    $$DESTDIR/media
win32:image_src_path = $$PWD/media/$$TARGET//*
win32:image_dst_path = $$DESTDIR/media/$$TARGET//
win32:copy_media.commands += ..\..\..\copy_files.bat \
    $$replace(image_src_path,/,\) \
    $$replace(image_dst_path,/,\)
POST_TARGETDEPS += copy_media
QMAKE_EXTRA_TARGETS += copy_media
FORMS += SettingsDetailViewUI.ui \
    SectionView.ui \
    SubSectionView.ui \
    FileExtensionsSettingView.ui \
    BooleanSettingView.ui \
    ScanedDirectorySettingView.ui \
    AddScanedDirectorySettingView.ui \
    IntegerSettingView.ui
RESOURCES += settingsDetailView.qrc
