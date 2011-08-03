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
#ifndef MEDIADBEXPORT_H
#define MEDIADBEXPORT_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QXmlStreamWriter>

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaLibrary {

            class MediaDBExport : public QObject {
                    Q_OBJECT
                public:
                    MediaDBExport( QObject *parent = 0 );

                    static bool exportToXml( QString fileName );
                private:
                    static void tableDataToXml( QXmlStreamWriter* docStream, QSqlTableModel* tableModel );
                    static void recordDataToXml( QXmlStreamWriter* docStream, QSqlRecord* record );
                    static void fieldDataToXml( QXmlStreamWriter* docStream, QSqlField* field );
            };
        }
    }
}

#endif // MEDIADBEXPORT_H
