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
#ifndef TEXTBROWSER_H
#define TEXTBROWSER_H

#include <QWidget>
#include <QTextBrowser>

#include "../Interfaces/MediaLibrary/IMediaLibrary.h"

namespace RedBullPlayer {
    namespace Widgets {
        class TextBrowser : public QTextBrowser {

            public:
                TextBrowser( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QWidget* parent );

                QVariant loadResource( int type, const QUrl& name );

                void setNewsGuid( QUuid guid ) {
                    _newsGuid = guid;
                }
                QUuid newsGuid() const {
                    return _newsGuid;
                }

            private:
                RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                QUuid _newsGuid;
        };
    }
}

#endif // TEXTBROWSER_H
