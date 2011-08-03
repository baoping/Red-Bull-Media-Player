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
#ifndef CIRCLEPROGRESS_H
#define CIRCLEPROGRESS_H

#include <QLabel>
#include <QPixmap>

namespace RedBullPlayer {
    namespace Widgets {
        class CircleProgress : public QLabel {
                Q_OBJECT

            public:
                CircleProgress( QWidget* parent,
                                QPixmap* pointImage );
                ~CircleProgress();

                virtual QSize sizeHint() const;

                double minimum() const {
                    return _min;
                }

                double maximum() const {
                    return _max;
                }

                double position() const {
                    return _pos;
                }

                double percent() const {
                    return _percent;
                }

                double step() const {
                    return _step;
                }

                QString text() const {
                    return _text;
                }

                void setMinimum( double minimum ) {
                    _min = minimum;
                }

                void setMaximum( double maximum ) {
                    _max = maximum;
                }

                bool setPosition( double position );
                bool setPercent( int percent );
                void setText( QString text );
                void setStep( double step ) {
                    _step = step;
                }

                void doProgress( double position, QString text = "" );
                bool doStep();

            protected:
                virtual void paintEvent( QPaintEvent *event );

            private:
                void updateCircleCount();

                QPixmap* _pointImage;
                double _min;
                double _max;
                double _pos;
                int _percent;
                double _step;
                int _circleCount;
                QString _text;

        };
    }
}

#endif // CIRCLEPROGRESS_H
