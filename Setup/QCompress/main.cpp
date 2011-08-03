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
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <cstring>

#include "Package.h"
#include "DirectoryPacker.h"
using namespace std;

int main( int argc, char** argv ) {
    // If not provided the correct arguments, show usage text
    if( argc < 4 ) {
        cout << "Usage: " << argv[0] <<
             " [ replace fileToReplace newFile extractPath |"
             " include newFile extractPath | "
             " remove fileToRemove | "
             " directory inputDirectory extractPath ] archiveToAddRemoveFileFrom" << endl;
        return 2;
    }

    QString action = argv[1];
    qDebug() << "action: " << action;

    if ( action != "replace" && action != "directory" && action != "include"  && action != "remove" ) {
        qDebug() <<  "ERROR Invalid action: " << action.toLatin1() << endl;
        return 2;
    }

    if ( QString( argv[1] ) == "directory" ) {
        QString inputFileName = argv[2];
        QString extractFileName = argv[3];
        QString archiveFileName = argv[4];
        qDebug()  << "input: " << inputFileName;
        qDebug()  << "extractpath:  " <<  extractFileName;
        qDebug()  << "archive: " <<  archiveFileName;
        Package* p = Package::openCreate( archiveFileName );
        DirectoryPacker dp( p, QFileInfo( inputFileName ) );
        dp.pack( extractFileName );
        p->save( archiveFileName );
        delete p;
    } else if ( QString( argv[1] ) == "replace" ) {
        QString replaceFileName = argv[2];
        QString newFileName = argv[3];
        QString extractPath = argv[4];
        QString archiveFileName = argv[5];
        qDebug()  << "replaceFileName: " << replaceFileName;
        qDebug()  << "newFileName:  " <<  newFileName;
        qDebug()  << "extractPath: " <<  extractPath;
        qDebug()  << "archive: " <<  archiveFileName;
        Package* p = Package::openEdit( archiveFileName );
        p->removeFile( replaceFileName );
        p->addFile( newFileName, extractPath.isNull() ? QString::null : QDir::cleanPath( extractPath + "/" + newFileName ) );
        p->save( archiveFileName );
        delete p;
    } else if ( QString( argv[1] ) == "include" ) {
        QString newFileName = argv[2];
        QString extractPath = argv[3];
        QString archiveFileName = argv[4];
        qDebug()  << "newFileName: " << newFileName;
        qDebug()  << "extractPath:  " <<  extractPath;
        qDebug()  << "archive: " <<  archiveFileName;
        Package* p = Package::openEdit( archiveFileName );
        p->addFile( newFileName, extractPath.isNull() ? QString::null : QDir::cleanPath( extractPath + "/" + newFileName ) );
        p->save( archiveFileName );
        delete p;
    } else if ( QString( argv[1] ) == "remove" ) {
        QString fileToRemove = argv[2];
        QString archiveFileName = argv[3];
        qDebug()  << "fileToRemove: " << fileToRemove;
        qDebug()  << "archive: " <<  archiveFileName;
        Package* p = Package::openEdit( archiveFileName );
        p->removeFile( fileToRemove );
        p->save( archiveFileName );
        delete p;
    } else {
        qFatal( "Invalid pack type" );
    }

    return 0;
}
