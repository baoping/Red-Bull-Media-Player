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
#include "ArchivFile.h"

#include <QDebug>

int ArchivFile::COMPRESSION_RATE = 9;

ArchivFile::ArchivFile( QFileInfo fileInfo,
                        QString extractFileName /* = QString::null */,
                        QString newInternalFileName  /* = QString::null  */ ) {
    Q_ASSERT( fileInfo.exists() );

    if ( extractFileName.isNull() || extractFileName.isEmpty() ) {
        _extractFileName = fileInfo.fileName();
    } else {
        _extractFileName = extractFileName;
    }

    readFromQFileInfo( fileInfo, newInternalFileName );
}


void ArchivFile::readFromQFileInfo( QFileInfo fileInfo, QString newInternalFileName ) {
    _fileInfo = fileInfo;

    if ( newInternalFileName.isNull() ) {
        _fileName = fileInfo.fileName();
    } else {
        _fileName = newInternalFileName;
    }

    _fileSize = fileInfo.size();
}


QByteArray ArchivFile::data() {
    if ( ! _data.isEmpty() ) {
        // qDebug() << "FileData allready read. Size: " << _data.size();
        return _data;
    }

    QFile fread( _fileInfo.absoluteFilePath() );

    if( fread.open( QIODevice::ReadOnly ) ) {
        _data = fread.readAll();
        fread.close();
        qDebug() << "Read file data from: " << _fileInfo.absoluteFilePath() << " Size: " << _data.size();
    } else {
        qFatal( "Could nopt read file" );
    }

    // qDebug() << "Return data: " << _data;
    return _data;
}

QByteArray ArchivFile::compressedData() {
    if ( ! _compressedData.isEmpty() ) {
        // qDebug() << "Compressed file: " << _fileInfo.absoluteFilePath() << " Size: " << _compressedData.size();
        return _compressedData;
    }

    _compressedData = qCompress( data(), COMPRESSION_RATE );
    //   qDebug() << "Compressed file: " << _fileInfo.absoluteFilePath() << " Size: " << _compressedData.size();
    return _compressedData;
}

qint32 ArchivFile::compressedDataSize() {
    return compressedData().size();
}
