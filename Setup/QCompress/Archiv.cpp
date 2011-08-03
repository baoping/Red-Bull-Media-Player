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
#include "Archiv.h"

#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QWaitCondition>
#include <QThread>
#include <iostream>

class SleeperThread : public QThread {
    public:
        static  void msleep( unsigned long msecs ) {
            QThread::msleep( msecs );
        }
};

using namespace std;

char dataMarker[] = "QCompressec";
char dataMarkerReplaceChar[] = "d";

uint dataMarkerLength = sizeof( dataMarker ) - 1;
int compressRate = 1;

Archiv::Archiv( QFileInfo archiveFile ) {
    Q_ASSERT( archiveFile.exists() );
    Q_ASSERT( archiveFile.isWritable() );
    Q_ASSERT( archiveFile.isFile() );
    _archiveFile = new QFile( archiveFile.absoluteFilePath() );
}

Archiv::~Archiv() {
    close();
    delete _archiveFile;
}

void Archiv::openRead() {
    if ( _archiveFile->isOpen()  ) {
        if ( ! _archiveFile->isReadable() ) {
            qFatal( "Archive open but not readable!" );
        }

        return;
    }

    int tries = 0;
    bool worked = _archiveFile->open( QIODevice::ReadOnly );

    while ( ! worked && tries < 10 ) {
        qDebug() << _archiveFile->errorString();
        qDebug() << "Could not open Archive file! Wait 1 sec...";
        SleeperThread::msleep( 1000 );
        worked = _archiveFile->open( QIODevice::ReadOnly );
        tries++;
    }

    if ( tries == 10 && ! _archiveFile->isOpen() ) {
        qFatal( "Could not open Archive File within 10 seconds!" );
    }
}
void Archiv::openAppend() {
    if ( _archiveFile->isOpen() ) {
        if( ! _archiveFile->openMode().testFlag( QIODevice::Append ) ) {
            qFatal( "Archive allready open but not in append mode!" );
        }

        return;
    }

    int tries = 0;
    bool worked = _archiveFile->open( QIODevice::Append );

    while ( ! worked  && tries < 10 ) {
        qDebug() << _archiveFile->errorString();
        qDebug() << "Could not open Archive file! Wait 1 sec...";
        SleeperThread::msleep( 1000 );
        worked = _archiveFile->open( QIODevice::Append );
        tries++;
    }

    if ( tries == 10 && ! _archiveFile->isOpen() ) {
        qFatal( "Could not open Archive File within 10 seconds!" );
    }
}
void Archiv::close() {
    if ( ! _archiveFile->isOpen() ) {
        return;
    }

    _archiveFile->flush();
    _archiveFile->close();
}


void Archiv::includeFile( QFileInfo inputFileInfo, QString extractPath ) {
    qDebug() << Q_FUNC_INFO;
    Q_ASSERT( inputFileInfo.exists() );
    Q_ASSERT( inputFileInfo.isReadable() );
    QFile inputFile( inputFileInfo.absoluteFilePath() );

    if ( ! inputFile.open( QIODevice::ReadOnly ) ) {
        qFatal( "Could Not open input file!" );
    }

    openAppend();
    // Read and compress input file
    QByteArray ba = qCompress( inputFile.readAll(), compressRate );
    // qDebug() << "Apend File: " << inputFile.fileName() << " Extract Path: " << extractPath;
    int status = -1;

    // Write header metadata and concatenate file
    // Magic string MUST NOT occour in the binary!
    // write char by char.
    for( uint i = 0; i < dataMarkerLength - 1; i++ ) {
        status = _archiveFile->write( dataMarker + i , 1 );
        Q_ASSERT( status != -1 );
    }

    status = _archiveFile->write( dataMarkerReplaceChar + 0 , 1 );
    Q_ASSERT( status != -1 );
    status = _archiveFile->write( inputFile.fileName().toAscii(), qstrlen( inputFile.fileName().toAscii() ) ); // OriginalFilename
    Q_ASSERT( status != -1 );
    status = _archiveFile->write( "\n", 1 );                 // Terminator
    Q_ASSERT( status != -1 );
    status = _archiveFile->write( extractPath.toAscii(), qstrlen( extractPath.toAscii() ) ); // Extract FileName
    Q_ASSERT( status != -1 );
    status = _archiveFile->write( "\n", 1 );                 // Terminator
    Q_ASSERT( status != -1 );
    int fileLength = ba.length();
    status = _archiveFile->write( ( char* )( &fileLength ), sizeof( int ) ); // Compressed length
    Q_ASSERT( status != -1 );
    status = _archiveFile->write( ba );                      // Compressed data
    Q_ASSERT( status != -1 );
    // Close files and exit cleanly
    qDebug() << "Successfully appendend file: " << inputFile.fileName().toLatin1() << " as " << extractPath.toLatin1();
    inputFile.close();
    _archiveFile->flush();
}

