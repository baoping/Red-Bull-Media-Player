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
#include "TaskViewFactory.h"

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../ToolLib/UuidCreator.h"
#include "TaskViewUI.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Modules::TaskManager;

TaskViewFactory::TaskViewFactory( QObject* parent ) : ITaskViewFactory( parent ) {
    _guid = RedBullPlayer::Tools::UuidCreator::create();
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "Create new TaskViewFactory: %1" )
                                     .arg( _guid ) );
    _textStyle = 0;
}

void TaskViewFactory::setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle ) {
    _textStyle = textStyle;
}

ITaskView* TaskViewFactory::createViewFor( TaskInfo taskInfo, QWidget* parent ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_STATEDISPLAY,
                                     QString( "TaskViewFactory (%1) create new TaskView." )
                                     .arg( _guid ) );
    TaskViewUI* taskView = new TaskViewUI( parent );
    taskView->setTaskInfo( taskInfo );

    if ( _textStyle != 0 )
        taskView->setTextStyle( _textStyle );

    return taskView;
}
