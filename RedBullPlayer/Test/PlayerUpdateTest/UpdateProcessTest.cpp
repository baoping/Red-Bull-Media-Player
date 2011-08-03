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
#include "UpdateProcessTest.h"


// QT includes
#include <QList>


using namespace RedBullPlayer::Updater;

void UpdateProcessTest::should_execute_all_tasks() {
    UpdateProcess* proc = new UpdateProcess( "1.0.0.0", "1.0.0.1", this );
    TestTask* task1 = new TestTask( "Task1" );
    TestTask* task2 = new TestTask( "Task2" );
    TestTask* task3 = new TestTask( "Task3" );
    proc->addTask( task1 );
    proc->addTask( task2 );
    proc->addTask( task3 );
    proc->execute();
    QVERIFY2( task1->executed(), "Should have executed Task" );
    QVERIFY2( task2->executed(), "Should have executed Task" );
    QVERIFY2( task3->executed(), "Should have executed Task" );
}

void UpdateProcessTest::should_initialize_with_empty_task_list() {
    UpdateProcess* proc = new UpdateProcess( "1.0.0.0", "1.0.0.1", this );
    const QList<UpdateTask*>& tasks =  proc->tasks();
    QVERIFY2( tasks.isEmpty(), "Should initialize withj empty task list" );
}
