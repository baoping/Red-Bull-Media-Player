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
#include "PlaylistUI.h"
#include "ui_PlaylistUI.h"

// Qt Includes
#include <QDebug>
#include <QFileDialog>
#include <QPainter>
#include <QDropEvent>
#include <QUrl>

// Project Includes
#include "../../Container/Constants.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/Playlist/PlaylistNavigationKeyPressedEvent.h"
#include "../../Events/Playlist/PlaylistItemDropEvent.h"
#include "../../Events/Playlist/PlaylistItemRemovedEvent.h"
#include "../../Events/Playlist/PlaylistItemDropEvent.h"
#include "../../ToolLib/ConfigManager.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/FileSystem/Storage.h"
#include "../../ToolLib/FileSystem/StorageFactory.h"
#include "../../Widgets/RedBullDialog.h"
#include "../../UpdateServices/PlayerVersion.h"
#include "../MediaLibrary/Media.h"
#include "../MediaLibrary/MediaType.h"
#include "DeleteBox.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::Playlist;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::Playlist::CoverViewPlaylist;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Updater;

PlaylistUI::PlaylistUI( Shell *shell, QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::PlaylistUI ) {
    _shell = shell;
    _ui->setupUi( this );
    _hasCentralFocus = true;
    _hasMainUIRegionFocus = false;
    _mainUIRegionGroup = MainUIRegionFocusChangedEvent::CENTRAL;
    _currentSelectedMediaId = QUuid();
    _currentTitle = "";
    _leftButton = NULL;
    _rightButton = NULL,
    setNavigation();
    setupPlaylistButtons();
    setupPlaylistStackedWidget();
    SetPlaylistType( COVERVIEW );
    setupFreeDiskSpaceCheck();
}

PlaylistUI::~PlaylistUI() {
    _freeDiskSpaceCheckTimer->stop();
    delete _ui;
}

void PlaylistUI::setupMediaLibraryConnection() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary *mediaLibrary = qobject_cast<IMediaLibrary*>( plugin );

        if ( mediaLibrary ) {
            _mediaDB = mediaLibrary->mediaDB();
        } else {
            error->setErrorMessage( tr( "Could not cast MediaLibrary Plugin!" ), Error::Normal );
        }
    } else {
        error->setErrorMessage( tr( "Could not find MediaLibrary Plugin!" ), Error::Normal );
    }
}

void PlaylistUI::resolveStyleFactory() {
    SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTDETAILVIEW, Q_FUNC_INFO );
    Error *error = new Error();
    Plugin* plugin = Container::instance()->findModule( "StyleFactory" );

    if ( plugin ) {
        IStyleFactory* retVal = qobject_cast<IStyleFactory*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast StyleFactory Plugin!" ), Error::Normal );
        }

        _styleFactory = retVal;
        SimpleLogger::instance()->debug( Constants::LOGGER_ABOUTDETAILVIEW, "Resolving succeded" );
    } else {
        error->setErrorMessage( tr( "Could not find StyleFactory Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

void PlaylistUI::setupStyle() {
    if( _styleFactory == NULL )
        return;

    setupTitleStyle();
}

void PlaylistUI::setupTitleStyle() {
    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "CoverView_HeaderText", &ok );

    if( ok ) {
        QString titleStyle = QString( "background-color: transparent;" );
        titleStyle += titleTextStyle->styleSheet();
        _ui->_title->setStyleSheet( titleStyle );
        _ui->_title->setAlignment( titleTextStyle->alignment() );
    }
}

void PlaylistUI::setNavigation() {
    QWidget *rootWidget = _shell->findRegion( "Root" )->widget();

    if( _leftButton == NULL ) {
        _leftButton = new CommandButton( _ui->_mainWidget,
                                         Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_NAVIGATION_LEFT_ARROW_URL,
                                         Constants::PLAYLISTUI_ARROW_WIDTH,
                                         Constants::PLAYLISTUI_ARROW_HEIGHT );
        _leftButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_NAVIGATION_LEFT_ARROW_DISABLED_URL );
        connect( _leftButton, SIGNAL( clicked() ), this, SLOT( onLeftNavigationClicked() ) );
    }

    _leftButton->move( Constants::PLAYLISTUI_ARROW_MARGIN, 0 );

    if( _rightButton == NULL ) {
        _rightButton = new CommandButton( _ui->_mainWidget,
                                          Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_NAVIGATION_RIGHT_ARROW_URL,
                                          Constants::PLAYLISTUI_ARROW_WIDTH,
                                          Constants::PLAYLISTUI_ARROW_HEIGHT );
        _rightButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_NAVIGATION_RIGHT_ARROW_DISABLED_URL );
        connect( _rightButton, SIGNAL( clicked() ), this, SLOT( onRightNavigationClicked() ) );
    }

    _rightButton->move( rootWidget->width() - _rightButton->width() - Constants::PLAYLISTUI_ARROW_MARGIN, 0 );
}

