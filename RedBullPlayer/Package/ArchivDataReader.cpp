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
#include "ArchivDataReader.h"

// QT includes
#include <QDebug>

// Project includes
#include "ArchivBuilder.h"

ArchivDataReader::ArchivDataReader( QFileInfo archiveFileInfo ) {
    _archiveFileInfo = archiveFileInfo;
    _dataSeciotnOffset = 0;
    _indexSectionOffset = 0;
    _exeStubSize = 0;
}


void ArchivDataReader::findSectionOffsets() {
    QFile fread( _archiveFileInfo.absoluteFilePath() );

    if ( ! fread.open( QIODevice::ReadOnly ) ) {
        qFatal( "Could not open file for read" );
    }

    char c;
    uint state = 0;

    // Since the magic string can't appear directly in the binary,
    // undo the sanitizing tweak made to the constant above.
    if ( ArchivBuilder::DATA_MARKER[ArchivBuilder::DATA_MARKER_LENGTH - 1] != ArchivBuilder::DATA_MARKER_REPLACECHAR[0] ) {
        ArchivBuilder::DATA_MARKER[ArchivBuilder::DATA_MARKER_LENGTH - 1] = ArchivBuilder::DATA_MARKER_REPLACECHAR[0];
    }

    // Ignore the stub by finding the first instance of the magic string
    while( fread.getChar( &c ) ) {
        if( c == ArchivBuilder::DATA_MARKER[state] ) {
            state++;
        } else {
            state = 0;
        }

        if( state == ArchivBuilder::DATA_MARKER_LENGTH ) {
            break;
        }
    }

    // If the magic string wasn't found, exit with error status
    if( state != ArchivBuilder::DATA_MARKER_LENGTH ) {
        qFatal( "Magic string not found" );
    }

    // qDebug() << "Found end of exestub incl. marker at: " << fread.pos();
    // QByteArray indexSizeString = fread.read();
    _exeStubSize = fread.pos() - ArchivBuilder::DATA_MARKER_LENGTH;
    QByteArray d = fread.readLine();
    //  qDebug() << "Found index size data: " << d.data();
    QString s = d.data();
    s = s.simplified();
    //  qDebug() << "Found index size string: " << s;
    bool ok;
    int indexSize = s.toInt( &ok, 10 );
    //  qDebug() << "Found index size " << indexSize;
    _indexSectionOffset = fread.pos();
    _dataSeciotnOffset = _indexSectionOffset + indexSize;
    //  qDebug() << "index offset: " << _indexSectionOffset;
    //  qDebug() << "data offset: " << _dataSeciotnOffset;
    fread.close();
    //  qDebug() << Q_FUNC_INFO << " done";
}

qint64 ArchivDataReader::dataSeciotnOffset() {
    return _dataSeciotnOffset;
}
qint64 ArchivDataReader::indexSectionOffset() {
    return _indexSectionOffset;
}

QByteArray ArchivDataReader::compressedIndexData() {
    if ( _dataSeciotnOffset == 0 ) {
        findSectionOffsets();
    }

    QFile fread( _archiveFileInfo.absoluteFilePath() );

    if ( ! fread.open( QIODevice::ReadOnly ) ) {
        qFatal( "Could not open file for read" );
    }

    fread.seek( _indexSectionOffset );
    QByteArray ret = fread.read( _dataSeciotnOffset - _indexSectionOffset );
    fread.close();
    return ret;
}

QByteArray ArchivDataReader::data() {
    if ( _dataSeciotnOffset == 0 ) {
        findSectionOffsets();
    }

    QFile fread( _archiveFileInfo.absoluteFilePath() );

    if ( ! fread.open( QIODevice::ReadOnly ) ) {
        qFatal( "Could not open file for read" );
    }

    fread.seek( _dataSeciotnOffset  );
    QByteArray ret = fread.readAll();
    fread.close();
    return ret;
}

QByteArray ArchivDataReader::compressedFileData( const ArchivFileReference& fileRef ) {
    if ( _dataSeciotnOffset == 0 ) {
        findSectionOffsets();
    }

    QFile fread( _archiveFileInfo.absoluteFilePath() );

    if ( ! fread.open( QIODevice::ReadOnly ) ) {
        qFatal( "Could not open file for read" );
        return QByteArray();
    }

    //  qDebug() << "Read file at " << _dataSeciotnOffset + fileRef.offset() << " size: " << fileRef.fileSize() ;
    fread.seek( _dataSeciotnOffset + fileRef.offset() );
    QByteArray ret = fread.read( fileRef.fileSize() );
    fread.close();
    return ret;
}

QByteArray ArchivDataReader::exeStubData() {
    if ( _dataSeciotnOffset == 0 ) {
        findSectionOffsets();
    }

    QFile fread( _archiveFileInfo.absoluteFilePath() );

    if ( ! fread.open( QIODevice::ReadOnly ) ) {
        qFatal( "Could not open file for read" );
        return QByteArray();
    }

    fread.seek( 0 );
    QByteArray ret = fread.read( _exeStubSize );
    fread.close();
    return ret;
}

QByteArray ArchivDataReader::fileData( const ArchivFileReference& fileRef ) {
    return qUncompress( compressedFileData( fileRef ) );
}
