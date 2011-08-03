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
#ifndef MIRRORWIDGET_H
#define MIRRORWIDGET_H

// Project Includes
#include "ClickableLabel.h"


namespace RedBullPlayer {
    namespace Widgets {
        class MirrorWidget : public ClickableLabel {
                Q_OBJECT
                Q_PROPERTY( qreal scaleFactor READ scaleFactor WRITE setScaleFactor )

            public:
                enum FadeStyle {
                    NoFade,
                    FadeDown,
                    FadeRight,
                    FadeUp,
                    FadeLeft
                };

                enum MirrorStyle {
                    NoMirror,
                    MirrorOverX,
                    MirrorOverY
                };

            public:
                MirrorWidget( QWidget *parent, const QString &imgUrl, const QString &activeImgUrl, const QString &darkImgUrl, const QString &darkActiveImgUrl
                              , const QString &title, bool hideAtStart, int id = -1 );
                QSize sizeHint() const;
                static QImage mirrorImage( const QImage &img, int alpha, MirrorStyle mirrorStyle = MirrorOverX, FadeStyle fadeStyle = NoFade );

                qreal scaleFactor();
                void setScaleFactor( qreal newValue );

                QString title();

                void scaleToSize( QSize newSize );
                void setActive( bool active );
                bool isActive();

                bool hasMainRegionFocus();
                void setMainRegionFocus( bool hasFocus );
                virtual void setEnabled( bool enabled );

            protected:
                void paintEvent( QPaintEvent *e );

            private:
                QImage _img;
                QImage _activeImg;
                QImage *_currentImg;
                qreal _scaleFactor;
                QString _title;
                QImage _darkActiveImg;
                QImage _darkImg;

                bool _hasMainUIRegionFocus;
                bool _active;
        };
    }
}

#endif // MIRRORWIDGET_H
