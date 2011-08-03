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
#ifndef CHAPTERSELECTMODEL_H
#define CHAPTERSELECTMODEL_H

#include <QObject>

#include "../../Interfaces/Player/IMediaModel.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"

#include "ChapterSelectModelEventBridge.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ChapterSelect {

            class ChapterSelectModel : public RedBullPlayer::Modules::Player::IMediaModel {
                    Q_OBJECT

                public:
                    explicit ChapterSelectModel( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject *parent = 0 );
                    ~ChapterSelectModel();

                    qint64 length() const {
                        return _length;
                    }
                    float position() const {
                        return _position;
                    }
                    void setPosition( float position );


                    qint64 chapterDuration() const {
                        return _chapterDuration;
                    }
                    qint64 chapterCount() const {
                        return _chapterCount;
                    }

                protected:
                    virtual void onNewMedia( RedBullPlayer::Modules::MediaLibrary::IMedia* media );

                private:
                    ChapterSelectModelEventBridge* createEventBridge( bool connectSignals = true );
                    qint64 getChapterDuration();
                    qint64 getChapterCount();

                    qint64 _length;
                    float _position;
                    qint64 _chapterDuration;
                    qint64 _chapterCount;

                    ChapterSelectModelEventBridge* _eventBridge;

                private slots:
                    void playerPositionChanged( float position );

                signals:
                    void positionChanged();
            };
        }
    }
}

#endif // CHAPTERSELECTMODEL_H
