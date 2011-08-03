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
#ifndef ARGUMENTPARSERTEST_H
#define ARGUMENTPARSERTEST_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <QUuid>

#include "../AutoTest.h"

class ArgumentParserTest : public QObject {
        Q_OBJECT

    private slots:
        void initTestCase();
        void should_create_valid_creationdata();
        void should_set_invalid_xml_error();
        void should_throw_error_on_invalid_command_tag();


    private:
        QUuid requestId1;
        QUuid requestId2;
        QUuid requestId3;
        QString validCommand1;
        QString validCommand2;
        QString validCommand3;

        QString invalid_command_no_xml;
        QString invalid_command_no_snapshot_command;
        QString invalid_command_no_movie_name;
        QString invalid_command_no_position;
        QString invalid_command_wrong_command;
};


DECLARE_TEST( ArgumentParserTest )

#endif // ARGUMENTPARSERTEST_H
