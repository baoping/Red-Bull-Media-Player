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
#include "ChapterSelectModel.h"

//Project includes
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Modules::ChapterSelect;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Tools;

ChapterSelectModel::ChapterSelectModel( RedBullPlayer::Modules::MediaLibrary::IMediaLibrary* mediaLibrary, QObject *parent ) :
    IMediaModel( mediaLibrary, parent ) {
    _eventBridge = createEventBridge( true );
}

ChapterSelectModel::~ChapterSelectModel() {
    delete _eventBridge;
}

void ChapterSelectModel::onNewMedia( RedBullPlayer::Modules::MediaLibrary::IMedia* media ) {
    _length = media->length();
    _position = 0;
    _chapterDuration = getChapterDuration();
    _chapterCount = getChapterCount();
}

void ChapterSelectModel::setPosition( float position ) {
    _position = position;
    emit positionChanged();
}

ChapterSelectModelEventBridge* ChapterSelectModel::createEventBridge( bool connectSignals ) {
    ChapterSelectModelEventBridge* eventBridge = new ChapterSelectModelEventBridge( this );

    if ( connectSignals ) {
        connect( eventBridge,
                 SIGNAL( playerPositionChanged( float ) ),
                 this,
                 SLOT( playerPositionChanged( float ) ) );
    }

    return eventBridge;
}

void ChapterSelectModel::playerPositionChanged( float position ) {
    setPosition( position );
}

qint64 ChapterSelectModel::getChapterDuration() {
    qint64 chapterDuration = 0;

    if( _length <= ( 1000 * ConfigManager::instance()->shortVideoUpperLimit() ) ) {
        chapterDuration = 1000 * ConfigManager::instance()->shortVideoChapterDuration();
    } else {
        if( _length <= ( 1000 * ConfigManager::instance()->middleVideoUpperLimit() ) ) {
            chapterDuration = 1000 * ConfigManager::instance()->middleVideoChapterDuration();
        } else {
            chapterDuration = 1000 * ConfigManager::instance()->longVideoChapterDuration();
        }
    }

    return chapterDuration;
}

qint64 ChapterSelectModel::getChapterCount() {
    qint64 chapterCount = 0;
    chapterCount = ( int )( _length / _chapterDuration ) + 1;

    if ( ( _length % _chapterDuration ) == 0 )
        chapterCount--;

    return chapterCount;
}
