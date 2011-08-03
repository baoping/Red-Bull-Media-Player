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
#ifndef FILETASK_H
#define FILETASK_H

#include "Task.h"
#include "FileThread.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TaskManager {
            class FileTask : public Task {
                    Q_OBJECT

                public:
                    FileTask( QString fileName, QString targetDirectory, bool deleteAfterCopy, QObject *parent = 0 );
                    virtual ~FileTask() {}

                    virtual void prepare();
                    virtual void execute();
                    virtual void clean();
                    virtual void cancel();

                    QString newFileName() const {
                        return _newFileName;
                    }

                    QString fileName() const {
                        return _fileName;
                    }

                private:
                    QString _fileName;
                    QString _targetDirectory;
                    QString _newFileName;
                    bool _deleteAfterCopy;

                    FileThread* _fileThread;

                private slots:
                    void thread_success();
                    void thread_failed();
                    void thread_canceled();


            };
        }
    }
}

#endif // FILETASK_H
