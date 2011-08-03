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
#ifndef ARCHIVFILE_H
#define ARCHIVFILE_H

#include <QString>
#include <QFileInfo>
#include <QDomElement>
#include <QByteArray>

class ArchivFile {

    public:
        static int COMPRESSION_RATE;

        ArchivFile( QFileInfo fileInfo, QString extractPath = QString::null, QString newInternalFileName = QString::null  );
        virtual ~ArchivFile() {}

        qint32 fileSize() const {
            return _fileSize;
        }

        QString fileName() const {
            return _fileName;
        }

        QString extractFileName() const {
            return _extractFileName;
        }

        QFileInfo fileInfo() const {
            return _fileInfo;
        }

        QByteArray data();

        QByteArray compressedData();

        qint32 compressedDataSize();

    protected:
        virtual void readFromQFileInfo( QFileInfo fileInfo, QString newInternalFileName );

    private:
        QString _fileName;
        QString _extractFileName;
        qint32 _fileSize;
        QByteArray _data;
        QByteArray _compressedData;
        QFileInfo _fileInfo;


};

#endif // ARCHIVFILE_H
