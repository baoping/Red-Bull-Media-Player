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
#include "SeekBarController.h"

#include <QDebug>

using namespace RedBullPlayer::Modules::SeekBar;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Modules::Player;

SeekBarController::SeekBarController( SeekBarModel* model, SeekBarUI* view,  QObject *parent ) :
    QObject( parent ) {
    _model = model;
    connect( _model, SIGNAL( mediaChanged() ), this, SLOT( onMediaChanged() ) );
    _view = view;
    connect( _view, SIGNAL( valueChanged( float ) ), this, SLOT( valueChanged( float ) ) );
    _seekBarEventBridge = createSeekBarEventBridge( true );
}

SeekBarController::~SeekBarController() {
    delete _seekBarEventBridge;
}

SeekBarEventBridge* SeekBarController::createSeekBarEventBridge( bool connectSignals ) {
    SeekBarEventBridge* seekBarEventBridge = new SeekBarEventBridge( this );

    if ( connectSignals ) {
        connect( seekBarEventBridge,
                 SIGNAL( updateSeekBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia* ) ),
                 this,
                 SLOT( updateSeekBarModel( RedBullPlayer::Modules::MediaLibrary::IMedia* ) ) );
        connect( seekBarEventBridge,
                 SIGNAL( playerTimeChanged( qint64 ) ),
                 this,
                 SLOT( playerTimeChanged( qint64 ) ) );
        connect( seekBarEventBridge,
                 SIGNAL( playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum ) ),
                 this,
                 SLOT( playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum ) ) );
        connect( seekBarEventBridge,
                 SIGNAL( bufferingAnimationChangedVisibility( bool ) ),
                 this,
                 SLOT( bufferingAnimationChangedVisibility( bool ) ) );
    }

    return seekBarEventBridge;
}

void SeekBarController::updateSeekBarModel( IMedia* media ) {
    if( _model->resolveMediaType( media ) == IMediaModel::DOCUMENT )
        return;

    _model->setMedia( media );
}

void SeekBarController::onMediaChanged() {
    switch( _model->mediaType() ) {
        case SeekBarModel::VIDEO:
            _view->enable( true );
            break;

        case SeekBarModel::AUDIO:
            _view->enable( true );
            break;

        case SeekBarModel::IMAGE:
            _view->enable( false );
            break;

        case SeekBarModel::STREAM:
            _view->enable( false );
            break;

        case SeekBarModel::DOCUMENT:
        case SeekBarModel::UNKOWN:
            break;
    }
}

void SeekBarController::playerTimeChanged( qint64 newTime ) {
    if ( _model->length() > 0 ) {
        float ratio = ( float )newTime / ( float )_model->length();
        _view->setValue( ratio );
    } else {
        _view->setValue( 0 );
    }
}

void SeekBarController::playerStateChanged( RedBullPlayer::Modules::Player::MediaPlayerState::StateEnum newState ) {
    switch( newState ) {
        case MediaPlayerState::PLAYING:
            break;

        case MediaPlayerState::PAUSED:
        case MediaPlayerState::STOPPED:
        case MediaPlayerState::ENDED:
            break;

        default:
            break;
    }
}

void SeekBarController::bufferingAnimationChangedVisibility( bool visible ) {
    _view->handleBufferingAnimationChangeVisibility( visible );
}

void SeekBarController::valueChanged( float value ) {
    if ( _model->mediaType() != SeekBarModel::STREAM ) {
        _seekBarEventBridge->seekBarValueChanged( value );
    }
}
