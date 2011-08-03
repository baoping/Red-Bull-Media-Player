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
#ifndef UPDATEDOWNLOADTASK_H
#define UPDATEDOWNLOADTASK_H

#include <QObject>
#include <QString>
#include <QUrl>

#include "../../Interfaces/TaskManager/ITask.h"
#include "../../ToolLib/TaskManager/Task.h"
#include "../../ToolLib/Network/DownloadUtil.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace PlayerUpdateChecker {
            class UpdateDownloadTask : public RedBullPlayer::Modules::TaskManager::Task {
                    Q_OBJECT
                public:
                    UpdateDownloadTask( QString name, QObject* parent = 0 );
                    virtual ~UpdateDownloadTask();

                    virtual void prepare();
                    virtual void execute();
                    virtual void clean();
                    virtual void cancel();

                    QUrl downloadUrl() const {
                        return _downloadUrl;
                    }
                    void setDownloadUrl( QUrl downloadUrl ) {
                        _downloadUrl = downloadUrl;
                    }

                    QString targetDirectory() const {
                        return _targetDirectory;
                    }
                    void setTargetDirectory( QString targetDirectory ) {
                        _targetDirectory = targetDirectory;
                    }

                    QString localFileName() const {
                        return _localFileName;
                    }
                private: //data
                    RedBullPlayer::Tools::Network::DownloadUtil* _downloader;

                    QUrl _downloadUrl;
                    QString _targetDirectory;
                    QString _localFileName;

                signals:
                    void downloadProgress( QUuid taskId, int progress );

                private slots:
                    void downloadFinished( QString, QString, QString );
                    void downloadFailed( QString, int, QString );
                    void progress( int progress );


            };
        }
    }
}

#endif // UPDATEDOWNLOADTASK_H
