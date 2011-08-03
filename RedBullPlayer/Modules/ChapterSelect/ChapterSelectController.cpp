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
#include "ChapterSelectController.h"

#include <QBitmap>
#include <QPainter>
#include <QDebug>

#include "Chapter.h"
#include "../../Container/Constants.h"
#include "../../Interfaces/IChapter.h"

using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;

ChapterSelectController::ChapterSelectController( RedBullPlayer::PlayerShell::Shell* shell,
        RedBullPlayer::PlayerShell::Region* region,
        ChapterSelectModel* model,
        ChapterSelectUI* view,
        QObject *parent ) :
    QObject( parent ) {
    _shell = shell;
    _region = region;
    _model = model;
    connect( _model, SIGNAL( mediaChanged() ), this, SLOT( onMediaChanged() ) );
    _view = view;
    connect( _view, SIGNAL( carrouselItemClicked( int ) ), this, SLOT( chapterSelected( int ) ) );
    _chapterSelectEventBridge = createChapterSelectEventBridge( true );
    _controllBarEventBridge = createControllBarEventBridge( true );
    _regionInitialized = false;
    _regionVisible = false;
    _carrouselInitializedWithPlaceHolder = false;

    _imageGenerationDelayTimer = new QTimer( this );
    _imageGenerationDelayTimer->setSingleShot( true );
    _imageGenerationDelayTimer->setInterval( 10 );
    connect( _imageGenerationDelayTimer, SIGNAL( timeout() ), this, SLOT( onImageGenerationDelayTimer() ) );

    _initImageTimer = new QTimer( this );
    _initImageTimer->setSingleShot( true );
    _initImageTimer->setInterval( 5 );
    connect( _initImageTimer, SIGNAL( timeout() ), this, SLOT( onInitImageTimer() ) );

    _currentChapterIndex = 0;
    initializeRegion();
}

ChapterSelectController::~ChapterSelectController() {
    delete _chapterSelectEventBridge;
    delete _controllBarEventBridge;
    delete _initImageTimer;
    delete _imageGenerationDelayTimer;
}

ChapterSelectEventBridge* ChapterSelectController::createChapterSelectEventBridge( bool connectSignals ) {
    ChapterSelectEventBridge* chapterSelectEventBridge = new ChapterSelectEventBridge( this );

    if ( connectSignals ) {
        connect( chapterSelectEventBridge,
                 SIGNAL( updateChapterSelectModel( RedBullPlayer::Modules::MediaLibrary::IMedia* ) ),
                 this,
                 SLOT( updateChapterSelectModel( RedBullPlayer::Modules::MediaLibrary::IMedia* ) ) );
        connect( chapterSelectEventBridge,
                 SIGNAL( playerTimeChanged( qint64 ) ),
                 this,
                 SLOT( playerTimeChanged( qint64 ) ) );
        connect( chapterSelectEventBridge,
                 SIGNAL( playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum ) ),
                 this,
                 SLOT( playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum ) ) );
        connect( chapterSelectEventBridge,
                 SIGNAL( centralUIRegionResized() ),
                 this,
                 SLOT( centralUIRegionResized() ) );
        connect( chapterSelectEventBridge,
                 SIGNAL( chapterImageCreated( QUuid, float ) ),
                 this,
                 SLOT( chapterImageCreated( QUuid, float ) ) );
    }

    return chapterSelectEventBridge;
}

ControllBarEventBridge* ChapterSelectController::createControllBarEventBridge( bool connectSignals ) {
    ControllBarEventBridge* controllBarEventBridge = new ControllBarEventBridge( this );

    if ( connectSignals ) {
        connect( controllBarEventBridge,
                 SIGNAL( previewClicked() ),
                 this,
                 SLOT( previewClicked() ) );
        connect( controllBarEventBridge,
                 SIGNAL( controllBarVisibilityChanged( bool ) ),
                 this,
                 SLOT( controllBarVisiblityChanged( bool ) ) );
    }

    return controllBarEventBridge;
}