int PlaylistUI::resolveTitleWidth() {
    QWidget *rootWidget = _shell->findRegion( "Root" )->widget();
    int titleIdealWidth = rootWidget->width() - _ui->_addRemoveFromPlaylistWidget->width() -
                          2 * Constants::PLAYLISTUI_ARROW_MARGIN - _ui->_playlistViewsWidget->width()
                          - 10;

    if( _ui->_title->width() < titleIdealWidth )
        return titleIdealWidth;
    else
        return _ui->_title->width();
}

void PlaylistUI::setupPlaylistButtons() {
    _gridViewButton = new ToogleButton( this,
                                        Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_GRID_VIEW_ICON,
                                        Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_GRID_VIEW_ICON_CHECKED );
    _gridViewButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_GRID_VIEW_ICON_DISABLED );
    connect( _gridViewButton, SIGNAL( clicked() ), this, SLOT( gridViewClicked() ) );
    _listViewButton = new ToogleButton( this,
                                        Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_LIST_VIEW_ICON,
                                        Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_LIST_VIEW_ICON_CHECKED );
    _listViewButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_LIST_VIEW_ICON_DISABLED );
    connect( _listViewButton, SIGNAL( clicked() ), this, SLOT( listViewClicked() ) );
    _coverflowViewButton = new ToogleButton( this,
            Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_COVERFLOW_ICON,
            Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_COVERFLOW_ICON_CHECKED );
    _coverflowViewButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_COVERFLOW_ICON_DISABLED );
    connect( _coverflowViewButton, SIGNAL( clicked() ), this, SLOT( coverflowViewClicked() ) );
    QBoxLayout *playlistViewsLayout = qobject_cast<QBoxLayout*>( _ui->_playlistViewsWidget->layout() );

    if( playlistViewsLayout != NULL ) {
        playlistViewsLayout->insertWidget( 0, _coverflowViewButton );
        playlistViewsLayout->insertWidget( 0, _listViewButton );
        playlistViewsLayout->insertWidget( 0, _gridViewButton );
    }

    _coverflowViewButton->setChecked( true );
    _plusButton = new CommandButton( this, Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_PLUS_ICON );
    _plusButton->setMouseOverImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_PLUS_ICON_MOUSEOVER );
    _plusButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_PLUS_ICON_DISABLED );
    connect( _plusButton, SIGNAL( clicked() ), this, SLOT( plusClicked() ) );
    _minusButton = new CommandButton( this, Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_MINUS_ICON );
    _minusButton->setMouseOverImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_MINUS_ICON_MOUSEOVER );
    _minusButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_MINUS_ICON_DISABLED );
    connect( _minusButton, SIGNAL( clicked() ), this, SLOT( minusClicked() ) );
    QBoxLayout *addRemoveLayout = qobject_cast<QBoxLayout*>( _ui->_addRemoveFromPlaylistWidget->layout() );

    if( addRemoveLayout != NULL ) {
        addRemoveLayout->insertWidget( 0, _minusButton );
        addRemoveLayout->insertWidget( 0, _plusButton );
    }
}

