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
#ifndef ISTATEVIEW_H
#define ISTATEVIEW_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QWidget>

#include "ITaskView.h"
#include "../StyleFactory/ITextStyle.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class IStateView : public QWidget  {
                    Q_OBJECT

                public:
                    IStateView( QWidget* parent ) : QWidget( parent )  { }
                    virtual ~IStateView() {}

                    virtual QUuid guid() const = 0;

                    virtual void append( ITaskView* taskView ) = 0;
                    virtual void remove( QUuid taskId ) = 0;
                    virtual void activate( QUuid taskId ) = 0;
                    virtual void updateText( QUuid taskId, QString text ) = 0;
                    virtual void updateProgressText( QUuid taskId, QString text ) = 0;

                    virtual void show( int x, int y ) = 0;
                    virtual void hide() = 0;
                    virtual void showCancelAll() = 0;
                    virtual void hideCancelAll() = 0;
                    virtual void setTitle( QString title ) = 0;
                    virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle ) = 0;

                signals:
                    void cancelClicked( QUuid );
                    void cancelAllClicked();

                    void mouseEnter();
                    void mouseLeave();
            };
        }
    }
}

#endif // ISTATEVIEW_H