void ChapterSelectController::updateChapterSelectModel( IMedia* media ) {
    if( _model->resolveMediaType( media ) == IMediaModel::DOCUMENT )
        return;

    _model->setMedia( media );
    reset();

    if( _regionVisible ) {
        setVisible( false );
    }
}

void ChapterSelectController::reset() {
    cancelImageRequests();
    _carrouselInitializedWithPlaceHolder = false;
    _currentChapterIndex = 0;
    _imageGenerationDelayTimer->stop();
    _initImageTimer->stop();
}

void ChapterSelectController::playerTimeChanged( qint64 newTime ) {
}

void ChapterSelectController::playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState ) {
    switch( newState ) {
        case MediaPlayerState::PLAYING:
            break;

        case MediaPlayerState::PAUSED:
        case MediaPlayerState::STOPPED:
        case MediaPlayerState::ENDED:
            break;

        default:
            break;
    }
}

void ChapterSelectController::previewClicked() {
    setVisible( !_regionVisible );

    if( _regionVisible ) {
        initChapterCarrouselWithPlaceHolder();
        _imageGenerationDelayTimer->start(); //delayed generation
    }
}

void ChapterSelectController::centralUIRegionResized() {
    _region->widget()->move( getPosition() );
}

void ChapterSelectController::controllBarVisiblityChanged( bool controllBarVisible ) {
    if( controllBarVisible && _regionVisible ) {
        _region->widget()->hide(); //Don't even ask why this event chain, so the Mac can refresh the ChapterselectUi without borders
        _view->hide();
        _region->widget()->move( getPosition() );
        _region->widget()->show();
        _view->show();
        _view->update();
    } else {
        _region->widget()->hide();
    }
}

QPoint ChapterSelectController::getPosition() {
    QWidget* controlBarRegion = _shell->findRegion( "ControllBarRegion" )->widget();
    int x = controlBarRegion->mapToGlobal( QPoint( 0, 0 ) ).x();
    int y = controlBarRegion->mapToGlobal( QPoint( 0, 0 ) ).y() - Constants::CHAPTER_SELECT_BOTTOM - Constants::CHAPTER_SELECT_HEIGHT;
    return QPoint( x, y );
}

QBitmap ChapterSelectController::getRoundCornerMask( QSize size ) {
    QBitmap bmp( size );
    bmp.fill( Qt::color0 );
    QPainter painter( &bmp );
    painter.setPen( Qt::color1 );
    painter.setBrush( Qt::color1 );
    painter.drawRoundedRect( 0, 0, size.width(), size.height(), 15, 15 );
    return bmp;
}

void ChapterSelectController::initializeRegion() {
    _region->widget()->setAttribute( Qt::WA_TranslucentBackground, true );
    _region->widget()->resize( Constants::CHAPTER_SELECT_WIDTH, Constants::CHAPTER_SELECT_HEIGHT );
    _region->widget()->move( getPosition() );
    _regionInitialized = true;
}

void ChapterSelectController::setVisible( bool visible ) {
    if( !_regionInitialized ) {
        initializeRegion();
    }

    if( visible )  {
        _region->widget()->hide();
        _view->hide();
        _region->widget()->move( getPosition() );
        _region->widget()->show();
        _view->show();
        _view->update();
    } else {
        _region->widget()->hide();
    }

    _view->onVisibilityChange( visible );
    _regionVisible = visible;
}

void ChapterSelectController::onInitImageTimer() {
    //Takes next from init queue and request snapshot creation if not set
    if( _initQueue.isEmpty() ) return;

    IChapter* chapter = _initQueue.dequeue();
    IMedia* media = _model->mediaLibrary()->mediaDB()->mediaRepository()->getMediaByGuid( _model->id() );
    ISnapshot* snapshot = _model->mediaLibrary()->mediaDB()
                          ->snapshotRepository()->getSnapshotsByMediaAnPosition( media, chapter->position(), 0 );
    int idx = chapterIndexByGuid( chapter->id() );

    if ( snapshot != 0 ) {
        _view->changeItemChapterImage( idx , snapshot->image(), true ); //refactor view to use guid instead of index!
    } else {
        _requestList.append( chapter );
        _chapterSelectEventBridge->requestChapterImage( _model->id(),
                chapter->position(),
                ( uint ) ( idx * ( int ) _model->chapterDuration() / 1000 ) );
    }

    if( !_initQueue.isEmpty() )
        _initImageTimer->start();
}

