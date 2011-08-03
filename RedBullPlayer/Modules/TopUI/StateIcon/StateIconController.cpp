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
#include "StateIconController.h"

#include "../../../ToolLib/ConfigManager.h"
#include "../../../EventAggregator/Aggregator.h"
#include "../../../Events/EventTypes.h"

using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Modules::MediaLibrary;

StateIconController::StateIconController( QWidget* buttonParent, QBoxLayout* buttonContainer, QObject *parent ) :
    QObject( parent ) {
    _buttonParent = buttonParent;
    _buttonContainer = buttonContainer;
}

void StateIconController::init( IStyleFactory *styleFactory, IMediaLibrary *mediaLibrary ) {
    Aggregator::instance()->registerListener( HoverBoxChangeVisibility, this );
    Aggregator::instance()->registerListener( RequestHoverBoxChangeVisibility, this );
    _buttonFactory = new StateDisplayButtonFactory( _buttonParent, styleFactory, this );
    _diskSpaceButton = _buttonFactory->createButtonFor( TaskPurpose::DISK_SPACE );
    _ioButton = _buttonFactory->createButtonFor( TaskPurpose::IO );
    _downloadButton = _buttonFactory->createButtonFor( TaskPurpose::DOWNLOAD );
    _updateButton = _buttonFactory->createButtonFor( TaskPurpose::UPDATE );
    _internetAvailabilityButton = _buttonFactory->createButtonFor( TaskPurpose::INTERNET_AVAILABILITY );
    _diskSpaceModel = new DiskSpaceModel( HoverBoxChangeVisibilityEvent::LOWERRIGHTCORNER, this );
    _diskSpaceModel->addPath( ConfigManager::instance()->getMediaDirectory() );
    _diskSpaceModel->addPath( ConfigManager::instance()->getTempDirectory() );
    _diskSpaceModel->addPath( ConfigManager::instance()->getDeleteDirectory() );
    _ioModel = new StateIconModel( TaskPurpose::IO, HoverBoxChangeVisibilityEvent::LOWERRIGHTCORNER, this );
    _contentModel = new ContentManagementModel( HoverBoxChangeVisibilityEvent::UPPERRIGHTCORNER, this );
    _updateModel = new StateIconModel( TaskPurpose::UPDATE, HoverBoxChangeVisibilityEvent::LOWERRIGHTCORNER, this );
    _internetAvailabilityModel = new InternetAvailabilityModel( HoverBoxChangeVisibilityEvent::LOWERRIGHTCORNER,
            mediaLibrary, this );
    _ioPresenter = new StateIconPresenter( _ioButton, _ioModel, this );
    _contentPresenter = new ContentManagementPresenter( _downloadButton, _contentModel, this );
    _updatePresenter = new StateIconPresenter( _updateButton, _updateModel, this );
    _diskSpacePresenter = new DiskSpacePresenter( _diskSpaceButton, _diskSpaceModel, this );
    _internetAvailabilityPresenter = new InternetAvailabilityPresenter( _internetAvailabilityButton, _internetAvailabilityModel, this );
    //insert buttons
    _buttonContainer->insertWidget( 1, _diskSpaceButton, 0, Qt::AlignTop );
    _buttonContainer->insertWidget( 2, _internetAvailabilityButton, 0, Qt::AlignTop );
    _buttonContainer->insertWidget( 3, _ioButton, 0, Qt::AlignTop );
    _buttonContainer->insertWidget( 4, _updateButton, 0, Qt::AlignTop );
    _buttonContainer->insertWidget( 5, _downloadButton, 0, Qt::AlignTop );
}

bool StateIconController::event( QEvent* event ) {
    if ( event->type() == HoverBoxChangeVisibility ) {
        onHoverBoxChangeVisibility( static_cast<HoverBoxChangeVisibilityEvent*>( event ) );
    }

    if ( event->type() == RequestHoverBoxChangeVisibility ) {
        onRequestHoverBoxChangeVisibility( static_cast<RequestHoverBoxChangeVisibilityEvent*>( event ) );
    }

    return false;
}

void StateIconController::onRequestHoverBoxChangeVisibility( RequestHoverBoxChangeVisibilityEvent* e ) {
    if( e->visible() ) {
        switch( e->statusGroup() ) {
            case TaskPurpose::DISK_SPACE :
                _diskSpacePresenter->requestVisibillityChanged();
                break;

            case TaskPurpose::IO :
                _ioPresenter->requestVisibillityChanged();
                break;

            case TaskPurpose::UPDATE :
                _updatePresenter->requestVisibillityChanged();
                break;

            case TaskPurpose::DOWNLOAD :
                _contentPresenter->requestVisibillityChanged();
                break;

            case TaskPurpose::INTERNET_AVAILABILITY :
                _internetAvailabilityPresenter->requestVisibillityChanged();
                break;

            case TaskPurpose::UNKNOWN :
                break;
        }
    }
}

void StateIconController::onHoverBoxChangeVisibility( HoverBoxChangeVisibilityEvent* e ) {
    if ( e->visible() ) {
        switch( e->statusGroup() ) {
            case ( TaskPurpose::DISK_SPACE ):
                _ioPresenter->disableHoverBox();
                _contentPresenter->disableHoverBox();
                _updatePresenter->disableHoverBox();
                _internetAvailabilityPresenter->disableHoverBox();
                break;

            case ( TaskPurpose::IO ):
                _diskSpacePresenter->disableHoverBox();
                _contentPresenter->disableHoverBox();
                _updatePresenter->disableHoverBox();
                _internetAvailabilityPresenter->disableHoverBox();
                break;

            case ( TaskPurpose::UPDATE ):
                _diskSpacePresenter->disableHoverBox();
                _ioPresenter->disableHoverBox();
                _contentPresenter->disableHoverBox();
                _internetAvailabilityPresenter->disableHoverBox();
                break;

            case ( TaskPurpose::DOWNLOAD ):
                _diskSpacePresenter->disableHoverBox();
                _ioPresenter->disableHoverBox();
                _updatePresenter->disableHoverBox();
                _internetAvailabilityPresenter->disableHoverBox();
                break;

            case ( TaskPurpose::INTERNET_AVAILABILITY ):
                _diskSpacePresenter->disableHoverBox();
                _ioPresenter->disableHoverBox();
                _updatePresenter->disableHoverBox();
                _contentPresenter->disableHoverBox();
                break;

            case ( TaskPurpose::UNKNOWN ):
                break;
        }
    }
}