void PlaylistUI::setupPlaylistStackedWidget() {
    //registered inside the UI_playlistWidget register where is corect in stacked Widget
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "GridViewPlaylistRegion", _ui->_gridViewPage ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "CoverViewPlaylistRegion", _ui->_coverViewPage ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "ListViewPlaylistRegion", _ui->_listViewPage ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "TVPlaylistRegion", _ui->_tvPage ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "NewsPlaylistRegion", _ui->_newsPage ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "AboutPlaylistRegion", _ui->_aboutPage ) );
    _shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "SettingsPlaylistRegion", _ui->_settingsPage ) );
    SetPlaylistType( COVERVIEW );
}

void PlaylistUI::setupFreeDiskSpaceCheck() {
    _freeDiskSpaceCheckTimer = new QTimer( this );
    _freeDiskSpaceCheckTimer->setSingleShot( false );
    _freeDiskSpaceCheckTimer->setInterval( 1000 );
    connect( _freeDiskSpaceCheckTimer, SIGNAL( timeout() ),
             this, SLOT( handleCurrentFreeDiskSpace() ) );
    _freeDiskSpaceCheckTimer->start();
}

void PlaylistUI::handleCurrentFreeDiskSpace() {
    if( _currentPlaylistType == PlaylistUI::GRIDVIEW ||
            _currentPlaylistType == PlaylistUI::LISTVIEW ||
            _currentPlaylistType == PlaylistUI::COVERVIEW )
        _plusButton->setEnabled( StorageFactory::defaultStorage()->freeBytes() > 0 );
}

bool PlaylistUI::onLeftNavigationClicked() {
    if( _leftButton->isEnabled() ) {
        emitNavigationKeyPressedEvent( PlaylistNavigationKeyPressedEvent::LEFT );
        return true;
    }

    return false;
}


bool PlaylistUI::onRightNavigationClicked() {
    if( _rightButton->isEnabled() ) {
        emitNavigationKeyPressedEvent( PlaylistNavigationKeyPressedEvent::RIGHT );
        return true;
    }

    return false;
}


void PlaylistUI::setNavigationKeysState( EndOfPlaylistEvent::EndOfPlaylistDirection endOfPlaylist ) {
    _leftButton->setEnabled( true );
    _rightButton->setEnabled( true );

    switch( endOfPlaylist ) {
        case EndOfPlaylistEvent::LEFT : {
                _leftButton->setEnabled( false );
                break;
            }

        case EndOfPlaylistEvent::RIGHT : {
                _rightButton->setEnabled( false );
                break;
            }

        case EndOfPlaylistEvent::BOTH : {
                _leftButton->setEnabled( false );
                _rightButton->setEnabled( false );
                break;
            }

        default:
            break;
    }
}

void PlaylistUI::handleFocusedPlaylistItemChanged( QString title, QUuid mediaId ) {
    changeTitle( title );

    if( !mediaId.isNull() ) //null test because NEWS TV ABOUT Views throw it with null and their are not valid playlists
        _currentSelectedMediaId = mediaId;
}

void PlaylistUI::changeTitle( QString title ) {
    QString visibleText = "";
    QFontMetrics fontMeter = QFontMetrics( _ui->_title->font() );
    visibleText = fontMeter.elidedText( title, Qt::ElideRight, resolveTitleWidth() );
    _ui->_title->setText( visibleText );
    _currentTitle = title;
}


void PlaylistUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}


void PlaylistUI::resizeEvent( QResizeEvent *e ) {
    setNavigation();
    _ui->_gridViewPage->resize( _ui->_mainWidget->width(), _ui->_mainWidget->height() );
    _ui->_coverViewPage->resize( _ui->_mainWidget->width(), _ui->_mainWidget->height() );
    _ui->_listViewPage->resize( _ui->_mainWidget->width(), _ui->_mainWidget->height() );
    changeTitle( _currentTitle );
}

void PlaylistUI::gridViewClicked() {
    SetPlaylistType( GRIDVIEW );
}

void PlaylistUI::listViewClicked() {
    SetPlaylistType( LISTVIEW );
}

void PlaylistUI::coverflowViewClicked() {
    SetPlaylistType( COVERVIEW );
}

