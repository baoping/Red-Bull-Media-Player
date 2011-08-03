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
#include "ArchivBuilder.h"

// QT inlcudes
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QUuid>

// Project includes
#include "../ToolLib/UuidCreator.h"
#include "ArchivDataReader.h"

char ArchivBuilder::DATA_MARKER[] = "3607fdf81fb44919342ef62490374a6";
char ArchivBuilder::DATA_MARKER_REPLACECHAR[] = "d";
uint ArchivBuilder::DATA_MARKER_LENGTH = sizeof( DATA_MARKER ) - 1;

ArchivBuilder::ArchivBuilder() {
    qDebug() << Q_FUNC_INFO;
    _data = new ArchivData();
    _index = new ArchivIndex();
    _reader = 0;
}

ArchivBuilder::ArchivBuilder( QFileInfo archivFile ) {
    qDebug() << Q_FUNC_INFO;
    _reader = new ArchivDataReader( archivFile );
    _data = new ArchivData();
    _data->setData( _reader->data() );
    _index = new ArchivIndex( qUncompress( _reader->compressedIndexData() ) );
}

ArchivBuilder::~ArchivBuilder() {
    qDebug() << Q_FUNC_INFO;
    delete _data;
    delete _index;

    if ( _reader ) {
        delete _reader;
    }
}

void ArchivBuilder::addFile( QString fileName, QString extractFileName, QString newInternalFileName ) {
    ArchivFile archFile( QFileInfo( fileName ), extractFileName, newInternalFileName );
    ArchivFileReference* ref = _data->appendFile( &archFile );
    _index->addFileReference( ref );
    delete ref;
}

void ArchivBuilder::removeFile( QString fileName ) {
    ArchivFileReference* ref = _index->getFileReferenceByFileName( fileName );

    if( ref == 0 ) {
        qWarning() << "No file found with name: " << fileName << " in the archiv index";
        return;
    }

    _index->removeFileReference( fileName );
    _data->removeFile( *ref );
    delete ref;
}

QByteArray ArchivBuilder::extractFileData( const ArchivFileReference& fileRef ) {
    if ( _reader == 0 ) {
        qFatal( "Cannot extract file if not running in edir Archiv mode" );
    }

    return _reader->fileData( fileRef );
}


void ArchivBuilder::build( QByteArray stubData, QString fileName ) {
    QTemporaryFile tmpArchiv;
    tmpArchiv.open();
    qDebug() << "Will create temporarty file: " << tmpArchiv.fileName();
    QDataStream ds( &tmpArchiv );
    ds.writeRawData( stubData, stubData.size() );

    // Set dataMarker
    for( uint i = 0; i < ArchivBuilder::DATA_MARKER_LENGTH - 1; i++ ) {
        ds.writeRawData( ArchivBuilder::DATA_MARKER + i , 1 );
    }

    // qDebug() << "Marker size: " << ArchivBuilder::DATA_MARKER_LENGTH;
    ds.writeRawData( ArchivBuilder::DATA_MARKER_REPLACECHAR + 0 , 1 );
    QByteArray indexData = _index->compressedIndexData();
    int indexSize = indexData.size();
    qDebug() << "Index size: " << indexSize;
    QString indexSizeString;
    indexSizeString.setNum( indexSize );
    ds.writeRawData( indexSizeString.toAscii(), indexSizeString.toAscii().size() );
    ds.writeRawData( "\n", 1 );
    // qDebug() << "Wrote size string: " << indexSizeString.toAscii();
    ds.writeRawData( indexData, indexData.size() );
    QByteArray archivData = _data->data();
    ds.writeRawData( archivData, archivData.size() );
    qDebug() << "Wrote data section of size: " <<  archivData.size();

    if ( QFileInfo( fileName ).exists() ) {
        QFile( fileName ).remove();
        // QFile( fileName ).rename( QString( "%1_back_%2" ).arg( fileName ).arg( RedBullPlayer::Tools::UuidCreator::create() ) );
    }

    if(  tmpArchiv.copy( fileName ) ) {
        qDebug() << "Created Archive at: " << QFileInfo( fileName ) .absoluteFilePath();
    } else {
        qWarning() << "Could not copy archiv file to: " <<  QFileInfo( fileName ) .absoluteFilePath();
    }

    qDebug() << "Will create temporarty file: " << tmpArchiv.fileName();
    tmpArchiv.close();
    tmpArchiv.remove();
}

void ArchivBuilder::build( QFileInfo stubFileInfo, QString fileName ) {
    QFile stubFile( stubFileInfo.absoluteFilePath() );

    if ( ! stubFile.open( QIODevice::ReadOnly ) ) {
        qFatal( "Coould not open stub file" );
    }

    stubFile.seek( 0 );
    QByteArray stubData = stubFile.readAll();
    stubFile.close();
    build( stubData, fileName );
}



