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
#ifndef VOLUMEBARMODULE_H
#define VOLUMEBARMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/IVolumeBar.h"
#include "../../PlayerShell/Shell.h"
#include "../../Widgets/Sprite.h"
#include "../../Events/Player/PlayerStateChangedEvent.h"
#include "../../Events/Player/PlayerTrackStartedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace VolumeBar {
            class VolumeBarModule : public IVolumeBar {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::VolumeBar::IVolumeBar )

                public:
                    VolumeBarModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual QString name() const {
                        return "VolumeBar";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                public slots:
                    void clickedSlot( int id );
                    void mouseOverSlot( int id );
                    void mouseOutSlot( int id );

                protected:
                    void initVolumeBars();
                    void updateVolumeBars( int newLevel );
                    bool event ( QEvent * e );
                    void onPlayerStateChanged( RedBullPlayer::Events::PlayerStateChangedEvent* e );
                    void onPlayerTrackStarted( RedBullPlayer::Events::PlayerTrackStartedEvent* e );
                    void setEnabled( bool enabled );
                    void resolveMediaLibrary();

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    RedBullPlayer::Widgets::Sprite *_volumeBars[6];
                    int _count;
                    int _currentLevel;
                    bool _enabled;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
            };
        }
    }
}

#endif // VOLUMEBARMODULE_H
