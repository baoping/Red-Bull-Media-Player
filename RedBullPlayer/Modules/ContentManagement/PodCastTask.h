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
#ifndef PODCASTTASK_H
#define PODCASTTASK_H

#include <QString>
#include <QUrl>
#include <QPixmap>

#include "../../ToolLib/TaskManager/Task.h"
#include "../../ToolLib/Network/DownloadUtil.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class PodCastTask : public RedBullPlayer::Modules::TaskManager::Task {

                    Q_OBJECT

                public:
                    PodCastTask( QString name, QObject* parent = 0 );
                    virtual ~PodCastTask();

                    virtual void prepare();
                    virtual void execute();
                    virtual void clean();
                    virtual void cancel();

                    QUrl podCastItemUrl() const {
                        return _podCastItemUrl;
                    }

                    void setPodCastItemUrl( QUrl podCastItemUrl ) {
                        _podCastItemUrl = podCastItemUrl;
                    }

                    QUrl podCastImageUrl() const {
                        return _podCastImageUrl;
                    }

                    void setPodCastImageUrl( QUrl podCastImageUrl ) {
                        _podCastImageUrl = podCastImageUrl;
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

                    QPixmap podCastImage() const {
                        return QPixmap::fromImage( _podCastImage );
                    }

                    bool hasImage() const {
                        return !_podCastImage.isNull();
                    }

                private:
                    void downloadImage();
                    void downloadPodCast();

                    QUrl _podCastItemUrl;
                    QUrl _podCastImageUrl;
                    QString _localFileName;
                    QString _targetDirectory;
                    QImage _podCastImage;

                    RedBullPlayer::Tools::Network::DownloadUtil* _imageDownloader;
                    RedBullPlayer::Tools::Network::DownloadUtil* _podCastDownloader;

                private slots:
                    void progress( int progress );

                    void image_downloadFinished( QString, QString, QString );
                    void image_downloadFailed( QString, int, QString );
                    void podCast_downloadFinished( QString, QString, QString );
                    void podCast_downloadFailed( QString, int, QString );
            };
        }
    }
}


#endif // PODCASTTASK_H
