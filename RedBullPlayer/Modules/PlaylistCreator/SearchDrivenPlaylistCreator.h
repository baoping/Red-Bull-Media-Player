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
#ifndef SEARCHDRIVENPLAYLISTCREATOR_H
#define SEARCHDRIVENPLAYLISTCREATOR_H

#include <QObject>
#include <QEvent>
#include <QTimer>
#include <QFutureWatcher>

// Project Includes
#include "../../Interfaces/IPlaylistCreator.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../PlayerShell/Shell.h"
#include "../../Events/Playlist/SearchChangedEvent.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlaylistCreator {
            class SearchDrivenPlaylistCreator : public QObject {
                    Q_OBJECT
                public:
                    SearchDrivenPlaylistCreator( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject* parent );

                protected:
                    virtual bool event( QEvent* ev );
                    virtual void onSearchChangedEvent( RedBullPlayer::Events::SearchChangedEvent* ev );

                protected slots:
                    void timerShot();
                    void searchFinished();
                    void mediaFound( QString uuid );

                private:
                    RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* _mediaLibrary;
                    QString _currentQuery;
                    QTimer* _timer;
                    QFutureWatcher<void>* _futureWatcher;
            };
        }
    }
}
#endif // SEARCHDRIVENPLAYLISTCREATOR_H
