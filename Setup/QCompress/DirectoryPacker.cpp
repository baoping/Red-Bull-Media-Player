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
#include "DirectoryPacker.h"

#include <QDebug>
#include <QStringList>

DirectoryPacker::DirectoryPacker( Package* pkg, QFileInfo directory ) {
    Q_ASSERT( pkg != 0 );
    _package = pkg;
    Q_ASSERT( directory.exists() );
    Q_ASSERT( directory.isReadable() );
    Q_ASSERT( directory.isDir() );
    qDebug() << Q_FUNC_INFO << " extractBaseDir: " << directory.path();
    _dirctory = QDir( directory.filePath() );
    qDebug() << Q_FUNC_INFO << " _dirctory: " << _dirctory.path();
}

DirectoryPacker::~DirectoryPacker() {
    //   delete _dirctory;
}


void DirectoryPacker::pack( QString extractBaseDir ) {
    qDebug() << Q_FUNC_INFO << " extractBaseDir: " << extractBaseDir;
    qDebug() << Q_FUNC_INFO << " directory: " << _dirctory.path();
    QStringList filesToPack;
    generateFileList( QFileInfo( _dirctory.absolutePath() ), &filesToPack );
    QDir dir;

    // _archiv->openAppend();
    for ( int i = 0; i < filesToPack.size(); ++i ) {
        QString relPath = dir.relativeFilePath( filesToPack.at( i ) );
        qDebug() << relPath << " extract to: " << QDir::cleanPath( extractBaseDir + "/" + relPath );
        _package->addFile( filesToPack.at( i ), QDir::cleanPath( extractBaseDir + "/" + relPath ) );
        // _archiv->includeFile( filesToPack.at(i), QDir::cleanPath( extractBaseDir + "/" + relPath ) );
    }

    // _archiv->close();
}

void DirectoryPacker::generateFileList( QFileInfo currDir, QStringList* fileList ) {
    Q_ASSERT( currDir.exists() );
    Q_ASSERT( currDir.isReadable() );
    Q_ASSERT( currDir.isDir() );
    qDebug() << Q_FUNC_INFO << " dir: " << currDir.filePath();
    QDir d ( currDir.filePath() );
    d.setFilter( QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot );
    QFileInfoList list = d.entryInfoList();

    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at( i );

        //	qDebug() << QString("File size: %1  name %2").arg(fileInfo.size(), 10)
        //						 .arg(fileInfo.filePath());
        if ( fileInfo.isFile() ) {
            *fileList <<  fileInfo.filePath();
        } else if ( fileInfo.isDir() ) {
            generateFileList( fileInfo, fileList );
        }
    }
}
