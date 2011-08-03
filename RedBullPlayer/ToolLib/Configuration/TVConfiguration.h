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
#ifndef TVCONFIGURATION_H
#define TVCONFIGURATION_H

#include <QObject>
#include <QSettings>
#include <QList>
#include <QUrl>
#include <QString>
#include <QImage>

#include "TVChannel.h"

namespace RedBullPlayer {
    namespace Tools {
        namespace Configuration {
            namespace TV {
                class TVConfiguration : public QObject {
                        Q_OBJECT

                    public:
                        TVConfiguration( QSettings* settings, QObject *parent = 0 );
                        ~TVConfiguration();

                        int count();
                        QString nameAt( int index );
                        QUrl urlAt( int index );
                        QImage imageAt( int index );
                        QString startAnimationFileAt( int index );
                        int animationFrameCountAt( int index );

                    private: //data
                        QList<TVChannel*>* _channels;
                    private: //functions
                        bool validIndex( int index );
                };

            } // namespace TV
        } // namespace Configuration
    } // namespace Tools
} // namespace RedBullPlayer

#endif // TVCONFIGURATION_H
