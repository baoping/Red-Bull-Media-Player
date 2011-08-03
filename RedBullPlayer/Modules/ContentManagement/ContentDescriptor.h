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
#ifndef CONTENTDESCRIPTOR_H
#define CONTENTDESCRIPTOR_H

//Qt includes
#include <QObject>

//Project includes
#include "../../Interfaces/MediaLibrary/IContent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class ContentDescriptor : public QObject {
                    Q_OBJECT
                public:
                    ContentDescriptor( RedBullPlayer::Modules::MediaLibrary::IContent* content,
                                       QString imageDownloadUrl,
                                       QObject* parent = 0 ) {
                        _content = content;
                        _imageDownloadUrl = imageDownloadUrl;
                    }

                    ~ContentDescriptor() {}

                    QString imageDownloadUrl() const {
                        return _imageDownloadUrl;
                    }
                    RedBullPlayer::Modules::MediaLibrary::IContent* content() const {
                        return _content;
                    }

                private:
                    RedBullPlayer::Modules::MediaLibrary::IContent* _content;

                    QString _imageDownloadUrl;
            };
        }
    }
}

#endif // CONTENTDESCRIPTOR_H
