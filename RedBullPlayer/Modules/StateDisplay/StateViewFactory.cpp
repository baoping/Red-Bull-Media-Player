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
#include "StateViewFactory.h"

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/UuidCreator.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Modules::TaskManager;

StateViewFactory::StateViewFactory( QObject* parent ) : IStateViewFactory( parent ) {
    _guid = RedBullPlayer::Tools::UuidCreator::create();
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "Create new StateViewFactory: %1" )
                                     .arg( _guid ) );
    _textStyle = 0;
}

void StateViewFactory::setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle ) {
    _textStyle = textStyle;
}

IStateView* StateViewFactory::createViewFor( TaskPurpose::PurposeEnum purpose, QString viewTitle, QWidget* parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "StateViewFactory (%1) create new StateView for purpose: %2" )
                                     .arg( _guid )
                                     .arg( TaskPurpose::purposeString( purpose ) ) );
    HoverBoxUI* box = NULL;

    if ( !_viewRepository.contains( purpose ) ) {
        if ( purpose != TaskPurpose::DISK_SPACE ) {
            box = new HoverBoxUI( Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_CANCEL_ALL_BUTTON,
                                  Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_CANCEL_ALL_BUTTON_PRESSED,
                                  parent );
        } else {
            box = new HoverBoxUI( Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_DISK_SPACE_SHOW_MEDIA,
                                  Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_DISK_SPACE_SHOW_MEDIA_PRESSED,
                                  parent );
        }

        _viewRepository.insert( purpose, box );
    } else {
        box = _viewRepository.value( purpose );
    }

    box->setTitle( viewTitle );

    if ( _textStyle != 0 )
        box->setTextStyle( _textStyle );

    return box;
}