void Archiv::removeFile( QString extractPath ) {
    qDebug() << Q_FUNC_INFO;
    openRead();
    QByteArray newFile;
    char c;
    char state = 0;
    int fileLength = 0;

    // Since the magic string can't appear directly in the binary,
    // undo the sanitizing tweak made to the constant above.
    if ( dataMarker[dataMarkerLength - 1] != dataMarkerReplaceChar[0] ) {
        // qDebug() << "Setup magic string";
        dataMarker[dataMarkerLength - 1] = dataMarkerReplaceChar[0];
    }

    // Ignore the stub by finding the first instance of the magic string
    while( _archiveFile->getChar( &c ) ) {
        if( c == dataMarker[state] ) {
            state++;
        } else {
            state = 0;
        }

        newFile.append( c );

        if( state == dataMarkerLength ) {
            break;
        }
    }

    // If the magic string wasn't found, exit with error status
    if( state != dataMarkerLength ) {
        cerr << "Magic string not found" << endl;
        return;
    }

    bool fileChanged = false;
    //        qDebug() << "End Magic File at: " << _archiveFile->pos();
    _archiveFile->seek( _archiveFile->pos() - dataMarkerLength );
    bool firstRead = true;
    bool firstWrite = true;

    while( !_archiveFile->atEnd() ) {
        // Consume the magic string. If this is the first file,
        // skip this step because we consumed it earlier.
        // if( ! first ) {
        _archiveFile->read( dataMarkerLength );
        // }
        //   qDebug() << "End Magic for file  at: " << _archiveFile->pos();
        // Read the filename and file length
        QByteArray filename = _archiveFile->readLine();
        filename.chop( 1 );
        // Read Extract filename
        QByteArray extractFilename = _archiveFile->readLine();
        extractFilename.chop( 1 );
        _archiveFile->read( ( char* )( &fileLength ), sizeof( int ) );

        // Indicate activity to user
        if ( QString( extractFilename.data() ) == extractPath ) {
            cout << endl << "Will Remove File: " << filename.data() << " that would will be extracted to: " << extractFilename.data() << endl << endl;
            _archiveFile->seek( _archiveFile->pos() + fileLength );
            fileChanged = true;
        } else {
            // cout << "Not touching file: " << filename.data() << " that would will be extracted to: " << extractFilename.data() << endl;
            if ( ! firstWrite ) {
                for( uint i = 0; i < dataMarkerLength - 1; i++ ) {
                    newFile.append( dataMarker[i] );
                }

                newFile.append( dataMarkerReplaceChar[0] );
            }

            newFile.append( filename );
            newFile.append( "\n" );
            newFile.append( extractFilename );
            newFile.append( "\n" );
            newFile.append( ( char* )( &fileLength ), sizeof( int ) );
            newFile.append( _archiveFile->read( fileLength ) );
            firstWrite = false;
        }

        //                if( first ) {
        //                    first = false;
        //                }
        firstRead = false;
    }

    close();

    if ( fileChanged ) {
        QFileInfo archFileInfo( *_archiveFile );
        QString completeFileName = archFileInfo.absoluteFilePath();
        QString path = archFileInfo.absolutePath();
        QString baseName = archFileInfo.baseName();
        int i = 0;
        QString newName = QString( "%1/%2_%3.exe" ).arg( path ).arg( baseName ).arg( i );

        while( QFileInfo( newName ).exists() ) {
            i++;
            newName = QString( "%1/%2_%3.exe" ).arg( path ).arg( baseName ).arg( i );
        }

        qDebug() << "Backup Original as: " << newName;
        _archiveFile->rename( newName );
        QFile newFileToCreate( completeFileName );

        if ( newFileToCreate.open( QIODevice::ReadWrite ) ) {
            newFileToCreate.write( newFile );
            newFileToCreate.close();
        } else {
            qDebug() << "Could not write new archive: " << completeFileName;
            qFatal( "Error Creating backup archiv!" );
        }

        _archiveFile = new QFile( completeFileName );
    }
}

