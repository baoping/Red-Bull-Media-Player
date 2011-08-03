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
#ifndef MEDIAREPOSITORYTEST_H
#define MEDIAREPOSITORYTEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"


class MediaRepositoryTest : public QObject {
        Q_OBJECT
    public:
        MediaRepositoryTest();
    private slots:

        void saveMedia_GetByGuid();
        void deleteMedia();
        void getAllMedia();
        void getMediaBy_FileName();
        void getMediaBy_MediaType();
        void addTag_getByTag();
        void removeTag_getByTag();
};
DECLARE_TEST( MediaRepositoryTest )
#endif // MEDIAREPOSITORYTEST_H
