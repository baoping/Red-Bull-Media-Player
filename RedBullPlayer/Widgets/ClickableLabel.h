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
#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

namespace RedBullPlayer {
    namespace Widgets {
        class ClickableLabel : public QLabel {
                Q_OBJECT

            public:
                ClickableLabel( QWidget *parent, int id = -1 );
                bool isEnabled();

            public slots:
                virtual void setEnabled( bool enabled );

            signals:
                void clicked();
                void clicked( int id );

            protected:
                virtual void mouseReleaseEvent ( QMouseEvent * event ) ;
                int _id;
        };
    }
}

#endif // CLICKABLELABEL_H
