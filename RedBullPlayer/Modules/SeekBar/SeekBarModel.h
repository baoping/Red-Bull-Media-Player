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
#ifndef SEEKBARMODEL_H
#define SEEKBARMODEL_H

#include <QObject>

//Porject includes
#include "../../Interfaces/Player/IMediaModel.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SeekBar {

            class SeekBarModel : public RedBullPlayer::Modules::Player::IMediaModel {

                    Q_OBJECT
                public:
                    explicit SeekBarModel( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject *parent = 0 );

                    qint64 length() const {
                        return _length;
                    }

                protected:
                    virtual void onNewMedia( RedBullPlayer::Modules::MediaLibrary::IMedia* media );

                private: //data
                    qint64 _length;
            };
        }
    }
}

#endif // SEEKBARMODEL_H
