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
#include "Package.h"

#include <QDir>
#include <QDebug>
#include <QString>

Package::Package() : QObject( 0 ) {}
Package::~Package() {
    delete _archivBuilder;
}

Package* Package::openCreate( QFileInfo exeStub ) {
    if ( ! exeStub.exists() ) {
        qFatal( QString( "Stub file: %1 does not exist" ).arg( exeStub.absoluteFilePath() ).toAscii() );
    }

    if ( ! exeStub.isReadable() ) {
        qFatal( QString( "Stub file: %1 is  not readable" ).arg( exeStub.absoluteFilePath() ).toAscii() );
    }

    Package* p = new Package();
    p->_exeStub = exeStub;
    p->_archivBuilder = new ArchivBuilder();
    p->_openMode = Package::Create;
    return p;
}

Package* Package::openEdit( QFileInfo existingPackage ) {
    Package* p = new Package();
    p->_existingPackage = existingPackage;
    p->_archivBuilder = new ArchivBuilder( existingPackage );
    p->_openMode = Package::Edit;
    return p;
}

void Package::addFile( QString fileName, QString extractFileName, QString newInternalFileName /* = QString::null */ ) {
    _archivBuilder->addFile( fileName, extractFileName, newInternalFileName );
}

void Package::removeFile( QString fileName ) {
    _archivBuilder->removeFile( fileName );
}

void Package::extractAll( QString basePath ) {
    if ( _openMode != Edit ) {
        qFatal( "Cannot extract ind create mode" );
    }

    const ArchivIndex& index = _archivBuilder->index();
    emit startExtracting( index.fileCount() );
    QList<ArchivFileReference*>* references = index.fileReferences();
    ArchivFileReference* currRef;
    foreach( currRef, *references ) {
        emit checkingFile( currRef->fileName() );
        QString extrFn =  currRef->extractFileName();

        if ( ! basePath.isNull() ) {
            extrFn = QDir::cleanPath( basePath + "/" + extrFn );
        }

        QFileInfo extractToFileInfo( extrFn );

        if ( extractToFileInfo.exists() ) {
            emit skippingFile( currRef->fileName() );
            qDebug() << "Skip File: " << extractToFileInfo.absoluteFilePath() << " as it allready exists";
            delete currRef;
            continue;
        } else {
            emit extractingFile( currRef->fileName() );
            QByteArray data = _archivBuilder->extractFileData( *currRef );
            writeFileTo( data, extrFn );
        }
    }
    emit finishExtracting();
}

void Package::extractFile( QString fileName, QString toFile ) {
    if ( _openMode != Edit ) {
        qFatal( "Cannot extract ind create mode" );
    }

    const ArchivIndex& index = _archivBuilder->index();
    ArchivFileReference* ref = index.getFileReferenceByFileName( fileName );
    emit startExtracting( 1 );

    if( ref == 0 ) {
        qWarning() << "No file found with name: " << fileName << " in the archiv index";
        return;
    }

    QFileInfo extractToFileInfo( toFile );

    if ( extractToFileInfo.exists() ) {
        qWarning() << "Cannot extract to existing file: " << toFile;
        qFatal( "Cannot extract file exists" );
    }

    emit extractingFile( ref->fileName() );
    QByteArray data = _archivBuilder->extractFileData( *ref );
    writeFileTo( data, toFile );
    emit finishExtracting();
}


int Package::requiredDiskSpace() {
    if ( _openMode != Edit ) {
        qFatal( "Cannot extract ind create mode" );
    }

    return _archivBuilder->index().extracedContentSize();
}

void Package::writeFileTo( QByteArray data, QFileInfo extractFileInfo ) {
    enshurePathForFile( extractFileInfo );
    QFile newFile( extractFileInfo.absoluteFilePath() );

    if ( ! newFile.open( QIODevice::WriteOnly )  ) {
        qFatal( QString( "Could not create file: %1" ).arg( extractFileInfo.absoluteFilePath() ).toAscii() );
    }

    qint64 writtenBytes = newFile.write( data );
    newFile.close();

    if ( writtenBytes != data.size() ) {
        qWarning() << "Error writning file: " << extractFileInfo.absoluteFilePath()  <<  "Only wrote " << writtenBytes << " out of " << data.size();
        return;
    }

    qDebug() << "Wrote file: " << extractFileInfo.absoluteFilePath() << " with size: " << writtenBytes;
}

void Package::save( QString fileName ) {
    QByteArray stubData;

    if ( _openMode == Edit ) {
        stubData = _archivBuilder->reader()->exeStubData();
        _archivBuilder->build( stubData, fileName );
    } else {
        _archivBuilder->build( _exeStub, fileName );
    }
}

void Package::enshurePathForFile( QFileInfo fileName ) {
    QDir dir( fileName.dir() );

    if ( !fileName.exists() ) {
        qDebug() << "Create dir : " << fileName.absolutePath();
        dir.mkpath( fileName.absolutePath() );
    }
}
