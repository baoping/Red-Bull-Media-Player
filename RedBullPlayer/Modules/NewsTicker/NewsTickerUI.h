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
#ifndef NEWSTICKERUI_H
#define NEWSTICKERUI_H

#include <QtGui/QWidget>
#include <QLabel>
#include <QObject>
#include <QString>
#include <QEvent>
#include <QList>
#include <QPointer>

//Project Includes
#include "../../Events/MainUIRegionFocusChangedEvent.h"
#include "../../Interfaces/INewsTicker.h"
#include "../../Interfaces/IXmlNewsFeed.h"

namespace Ui {
    class NewsTickerUI;
}

class NewsTickerUI : public QWidget {
        Q_OBJECT
    public:
        NewsTickerUI( QWidget *parent = 0 );
        ~NewsTickerUI();

    private:
        Ui::NewsTickerUI *_ui;

};

#endif // NEWSTICKERUI_H
