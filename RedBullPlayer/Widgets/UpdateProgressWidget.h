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
#ifndef UPDATEPROGRESSWIDGET_H
#define UPDATEPROGRESSWIDGET_H

#include <QtGui/QWidget>

namespace Ui {
    class UpdateProgressWidget;
}

namespace RedBullPlayer {
    namespace Widgets {
        class UpdateProgressWidget : public QWidget {
                Q_OBJECT
            public:
                UpdateProgressWidget( QWidget *parent = 0 );
                ~UpdateProgressWidget();

            signals:
                void okClicked();
                void cancelClicked();

            public slots:
                void doStep( QString task );
                void setSteps( int count );
                void setHeader( QString header );
                void showOkButton();
                void showCancelButton();

            protected:
                void changeEvent( QEvent *e );
                void paintEvent( QPaintEvent *e );

            private:
                Ui::UpdateProgressWidget *m_ui;
                QPixmap _backgroundImage;
                QPixmap _logoImage;
        };
    }
}
#endif // UPDATEPROGRESSWIDGET_H