void Archiv::extractAll( const QString& basePath = NULL ) {
    qDebug() << Q_FUNC_INFO;
    char c;
    char state = 0;
    int l = 0;
    bool first = 0;
    qDebug() << "Orig Magic String: " << dataMarker;

    // Since the magic string can't appear directly in the binary,
    // undo the sanitizing tweak made to the constant above.
    if ( dataMarker[dataMarkerLength - 1] != dataMarkerReplaceChar[0] ) {
        dataMarker[dataMarkerLength - 1] = dataMarkerReplaceChar[0];
    }

    qDebug() << "Real Magic String: " << dataMarker;
    // Open the current executable as a data file
    openRead();

    // Ignore the stub by finding the first instance of the magic string
    while( _archiveFile->getChar( &c ) ) {
        if( c == dataMarker[state] ) {
            state++;
        } else {
            state = 0;
        }

        if( state == dataMarkerLength ) {
            break;
        }
    }

    // If the magic string wasn't found, exit with error status
    if( state != dataMarkerLength ) {
        cerr << "Magic string not found" << endl;
        return;
    }

    // Loop through the rest of the file
    while( !_archiveFile->atEnd() ) {
        // Consume the magic string. If this is the first file,
        // skip this step because we consumed it earlier.
        if( first ) {
            _archiveFile->read( dataMarkerLength );
        } else {
            first = 1;
        }

        // Read the filename and file length
        QByteArray filename = _archiveFile->readLine();
        filename.chop( 1 );
        // Read Extract filename
        QByteArray extractFilename = _archiveFile->readLine();
        extractFilename.chop( 1 );
        _archiveFile->read( ( char* )( &l ), sizeof( int ) );
        // Indicate activity to user
        cout << "Filename: " << filename.data() << " will be extracted to: " << extractFilename.data() << endl;
        QString extrFn = extractFilename.data();

        if ( ! basePath.isNull() ) {
            extrFn = QDir::cleanPath( basePath + "/" + extrFn );
        }

        // Open output file and write decompressed data
        QFile outFile( *( new QString( extrFn ) ) );
        QFileInfo inf( outFile );
        QDir dir( inf.dir() );

        if ( !dir.exists() ) {
            qDebug() << "Create dir : " << inf.absolutePath();
            dir.mkpath( inf.absolutePath() );
        }

        if ( inf.exists() ) {
            qDebug() << "Skip File: " << inf.absoluteFilePath() << " already exists.";
            // seek to set to vaild position
            _archiveFile->seek( _archiveFile->pos() + l );
            // f.read(l);
        } else {
            qDebug() << "Extracting File: " << inf.absoluteFilePath();
            outFile.open( QIODevice::WriteOnly );
            outFile.write( qUncompress( _archiveFile->read( l ) ) );
            outFile.flush();
            outFile.close();
        }
    }

    close();
}

bool Archiv::setAllPermissions( QString fileName ) {
    QFile f( fileName );
    QFlags<QFile::Permission> newPerms;
    newPerms = QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
               QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup |
               QFile::ReadUser | QFile::WriteUser | QFile::ExeUser |
               QFile::ReadOther | QFile::WriteOther | QFile::ExeOther;
    bool retVal = f.setPermissions( newPerms );
    qDebug() << "setting permissions for" << fileName << "retVal = " << retVal;
    f.close();
    return retVal;
}
