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
#ifndef TVCHANNEL_H
#define TVCHANNEL_H

#include <QString>
#include <QUrl>
#include <QImage>
#include <QObject>

namespace RedBullPlayer {
    namespace Tools {
        namespace Configuration {
            namespace TV {
                class TVChannel : public QObject {
                        Q_OBJECT

                    public:
                        TVChannel( QObject* parent = 0 ) : QObject( parent ) { }
                        ~TVChannel() {
                            delete _image;
                        }

                        QString name() const {
                            return _name;
                        }
                        void setName( QString name ) {
                            _name = name;
                        }

                        QUrl url() const {
                            return _url;
                        }
                        void setUrl( QUrl url ) {
                            _url = url;
                        }

                        QImage* image() const {
                            return _image;
                        }
                        void setImage( QImage* image ) {
                            _image = image;
                        }

                        QString startAnimationFile() const {
                            return _startAnimationFile;
                        }
                        void setStartAnimationFile( QString fileName ) {
                            _startAnimationFile = fileName;
                        }

                        int animationFrameCount() const {
                            return _animationFrameCount;
                        }
                        void setAnimationFrameCount( int frameCount ) {
                            _animationFrameCount = frameCount;
                        }

                    private:
                        QString _name;
                        QUrl _url;
                        QImage* _image;
                        QString _startAnimationFile;
                        int _animationFrameCount;

                };
            }
        }
    }
}


#endif // TVCHANNEL_H
