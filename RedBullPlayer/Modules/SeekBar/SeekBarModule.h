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
#ifndef SEEKBARMODULE_H
#define SEEKBARMODULE_H

//QT Includes
#include <QObject>

// Project Includes
#include "../../Interfaces/ISeekBar.h"
#include "../../PlayerShell/Shell.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"

//MVC
#include "SeekBarUI.h"
#include "SeekBarModel.h"
#include "SeekBarController.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SeekBar {
            class SeekBarModule : public ISeekBar {
                    Q_OBJECT
                    Q_INTERFACES( RedBullPlayer::Modules::SeekBar::ISeekBar )

                public:
                    SeekBarModule();
                    virtual void Setup( QObject* shell, QDomElement config );
                    virtual void SetupModuleConnections();
                    virtual QString name() const {
                        return "SeekBar";
                    }
                    virtual QString version() const {
                        return "1.0.0";
                    }

                protected:

                    void resolveMediaLibrary();

                private:
                    RedBullPlayer::PlayerShell::Shell *_shell;
                    RedBullPlayer::PlayerShell::Region *_region;
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;

                    //MVC
                    SeekBarUI *_ui;
                    SeekBarModel* _model;
                    SeekBarController* _controller;
            };
        }
    }
}

#endif // SEEKBARMODULE_H
