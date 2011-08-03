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
#include "VolumeBarModule.h"

// QT includes
#include <QtPlugin>
#include <QDebug>
#include <QLayout>

// Project Includes
#include "../../Container/Error.h"
#include "../../Container/Container.h"
#include "../../Container/Constants.h"
#include "../../EventAggregator/Aggregator.h"
#include "../../Events/EventTypes.h"
#include "../../Events/Player/PlayerVolumeChangedEvent.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Interfaces/Enumerations/MediaPlayerStateEnum.h"

using namespace RedBullPlayer::Modules::VolumeBar;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::Player;

VolumeBarModule::VolumeBarModule() : IVolumeBar() {
    _count = 6;
}

void VolumeBarModule::Setup( QObject* shell, QDomElement config ) {
    _shell = qobject_cast<Shell*>( shell );
    Q_ASSERT( _shell != 0 );
    Q_ASSERT( ! config.isNull() );
    QString regionName = config.attribute( "UIRegion" );
    Q_ASSERT( ! regionName.isNull() );
    _region = _shell->findRegion( regionName );
    Q_ASSERT( _region != 0 );
    initVolumeBars();
    _currentLevel = -1;
    Aggregator::instance()->registerListener( PlayerStateChanged, this );
    Aggregator::instance()->registerListener( PlayerTrackStarted, this );
    resolveMediaLibrary();
}

void VolumeBarModule::initVolumeBars() {
    int itemWidth = Constants::VOLUME_BAR_WIDTH / _count;
    int itemHeight = Constants::VOLUME_BAR_HEIGHT / _count;

    for( int i = 0; i < _count; i++ ) {
        _volumeBars[i] = new Sprite( _region->widget(), i );
        _volumeBars[i]->setBgColor( Constants::VOLUME_BAR_INACTIVE_COLOR );
        _volumeBars[i]->setGeometry( i * itemWidth, _region->widget()->height() - itemHeight * ( i + 1 ), itemWidth, itemHeight * ( i + 1 ) );
        connect( _volumeBars[i], SIGNAL( clicked( int ) ), this, SLOT( clickedSlot( int ) ) );
        connect( _volumeBars[i], SIGNAL( mouseOver( int ) ), this, SLOT( mouseOverSlot( int ) ) );
        connect( _volumeBars[i], SIGNAL( mouseOut( int ) ), this, SLOT( mouseOutSlot( int ) ) );
    }
}

void VolumeBarModule::clickedSlot( int id ) {
    updateVolumeBars( id );
    int percentage = (int) ( ( ( float ) ( id + 1 ) / ( float ) _count ) * 100 );
    PlayerVolumeChangedEvent e( percentage );
    Aggregator::instance()->sendEvent( &e );
}

void VolumeBarModule::mouseOverSlot( int id ) {
    for( int i = 0; i < _count; i++ ) {
        if( i <= id ) {
            _volumeBars[i]->setBgColor( Constants::VOLUME_BAR_MOUSE_OVER_COLOR );
        }
    }
}

void VolumeBarModule::mouseOutSlot( int /*id*/ ) {
    updateVolumeBars( _currentLevel );
}

void VolumeBarModule::updateVolumeBars( int newLevel ) {
    _currentLevel = newLevel;

    for( int i = 0; i < _count; i++ ) {
        if( i <= _currentLevel ) {
            _volumeBars[i]->setBgColor( Constants::VOLUME_BAR_ACTIVE_COLOR );
        } else {
            _volumeBars[i]->setBgColor( Constants::VOLUME_BAR_INACTIVE_COLOR );
        }
    }
}

bool VolumeBarModule::event ( QEvent * e ) {
    if( e->type() == PlayerStateChanged ) {
        onPlayerStateChanged( static_cast<PlayerStateChangedEvent*>( e ) );
    }

    if( e->type() == PlayerTrackStarted ) {
        onPlayerTrackStarted( static_cast<PlayerTrackStartedEvent*>( e ) );
    }

    return false;
}

void VolumeBarModule::onPlayerStateChanged( PlayerStateChangedEvent *ev ) {
    switch( ev->state() ) {
        case MediaPlayerState::PLAYING:
            if( _currentLevel == -1 ) {
                updateVolumeBars( Constants::VOLUME_BAR_START_LEVEL - 1 );
                int percentage = (int) ( ( ( float ) ( Constants::VOLUME_BAR_START_LEVEL ) / ( float ) _count ) * 100 );
                PlayerVolumeChangedEvent e( percentage );
                Aggregator::instance()->sendEvent( &e );
            }

            break;

        default:
            break;
    }
}

void VolumeBarModule::onPlayerTrackStarted( PlayerTrackStartedEvent* e ) {
    IMedia *media = _mediaLibrary->mediaDB()->mediaRepository()->getMediaByGuid( e->mediaId() );

    if ( media == 0 ) {
        if( e->mediaTypeId() == _mediaLibrary->mediaDB()->tvChannelType()->guid() ) {
            setEnabled( true );
            _enabled = true;
        }

        return;
    }

    if( media->mediaType().guid() == _mediaLibrary->mediaDB()->imageType()->guid() ) {
        setEnabled( false );
        _enabled = false;
    } else {
        setEnabled( true );
        _enabled = true;
    }
}

void VolumeBarModule::setEnabled( bool enabled ) {
    for( int i = 0; i < 6; i++ ) {
        _volumeBars[i]->setEnabled( enabled );
    }
}

void VolumeBarModule::resolveMediaLibrary() {
    Error *error = new Error();
    Plugin* plugin = RedBullPlayer::Container::Container::instance()->findModule( "MediaLibrary" );

    if ( plugin ) {
        IMediaLibrary* retVal = qobject_cast<IMediaLibrary*>( plugin );

        if ( retVal == 0 ) {
            error->setErrorMessage( tr( "Could not cast PlaylistCreator Plugin!" ), Error::Normal );
        }

        _mediaLibrary = retVal;
    } else {
        error->setErrorMessage( tr( "Could not find PlaylistCreator Plugin!" ), Error::Normal );
    }

    Error::raise( error );
    delete error;
}

Q_EXPORT_PLUGIN2( VolumeBarModule, RedBullPlayer::Modules::VolumeBar::VolumeBarModule )
