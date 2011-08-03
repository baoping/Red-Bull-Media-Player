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
#ifndef PATHRESOLVERTEST_H
#define PATHRESOLVERTEST_H


#include <QObject>
#include <QtTest/QtTest>

#include "../AutoTest.h"

class PathResolverTest : public QObject {
        Q_OBJECT

    private slots:
        void should_create_resolvable_path_in_baseDir();
        void should_create_resolvable_path_in_subdir_of_baseDir();
        void should_only_build_if_baseDir_and_filePAth_areAbsolute();
        void should_only_build_if_filePath_starts_with_baseDir();

        void should_only_resolve_if_baseDir_isAbsolute();
        void should_only_resolve_if_path_starts_with_placeholder();
        void should_resolve_path();
};


DECLARE_TEST( PathResolverTest )

#endif // PATHRESOLVERTEST_H
