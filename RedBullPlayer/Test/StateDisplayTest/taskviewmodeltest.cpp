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
#include "taskviewmodeltest.h"

#include <QSignalSpy>
#include <QVariant>
#include <QList>
#include <QUuid>
#include <QDebug>

#include "TaskViewModel.h"
#include "mocktask.h"

using namespace RedBullPlayer::Modules::StateDisplay;
using namespace RedBullPlayer::Modules::TaskManager;


TaskViewModelTest::TaskViewModelTest() {
}

void TaskViewModelTest::should_initialize() {
    TaskViewModel* viewModel = new TaskViewModel( DOWNLOAD, this );
    QVERIFY( viewModel->count() == 0 );
    QVERIFY( viewModel->purpose() == DOWNLOAD );
    viewModel->deleteLater();
}

void TaskViewModelTest::should_add_task() {
    TaskViewModel* viewModel = new TaskViewModel( MockTask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskInfo taskInfo;
    taskInfo.first = mockTask->guid();
    taskInfo.second = mockTask->name();
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_add_multiple_tasks() {
    TaskViewModel* viewModel = new TaskViewModel( MockTask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    MockTask* mockTask2 = new MockTask( "MockTask2", QUuid::createUuid() );
    mockTask2->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    TaskDto taskDto2( mockTask2 );
    viewModel->add( taskDto );
    viewModel->add( taskDto2 );
    QVERIFY( viewModel->count() == 2 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_not_add_duplicate_task() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    viewModel->add( taskDto );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_not_add_task_wrong_purpose() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::IO, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 0 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_not_add_task_wrong_task_purpose() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::IO );
    TaskDto taskDto( mockTask );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 0 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_emit_task_added() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    qRegisterMetaType<QUuid>( "QUuid" );
    QSignalSpy spy( viewModel, SIGNAL( taskAdded( QUuid ) ) );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    QCOMPARE( spy.count(), 1 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_not_emit_task_added() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::IO, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    qRegisterMetaType<QUuid>( "QUuid" );
    QSignalSpy spy( viewModel, SIGNAL( taskAdded( QUuid ) ) );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 0 );
    QCOMPARE( spy.count(), 0 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_get_task() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    bool ok;
    TaskDto result = viewModel->get( taskDto.guid(), &ok );
    QVERIFY( ok );
    QCOMPARE( result.guid(), taskDto.guid() );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_remove_task() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    viewModel->remove( taskDto.guid() );
    QVERIFY( viewModel->count() == 0 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_not_remove_task() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    viewModel->remove( QUuid::createUuid() );
    QVERIFY( viewModel->count() == 1 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_emit_task_removed() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    qRegisterMetaType<QUuid>( "QUuid" );
    QSignalSpy spy( viewModel, SIGNAL( taskRemoved( QUuid ) ) );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    viewModel->remove( taskDto.guid() );
    QVERIFY( viewModel->count() == 0 );
    QCOMPARE( spy.count(), 1 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_not_emit_task_removed() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    qRegisterMetaType<QUuid>( "QUuid" );
    QSignalSpy spy( viewModel, SIGNAL( taskRemoved( QUuid ) ) );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    viewModel->remove( QUuid::createUuid() );
    QVERIFY( viewModel->count() == 1 );
    QCOMPARE( spy.count(), 0 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_emit_task_started() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    qRegisterMetaType<QUuid>( "QUuid" );
    QSignalSpy spy( viewModel, SIGNAL( taskStarted( QUuid ) ) );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    viewModel->setStarted( taskDto.guid() );
    QCOMPARE( spy.count(), 1 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}

void TaskViewModelTest::should_not_emit_task_started() {
    TaskViewModel* viewModel = new TaskViewModel( ITask::DOWNLOAD, this );
    MockTask* mockTask = new MockTask( "MockTask", QUuid::createUuid() );
    mockTask->setPurpose( MockTask::DOWNLOAD );
    TaskDto taskDto( mockTask );
    qRegisterMetaType<QUuid>( "QUuid" );
    QSignalSpy spy( viewModel, SIGNAL( taskStarted( QUuid ) ) );
    viewModel->add( taskDto );
    QVERIFY( viewModel->count() == 1 );
    viewModel->setStarted( QUuid::createUuid() );
    QCOMPARE( spy.count(), 0 );
    mockTask->deleteLater();
    viewModel->deleteLater();
}
