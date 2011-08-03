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
#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QUuid>
#include <QPointer>

//Project includes
#include "../../Container/Container.h"
#include "../../Interfaces/MediaLibrary/IMediaLibrary.h"
#include "../../Interfaces/MediaLibrary/IMediaRepository.h"
#include "../../Interfaces/MediaLibrary/IMediaDB.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../Interfaces/IPlaylist.h"
#include "../../Events/Playlist/MediaDiscoveredEvent.h"
#include "../../Events/Playlist/AddedItemToPlaylistEvent.h"


class PlaylistManager {
    public:
        PlaylistManager();
        ~PlaylistManager();

        void setupMediaLibraryConnection();

        void handleMediaDisovered( QUuid mediaId, RedBullPlayer::Events::MediaDiscoveredEvent::MediaDiscoveredSource source );
        void handleStartupPlaylistGenerationStarted( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> playlist );
        void handleStartupPlaylistGenerationFinished();
        void handleSearchPlaylistGenerationStarted( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> playlist );
        void handleSearchEnded();
        void handleMediaRemoved( QUuid mediaId );
        void handleMediaVanished( QUuid mediaId );

    protected:
        void setCurrentPlaylist( QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> playlist );
        QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> getCurrentPlaylist();
        void cacheCurrentPlaylist();
        void restoreCachedPlaylist();
        void appendItemToCurrentPlaylist( QUuid mediaId );
        void prependItemToCurrentPlaylist( QUuid mediaId );
        void appendItemToCachedPlaylist( QUuid mediaId );
        void prependItemToCachedPlaylist( QUuid mediaId );
        void quequePodcast( QUuid mediaId );
        void insertQuequedPodcasts();

        void emitAddedItemToPlaylist( QUuid meidaId, RedBullPlayer::Events::MediaDiscoveredEvent::MediaDiscoveredSource source );
        void emitCachedPlaylistRestored();

    private:
        QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> _currentPlaylist;
        QPointer<RedBullPlayer::Modules::Playlist::IPlaylist> _cachedPlaylist;
        QList<QUuid> _quequedPodcastItems;

        bool _isInSearchMode;
        bool _isInStartupMode;

        RedBullPlayer::Modules::MediaLibrary::IMediaDB *_mediaDB;
};


#endif // PLAYLISTMANAGER_H
