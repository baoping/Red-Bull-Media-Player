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
#ifndef CHAPTERSELECTCONTROLLER_H
#define CHAPTERSELECTCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QPoint>

//MVC
#include "ChapterSelectModel.h"
#include "ChapterSelectUI.h"

//Events
#include "ChapterSelectEventBridge.h"
#include "ControllBarEventBridge.h"

//Project includes
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../PlayerShell/Shell.h"


namespace RedBullPlayer {
    namespace Modules {
        namespace ChapterSelect {

            class ChapterSelectController : public QObject {
                    Q_OBJECT
                public:
                    explicit ChapterSelectController( RedBullPlayer::PlayerShell::Shell* shell,
                                                      RedBullPlayer::PlayerShell::Region* region,
                                                      ChapterSelectModel* model,
                                                      ChapterSelectUI* view,
                                                      QObject *parent = 0 );
                    ~ChapterSelectController();

                private:
                    ChapterSelectEventBridge* createChapterSelectEventBridge( bool connectSignals = true );
                    ControllBarEventBridge* createControllBarEventBridge( bool connectSignals = true );

                    QPoint getPosition();
                    QBitmap getRoundCornerMask( QSize size );

                    void initializeRegion();

                    void setVisible( bool visible );

                    void initChapterCarrouselWithPlaceHolder();
                    float positionInVideo( qint64 duration );

                    int chapterIndexByGuid( QUuid guid );
                    int chapterIndexByPosition( float position );

                    void cancelImageRequests();

                    void reset();

                    ChapterSelectModel* _model;
                    ChapterSelectUI* _view;

                    ChapterSelectEventBridge* _chapterSelectEventBridge;
                    ControllBarEventBridge* _controllBarEventBridge;

                    RedBullPlayer::PlayerShell::Shell* _shell;
                    RedBullPlayer::PlayerShell::Region* _region;

                    bool _regionInitialized;
                    bool _regionVisible;

                    bool _carrouselInitializedWithPlaceHolder;

                    int _currentChapterIndex;

                    QTimer* _initImageTimer;
                    QTimer* _imageGenerationDelayTimer;

                    QList<IChapter*> _chapterList;
                    QList<IChapter*> _requestList;
                    QQueue<IChapter*> _initQueue;

                private slots:
                    //MediaPlayer
                    void updateChapterSelectModel( RedBullPlayer::Modules::MediaLibrary::IMedia* media );
                    void playerTimeChanged( qint64 newTime );
                    void playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState );
                    //ControllBar
                    void previewClicked();
                    void controllBarVisiblityChanged( bool controllBarVisible );
                    //UIResize
                    void centralUIRegionResized();
                    //ChapterImage
                    void chapterImageCreated( QUuid mediaId, float position );

                    //Timer
                    void onInitImageTimer();
                    void onImageGenerationDelayTimer();

                    //View
                    void chapterSelected( int time );

            };
        }
    }
}

#endif // CHAPTERSELECTCONTROLLER_H
