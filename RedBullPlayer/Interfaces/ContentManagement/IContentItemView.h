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
#ifndef ICONTENTITEMVIEW_H
#define ICONTENTITEMVIEW_H

#include <QWidget>
#include <QUuid>
#include <QString>

#include "../StyleFactory/ITextStyle.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace ContentManagement {
            class IContentItemView : public QWidget {
                    Q_OBJECT
                public:
                    IContentItemView( QString guid, QWidget *parent = 0 ) : QWidget( parent ) {
                        _guid = guid;
                        _state = "";
                    }
                    virtual ~IContentItemView() {}

                    virtual QString guid() const {
                        return _guid;
                    }
                    void setState( QString state ) {
                        _state = state;
                    }
                    QString getState() {
                        return _state;
                    }

                    virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textStyle ) = 0;
                    virtual void setTitle( QString title ) = 0;
                    virtual void setSize( QString sizeString ) = 0;
                    virtual void setToolTipText( QString toolTip ) = 0;

                private:
                    QString _guid;
                    QString _state;
            };
        }
    }
}

#endif // ICONTENTITEMVIEW_H
