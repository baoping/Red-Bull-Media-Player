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
#ifndef REGION_H
#define REGION_H

// QT Includes
#include<QObject>
#include<QString>
#include<QWidget>

namespace RedBullPlayer {
    namespace PlayerShell {
        class Region : public QObject {
                Q_OBJECT
            public:
                Region( QObject * parent, QString name, QWidget* widget );
                ~Region();

                QString name() const {
                    return _name;
                }

                QWidget* widget() const {
                    return _widget;
                }

            private:
                QString _name;
                QWidget* _widget;
        };
    }
}
#endif // REGION_H