void PlaylistUI::plusClicked() {
    QString path = "..";

    if ( !ConfigManager::instance()->getLastDialogDirectory().isEmpty() ) {
        path = ConfigManager::instance()->getLastDialogDirectory();
    } else {
        QDir dir( ConfigManager::instance()->getMediaDirectory() );
        dir.cdUp();
        path = dir.absolutePath();
    }

    QString nameFilter = ConfigManager::instance()->getAllNameFilter() + ";;" +
                         ConfigManager::instance()->getVideoNameFilter() + ";;" +
                         ConfigManager::instance()->getAudioNameFilter()  + ";;" +
                         ConfigManager::instance()->getImageNameFilter() + ";;" +
                         ConfigManager::instance()->getDocumentNameFilter() + ";;";
    QString selNameFilter = ConfigManager::instance()->getAllNameFilter();
    QStringList fileNames = QFileDialog::getOpenFileNames( NULL,
                            tr( "Choose Files to add to Playlist" ),
                            path,
                            nameFilter,
                            &selNameFilter );

    if ( fileNames.count() > 0 ) {
        ConfigManager::instance()->setLastDialogDirectory( QFileInfo( fileNames.at( 0 ) ).absolutePath() );
        tryAddFiles( fileNames );
    }
}

void PlaylistUI::tryAddFiles( QStringList fileNames ) {
    //Check if enough space is free to add file!
    quint64 sizeSum = 0;

    for( int i = 0; i < fileNames.count(); i++ ) {
        QFileInfo fileInfo( fileNames.at( i ) );
        sizeSum += fileInfo.size();
    }

    if ( StorageFactory::defaultStorage()->freeBytes() > sizeSum ) {
        PlaylistItemDropEvent e( fileNames );
        Aggregator::instance()->sendEvent( &e );
    } else {
        showDiskSpaceError();
    }
}

void PlaylistUI::showDiskSpaceError() {
    QString styleSheet = "";

    if ( _styleFactory != 0 ) {
        bool ok;
        ITextStyle* style = _styleFactory->getTextStyle( "UserSettingsDialog_Text", &ok );

        if ( ok ) {
            styleSheet = style->styleSheet();
        }
    }

    QString dialogText;
    QFile downloadFinishedFile( Constants::ROOT_UI_TEXT_DIR + Constants::DISKSPACE_FULL_TEXT );

    if ( downloadFinishedFile.open( QFile::ReadOnly ) ) {
        QByteArray fileArray = downloadFinishedFile.readAll();
        downloadFinishedFile.close();
        dialogText = QString( fileArray );
    } else {
        dialogText = "There is not enough disk space left on your volume. Please free up some space. Until that Red Bull Media Player will not fully function.";
    }

    PlayerVersion::instance()->parseString( &dialogText );
    RedBullDialog* dialog = new RedBullDialog( dialogText,
            styleSheet,
            RedBullDialog::Ok,
            0 );
    dialog->exec();
}


void PlaylistUI::minusClicked() {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTUI, Q_FUNC_INFO );

    if( _currentSelectedMediaId.isNull() )
        return;

    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTUI, "currentMediaId : " + _currentSelectedMediaId.toString() );
    IMedia *currentItem = _mediaDB->mediaRepository()->getMediaByGuid( _currentSelectedMediaId );

    if( currentItem == NULL ) {
        return;
    }

    QString fileName = currentItem->fileName();
    QString text = "";

    if( fileName.contains( ConfigManager::instance()->getMediaDirectory() ) )
        text = QString( Constants::PLAYLISTUI_SURE ).arg( fileName );
    else
        text = QString( Constants::PLAYLISTUI_SURE_ONLYPLAYLIST ).arg( fileName );

    bool ok;
    ITextStyle *textStyle = _styleFactory->getTextStyle( "DeleteBox_Text", &ok );
    DeleteBox *deleteBox = new DeleteBox( text, textStyle, _shell );
    int retVal = deleteBox->exec();

    if( retVal == 1 ) {
        QStringList list;
        list.append( fileName );
        PlaylistItemRemovedEvent e( list );
        Aggregator::instance()->sendEvent( &e );
    }
}

