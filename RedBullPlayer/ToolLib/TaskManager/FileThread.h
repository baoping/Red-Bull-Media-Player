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
#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>

namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class FileThread : public QThread {
                    Q_OBJECT
                public:
                    FileThread( QString sourceFileName,
                                QString targetFileName,
                                bool deleteAfterCopy,
                                QObject *parent = 0 );
                    void run();
                    void requestCancelation();

                signals:
                    void success();
                    void failed();
                    void canceled();

                private:
                    bool _cancelationRequested;

                    QString _sourceFileName;
                    QString _targetFileName;
                    bool _deleteAfterCopy;

                    QMutex _cancelMutex;

            };
        }
    }
}

#endif // FILETHREAD_H
