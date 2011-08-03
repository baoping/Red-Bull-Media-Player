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
#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QObject>
#include <QUuid>
#include <QImage>

//Project includes
#include "../../Interfaces/Player/IMediaModel.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {
            class PlayerModel : public IMediaModel {
                    Q_OBJECT
                public:
                    explicit PlayerModel( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject *parent = 0 );

                    QString fileName() const {
                        return _fileName;
                    }
                    qint64 startTime() const {
                        return _startTime;
                    }
                    QImage coverImage() const {
                        return _coverImage;
                    }

                    QString animationFile() const {
                        return _animationFile;
                    }
                    int animationFrameCount() const {
                        return _animationFrameCount;
                    }

                    qint64 length() const {
                        return _length;
                    }

                    bool isPodCast() const {
                        return _isPodCast;
                    }
                    QString podCastId() const {
                        return _podCastId;
                    }

                    bool isNew() const {
                        return _isNew;
                    }
                    void setIsNew( bool isNew );

                    QUuid mediaTypeId() const {
                        return _mediaTypeId;
                    }

                    RedBullPlayer::Modules::MediaLibrary::IMedia* updateLength( qint64 length );
                    RedBullPlayer::Modules::MediaLibrary::IMedia* saveStartTime( qint64 startTime );

                protected:
                    virtual void onNewMedia( RedBullPlayer::Modules::MediaLibrary::IMedia* media );

                private: //data
                    QString _fileName;
                    qint64 _startTime;
                    qint64 _length;
                    QImage _coverImage;
                    QString _animationFile;
                    int _animationFrameCount;
                    bool _isPodCast;
                    QString _podCastId;
                    bool _isNew;
                    bool _isDocument;
                    QUuid _mediaTypeId;
            };
        }
    }
}

#endif // PLAYERMODEL_H