ICoverViewPlaylist *PlaylistUI::loadMediaCoverView() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "CoverViewPlaylist", "MediaCoverViewPlaylist" );

    if ( plugin ) {
        ICoverViewPlaylist* retVal = qobject_cast<ICoverViewPlaylist*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast ICoverViewPlaylist Plugin!" ), Error::Normal );
        }

        return retVal;
    } else {
        error->setErrorMessage( tr( "Could not find ICoverViewPlaylist Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
    return NULL;
}

void  PlaylistUI::SetPlaylistType( PlaylistTypes playlistType ) {
    _currentPlaylistType = playlistType;

    switch( playlistType ) {
        case GRIDVIEW :
            _lastMediaType = GRIDVIEW;
            _plusButton->setEnabled( true );
            _minusButton->setEnabled( true );
            _gridViewButton->setEnabled( true );
            _listViewButton->setEnabled( true );
            _coverflowViewButton->setEnabled( true );
            _gridViewButton->setChecked( true );
            _coverflowViewButton->setChecked( false );
            _listViewButton->setChecked( false );
            _ui->_playlistStackedWidget->setCurrentIndex( GRIDVIEW );
            emitCentralFocusChangedEvent( CentralFocusChangedEvent::GRIDVIEW_PLAYLIST );
            break;

        case COVERVIEW :
            _lastMediaType = COVERVIEW;
            _plusButton->setEnabled( true );
            _minusButton->setEnabled( true );
            _gridViewButton->setEnabled( true );
            _listViewButton->setEnabled( true );
            _coverflowViewButton->setEnabled( true );
            _gridViewButton->setChecked( false );
            _coverflowViewButton->setChecked( true );
            _listViewButton->setChecked( false );
            _ui->_playlistStackedWidget->setCurrentIndex( COVERVIEW );
            emitCentralFocusChangedEvent( CentralFocusChangedEvent::COVERVIEW_PLAYLIST );
            break;

        case LISTVIEW :
            _lastMediaType = LISTVIEW;
            _plusButton->setEnabled( true );
            _minusButton->setEnabled( true );
            _gridViewButton->setEnabled( true );
            _listViewButton->setEnabled( true );
            _coverflowViewButton->setEnabled( true );
            _gridViewButton->setChecked( false );
            _coverflowViewButton->setChecked( false );
            _listViewButton->setChecked( true );
            _ui->_playlistStackedWidget->setCurrentIndex( LISTVIEW );
            emitCentralFocusChangedEvent( CentralFocusChangedEvent::LISTVIEW_PLAYLIST );
            break;

        case TV:
            _plusButton->setEnabled( false );
            _minusButton->setEnabled( false );
            _gridViewButton->setEnabled( false );
            _listViewButton->setEnabled( false );
            _coverflowViewButton->setEnabled( false );
            _gridViewButton->setChecked( false );
            _coverflowViewButton->setChecked( true );
            _listViewButton->setChecked( false );
            _ui->_playlistStackedWidget->setCurrentIndex( TV );
            _coverflowViewButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_COVERFLOW_ICON_CHECKED );
            emitCentralFocusChangedEvent( CentralFocusChangedEvent::TV_PLAYLIST );
            break;

        case NEWS:
            _plusButton->setEnabled( false );
            _minusButton->setEnabled( false );
            _gridViewButton->setEnabled( false );
            _listViewButton->setEnabled( false );
            _coverflowViewButton->setEnabled( false );
            _gridViewButton->setChecked( false );
            _coverflowViewButton->setChecked( true );
            _listViewButton->setChecked( false );
            _ui->_playlistStackedWidget->setCurrentIndex( NEWS );
            _coverflowViewButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_COVERFLOW_ICON_CHECKED );
            emitCentralFocusChangedEvent( CentralFocusChangedEvent::NEWS_PLAYLIST );
            break;

        case ABOUT:
            _plusButton->setEnabled( false );
            _minusButton->setEnabled( false );
            _gridViewButton->setEnabled( false );
            _listViewButton->setEnabled( false );
            _coverflowViewButton->setEnabled( false );
            _gridViewButton->setChecked( false );
            _coverflowViewButton->setChecked( true );
            _listViewButton->setChecked( false );
            _coverflowViewButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_COVERFLOW_ICON_CHECKED );
            _ui->_playlistStackedWidget->setCurrentIndex( ABOUT );
            emitCentralFocusChangedEvent( CentralFocusChangedEvent::ABOUT_PLAYLIST );
            break;

        case SETTINGS:
            _plusButton->setEnabled( false );
            _minusButton->setEnabled( false );
            _gridViewButton->setEnabled( false );
            _listViewButton->setEnabled( false );
            _coverflowViewButton->setEnabled( false );
            _gridViewButton->setChecked( false );
            _coverflowViewButton->setChecked( true );
            _listViewButton->setChecked( false );
            _coverflowViewButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_COVERFLOW_ICON_CHECKED );
            _ui->_playlistStackedWidget->setCurrentIndex( SETTINGS );
            emitCentralFocusChangedEvent( CentralFocusChangedEvent::SETTINGS_PLAYLIST );
            break;

        case MEDIA:
            _coverflowViewButton->setDisabledImage( Constants::PLAYLISTUI_IMAGE_DIR + Constants::PLAYLISTUI_COVERFLOW_ICON_DISABLED );
            SetPlaylistType( _lastMediaType );
            break;
    }
}

