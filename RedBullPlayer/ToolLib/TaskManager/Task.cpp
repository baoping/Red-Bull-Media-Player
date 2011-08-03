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
#include "Task.h"

#include "../../ToolLib/UuidCreator.h"

using namespace RedBullPlayer::Modules::TaskManager;

Task::Task( TaskPurpose::PurposeEnum purpose, QString name, QObject *parent ) : ITask( parent ) {
    _purpose = 0;
    _state = 0;
    _result = 0;
    setPurpose( purpose );
    setState( TaskState::NEW );
    setResult( TaskResult::NOT_EXECUTED );
    _name = name;
    _guid = RedBullPlayer::Tools::UuidCreator::create();
    _canceled = false;
}

void Task::prepare() {
    emit prepared( true );
}

void Task::execute() {
    emit finished( true );
}

void Task::cancel() {
    setState( TaskState::CANCELED );
    setCanceled( true );
    emit canceled();
}

void Task::clean() {
}

TaskInfo Task::taskInfo() {
    TaskInfo taskInfo;
    taskInfo.first = _guid;
    taskInfo.second = _name;
    return taskInfo;
}

void Task::setPurpose( TaskPurpose::PurposeEnum purpose ) {
    if ( _purpose )
        delete _purpose;

    _purpose = new TaskPurpose( purpose );
}

void Task::setState( TaskState::StateEnum state ) {
    if ( _state )
        delete _state;

    _state = new TaskState( state );
}

void Task::setResult( TaskResult::ResultEnum result ) {
    if ( _result )
        delete _result;

    _result = new TaskResult( result );
}
