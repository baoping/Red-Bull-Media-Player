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
#ifndef NEWSREPOSITORYTEST_H
#define NEWSREPOSITORYTEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "../AutoTest.h"


class NewsRepositoryTest : public QObject {
        Q_OBJECT
    public:
        NewsRepositoryTest();
    private slots:
        void should_create_news();
        void should_create_news_from_news();
        void should_create_newsImage();
        void should_create_newsImage_from_newsImage();

        void should_create_repository();
        void should_create_news_from_repository();
        void should_create_newsImage_from_repository();

        void should_save_news();
        void should_not_save_news();
        void should_get_news_by_guid();
        void should_get_news_by_external_guid();
        void should_update_news();
        void should_delete_news();
        void should_get_all_news();
        void should_get_all_news_order_by_pubDate();

        void should_have_news_image();

        void should_save_news_image();
        void should_get_news_image_by_fileName();
        void should_update_news_image();
        void should_delete_news_image();
        void should_get_all_news_images();
};
DECLARE_TEST( NewsRepositoryTest )

#endif // NEWSREPOSITORYTEST_H
