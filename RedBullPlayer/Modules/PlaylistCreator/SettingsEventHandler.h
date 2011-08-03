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
#ifndef SETTINGSEVENTHANDLER_H
#define SETTINGSEVENTHANDLER_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QQueue>

#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Events/UserSettings/FileExtensionsSettingsChangedEvent.h"
#include "../../Events/UserSettings/ScanedDirectoriesSettingsChangedEvent.h"

#include "DynamicFileExtensionsPlaylistProcessorThread.h"
#include "ScanedDirectoriesPlaylistProcessorThread.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlaylistCreator {
            class SettingsEventHandler  : public QObject {
                    Q_OBJECT

                public:
                    SettingsEventHandler( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *mediaLibrary, QObject* parent );
                    ~SettingsEventHandler();

                protected:
                    bool event( QEvent *event );
                    void onFileExtensionsSettingsChanged( RedBullPlayer::Events::FileExtensionsSettingsChangedEvent *e );
                    void onScanedDirectoriesSettingsChanged( RedBullPlayer::Events::ScanedDirectoriesSettingsChangedEvent *e );
                    void enqueueThread( QThread *processor );

                protected slots:
                    void playlistProcessorThreadFinished();

                private:
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary *_mediaLibrary;
                    QQueue<QThread*> _playlistProcessorThreads;
            };
        }
    }
}

#endif // SETTINGSEVENTHANDLER_H
