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
#ifndef ARCHIVDATAREADERTEST_H
#define ARCHIVDATAREADERTEST_H

#include <QObject>
#include <QtTest/QtTest>

#include "../AutoTest.h"


#include "../../../RedBullPlayer/Package/ArchivFileReference.h"
#include "../../../RedBullPlayer/Package/ArchivFile.h"

class ArchivDataReaderTest : public QObject {
        Q_OBJECT

    private:
        QTemporaryFile* buildFakedArchiv( qint64 stubSize, qint64 indexSize,
                                          ArchivFile** data1ArchFile = 0,
                                          ArchivFileReference** data1ArchFileRef = 0,
                                          ArchivFile** data2ArchFile = 0,
                                          ArchivFileReference** data2ArchFileRef = 0,
                                          ArchivFile** data3ArchFile = 0,
                                          ArchivFileReference** data3ArchFileRef = 0 );

    private slots:
        void should_read_file();
        void should_find_section_offsets();
        void should_return_index_data_test();
        void should_return_file_data_test();
        void should_return_exestub_data_test();

};
DECLARE_TEST( ArchivDataReaderTest )
#endif // ARCHIVDATAREADERTEST_H