void PlaylistUI::emitNavigationKeyPressedEvent( PlaylistNavigationKeyPressedEvent::Direction direction ) {
    PlaylistNavigationKeyPressedEvent e( direction );
    Aggregator::instance()->sendEvent( &e );
}

void PlaylistUI::emitCentralFocusChangedEvent( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    CentralFocusChangedEvent e( widgetType );
    Aggregator::instance()->sendEvent( &e );
}

void PlaylistUI::handleCentralFocusChanged( CentralFocusChangedEvent::CentralWidgetType widgetType ) {
    if( widgetType == CentralFocusChangedEvent::COVERVIEW_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::LISTVIEW_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::GRIDVIEW_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::NEWS_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::TV_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::ABOUT_PLAYLIST ||
            widgetType == CentralFocusChangedEvent::SETTINGS_PLAYLIST ) {
        _hasCentralFocus = true;
    } else {
        _hasCentralFocus = false;
    }
}

void PlaylistUI::changeMainUIRegionFocusGraphics( MainUIRegionFocusChangedEvent::UIRegion mainUIRegion ) {
    if( _mainUIRegionGroup == mainUIRegion )
        _hasMainUIRegionFocus = true;
    else
        _hasMainUIRegionFocus = false;
}

bool PlaylistUI::hasMainFocus() {
    return _hasMainUIRegionFocus;
}

bool PlaylistUI::hasCentralFocus() {
    return _hasCentralFocus;
}

void PlaylistUI::dropEvent( QDropEvent *e ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTUI, "Drop Event" );

    if ( !e->mimeData()->hasUrls() || e->proposedAction() == Qt::LinkAction ) {
        e->ignore();
        return;
    }

    e->acceptProposedAction();
    QList<QUrl> urls = e->mimeData()->urls();
    QStringList list;

    for( int i = 0; i < urls.size(); i++ ) {
        if ( ConfigManager::instance()->isInWhiteList( urls[i].toLocalFile() ) ) {
            list.append( urls[i].toLocalFile() );
        }
    }

    qDebug() << "DropEvent => ItemCount=" << list.count();

    if ( list.count() > 0 ) {
        tryAddFiles( list );
    } else {
        e->ignore();
    }
}

void PlaylistUI::dragEnterEvent( QDragEnterEvent *e ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_PLAYLISTUI, "Drag Enter Event" );
    dragMoveEvent( e );
}

void PlaylistUI::dragMoveEvent( QDragMoveEvent *e ) {
    if ( e->mimeData()->hasUrls() ) {
        QList<QUrl> urls = e->mimeData()->urls();
        QStringList list;

        for( int i = 0; i < urls.size(); i++ ) {
            if ( ConfigManager::instance()->isInWhiteList( urls[i].toLocalFile() ) ) {
                list.append( urls[i].toLocalFile() );
            }
        }

        if ( list.count() > 0 ) {
            e->acceptProposedAction();
        } else {
            e->ignore();
        }
    }
}

void PlaylistUI::clearTitle() {
    _ui->_title->clear();
    _currentTitle = "";
}

