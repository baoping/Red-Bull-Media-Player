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
#ifndef IMEDIAMODEL_H
#define IMEDIAMODEL_H

#include <QObject>
#include <QUuid>
#include <QMetaType>

#include "../MediaLibrary/IMediaLibrary.h"
#include "../MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {
            class IMediaModel : public QObject {
                    Q_OBJECT
                    Q_ENUMS( MediaTypeEnum )
                public:
                    enum MediaTypeEnum {
                        UNKOWN = 0,
                        VIDEO = 1,
                        AUDIO = 2,
                        IMAGE = 3,
                        STREAM = 4,
                        DOCUMENT = 5
                    };

                    explicit IMediaModel( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject *parent = 0 );
                    virtual ~IMediaModel() {}

                    void setMedia( RedBullPlayer::Modules::MediaLibrary::IMedia* media );

                    QUuid id() const {
                        return _id;
                    }
                    bool hasMedia() const {
                        return _hasMedia;
                    }
                    MediaTypeEnum mediaType() const {
                        return _mediaType;
                    }

                    bool isDocument() const {
                        return _isDocument;
                    }

                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary() const {
                        return _mediaLibrary;
                    }

                    MediaTypeEnum resolveMediaType( RedBullPlayer::Modules::MediaLibrary::IMedia* media );
                    static QString mediaTypeString( MediaTypeEnum mediaType );

                protected:
                    virtual void onNewMedia( RedBullPlayer::Modules::MediaLibrary::IMedia* media ) = 0;

                private:
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    QUuid _id;
                    MediaTypeEnum _mediaType;
                    bool _hasMedia;
                    bool _isDocument;

                signals:
                    void mediaChanged();
            };
        }
    }
}

Q_DECLARE_METATYPE( RedBullPlayer::Modules::Player::IMediaModel::MediaTypeEnum );

#endif // IMEDIAMODEL_H