int ChapterSelectController::chapterIndexByGuid( QUuid guid ) {
    for( int i = 0; i < _chapterList.count(); i++ ) {
        if ( _chapterList.at( i )->id() == guid )
            return i;
    }

    return -1;
}

int ChapterSelectController::chapterIndexByPosition( float position ) {
    for( int i = 0; i < _chapterList.count(); i++ ) {
        if ( _chapterList.at( i )->position() == position )
            return i;
    }

    return -1;
}

void ChapterSelectController::onImageGenerationDelayTimer() {
    //Enqueue all from current position to end
    for( int i = _currentChapterIndex; i < _chapterList.count(); i++ )
        _initQueue.enqueue( _chapterList.at( i ) );

    //Enqueue from the start to current position
    for( int i = 0; i < _currentChapterIndex; i++ )
        _initQueue.enqueue( _chapterList.at( i ) );

    //If any items start init delay timer
    if( !_initQueue.isEmpty() )
        _initImageTimer->start();
}

void ChapterSelectController::initChapterCarrouselWithPlaceHolder() {
    if ( !_carrouselInitializedWithPlaceHolder ) {
        _requestList.clear();
        _chapterList.clear();
        _initQueue.clear();
        QImage placeHolder = QImage( Constants::CHAPTER_SELECT_IMAGE_DIR + Constants::CHAPTER_SELECT_DEFAULT_PREVIEW_IMAGE );
        int chapterIndex = 0;

        for( int i = 0; i < _model->chapterCount(); i++ ) {
            Chapter *chapter = new Chapter();
            float duration = i * _model->chapterDuration();
            float chapterPosition = positionInVideo( (int)duration );
            chapter->setSnapshot( placeHolder );
            chapter->setTimeInMS( (int)duration );
            chapter->setPosition( chapterPosition );
            _chapterList.append( chapter );

            if ( ( chapterPosition <= _model->position() ) &&
                    ( _model->position() < positionInVideo( ( i + 1 )*_model->chapterDuration() ) ) ) {
                chapterIndex = i;
            }
        }

        _view->setCurrentVideoLengthInSeconds( int ( _model->length() / 1000 ) );
        _view->setItems( _chapterList, chapterIndex );
        _currentChapterIndex = chapterIndex;
        _carrouselInitializedWithPlaceHolder = true;
    }
}

float ChapterSelectController::positionInVideo( qint64 duration ) {
    return ( _model->length() != 0 ) ? ( float )duration / _model->length() : 0;
}

void ChapterSelectController::cancelImageRequests() {
    while ( !_requestList.isEmpty() ) {
        IChapter* chapter = _requestList.takeFirst();
        _chapterSelectEventBridge->cancelChapterImageRequest( _model->id(),
                chapter->position() );
    }
}

void ChapterSelectController::chapterSelected( int time ) {
    _chapterSelectEventBridge->chapterSelected( time );
}

void ChapterSelectController::chapterImageCreated( QUuid mediaId, float position ) {
    if ( mediaId == _model->id() ) {
        QImage chapterImage;
        int indexOfItem = chapterIndexByPosition( position );
        Error error;
        IMedia* media = _model->mediaLibrary()->mediaDB()->mediaRepository()->getMediaByGuid( mediaId );
        ISnapshot *snapshot = _model->mediaLibrary()->mediaDB()->snapshotRepository()->getSnapshotsByMediaAnPosition( media, position, &error );
        Error::raise( &error );

        if( snapshot != NULL ) {
            chapterImage = snapshot->image();
        } else {
            chapterImage = QImage( Constants::CHAPTER_SELECT_IMAGE_DIR + Constants::CHAPTER_SELECT_DEFAULT_PREVIEW_IMAGE );
        }

        _view->changeItemChapterImage( indexOfItem, chapterImage, true );
        _requestList.removeAll( _chapterList.at( indexOfItem ) );
    }
}
