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
#ifndef UPDATEPROCESSTEST_H
#define UPDATEPROCESSTEST_H

#include <QObject>
#include <QString>
#include <QtTest/QtTest>

#include "../AutoTest.h"

#include "PlayerVersion.h"
#include "UpdateTask.h"
#include "UpdateProcess.h"

class UpdateProcessTest : public QObject {
        Q_OBJECT
    private slots:
        void should_execute_all_tasks();
        void should_initialize_with_empty_task_list();
};

class TestTask : public RedBullPlayer::Updater::UpdateTask {

    public:
        TestTask( QString name ) : RedBullPlayer::Updater::UpdateTask( name ) {
            _executed = false;
        }
        virtual bool doWork() {
            _executed = true;
            return true;
        }

        bool executed() const {
            return _executed;
        }

    private:
        bool _executed;
};

DECLARE_TEST( UpdateProcessTest )

#endif // UPDATEPROCESSTEST_H
