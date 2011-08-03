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
#ifndef AUTORESIZEIMAGELABEL_H
#define AUTORESIZEIMAGELABEL_H

#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QMovie>
#include <QResizeEvent>

class AutoResizeImageLabel : public QLabel {
        Q_OBJECT
    public:
        explicit AutoResizeImageLabel( QWidget *parent = 0 );
        virtual ~AutoResizeImageLabel();
        void setImage( QString fileName, int frameCount = 0 );


    public slots:
        void clear();
        void setMovie( QMovie* movie, int frameCount );

    protected:
        virtual void resizeEvent( QResizeEvent * event );

    private:
        void setPixmap_internal( QPixmap pixmap );
        void setMovie_internal( QMovie* movie );
        void refreshSize();

        // DATA
    private:
        QPixmap _pixmap;
        QMovie* _movie;
        bool _nativeUsed;
        bool _displayMovie;
        int _frameCount;

    private slots:
        void animationFrameChanged( int frameNumber );
};

#endif // AUTORESIZEIMAGELABEL_H
