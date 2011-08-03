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
#include "ArchivData.h"

// Qt includes
#include <QDebug>
#include <QDataStream>

ArchivData::ArchivData()  {
    qDebug() << Q_FUNC_INFO;
    _dataFile = new QTemporaryFile();
    _dataFile->open();
    qDebug() << "Created Temp file: " << _dataFile->fileName();
}

ArchivData::~ArchivData() {
    qDebug() << Q_FUNC_INFO;
    qDebug() << "Will delete file: " << _dataFile->fileName();
    _dataFile->close();
    _dataFile->remove();
    _dataFile->deleteLater();
}

ArchivFileReference* ArchivData::appendFile( ArchivFile* file ) {
    //   qDebug() << "Curry file size: " << _dataFile.size();
    qint32 offset = _dataFile->size();
    _dataFile->seek( _dataFile->size() );
    QDataStream ds( _dataFile );
    ds.writeRawData( file->compressedData(), file->compressedDataSize() );
    //  qDebug() << "Creating file ref offset: " << offset << " size: " << file->compressedDataSize();
    return new ArchivFileReference( file->fileName(), file->extractFileName(), offset, file->compressedDataSize(), file->fileSize() );
}

void ArchivData::removeFile( const ArchivFileReference& reference ) {
    qDebug() << "Remove file: " << reference.fileName()
             << "Size: " << reference.fileSize()
             << "At offset: " << reference.offset();
    QTemporaryFile bufferFile;

    if ( ! bufferFile.open() ) {
        qFatal( "Could not open buffer file" );
    }

    qDebug() << "Will create temporarty file: " << bufferFile.fileName();
    _dataFile->seek( 0 );
    QDataStream ds( &bufferFile );
    int bytesBefore = reference.offset();
    //  qDebug() << "Bytes unaffected: " << bytesBefore;
    ds.writeRawData( _dataFile->read( bytesBefore ), bytesBefore );
    int newSeekPos = reference.offset() + reference.fileSize();
    _dataFile->seek( newSeekPos );
    ds.writeRawData( _dataFile->readAll(),  _dataFile->size() - newSeekPos );
    _dataFile->resize( 0 );
    bufferFile.seek( 0 );
    QDataStream dsCopy( _dataFile );
    dsCopy.writeRawData( bufferFile.readAll(), bufferFile.size() );
    qDebug() << "Will delete temporarty file: " << bufferFile.fileName();
    bufferFile.close();
    bufferFile.remove();
    //  qDebug() << "new buffer size: " << bufferFile.size() ;
    //  qDebug() << "new data size: " << _dataFile.size() ;
}

void ArchivData::setData( QByteArray data ) {
    _dataFile->resize( 0 );
    QDataStream ds( _dataFile );
    ds.writeRawData( data, data.size() );
}

QByteArray ArchivData::data() {
    /* qDebug() << "Read file data from: " << _dataFile.fileName()
        << " with size: " << _dataFile.size()
        << " is open: " << _dataFile.isOpen()
        << " is readable: " << _dataFile.isReadable(); */
    QByteArray ret;
    _dataFile->seek( 0 );
    ret = _dataFile->readAll();
    //  qDebug() << "Error: " << _dataFile.errorString() << " FileError: " << _dataFile.error();
    return ret;
}
