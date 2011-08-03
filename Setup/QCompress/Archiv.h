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
#ifndef ARCHIV_H
#define ARCHIV_H

#include <QFileInfo>

class Archiv {
    public:
        Archiv( QFileInfo archiveFile );
        ~Archiv();
        void includeFile( QFileInfo inputFile, QString extractPath );
        void removeFile( QString extractPath );
        void extractAll( const QString& basePath );
        void openRead();
        void openAppend();
        void close();
        static bool setAllPermissions( QString fileName );

    private:
        QFile* _archiveFile;

};

#endif // ARCHIV_H
