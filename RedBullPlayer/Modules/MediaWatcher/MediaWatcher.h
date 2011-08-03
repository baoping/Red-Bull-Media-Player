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
#ifndef MEDIAWATCHER_H
#define MEDIAWATCHER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QList>
#include <QHash>
#include <QFileInfoList>

namespace RedBullPlayer {
    namespace Modules {
        namespace MediaWatcher {
            class MediaWatcher : public QObject {
                    Q_OBJECT
                public:
                    MediaWatcher( QString path, QObject *parent = 0 );

                    void startWatching();
                    void stopWatching();

                    bool isWatching() const {
                        return _isWatching;
                    }
                    QString path() const {
                        return _path;
                    }

                    QFileInfoList files() const {
                        return _files;
                    }

                signals:
                    void fileAdded( QString fileName );
                    void fileRemoved( QString fileName );


                private: //data
                    QFileSystemWatcher* _fileSystemWatcher;
                    QString _path;

                    QFileInfoList _files;

                    bool _isWatching;

                private slots:
                    void directoryChanged( const QString& path );
            };
        }
    }
}

#endif // MEDIAWATCHER_H
