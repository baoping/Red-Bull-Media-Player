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
#ifndef ITASKVIEW_H
#define ITASKVIEW_H


#include <QWidget>
#include <QString>
#include <QUuid>

#include "../StyleFactory/ITextStyle.h"
#include "../TaskManager/Types.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class ITaskView : public QWidget  {
                    Q_OBJECT

                public:
                    ITaskView( QWidget* parent = 0 ) : QWidget( parent ) { }
                    virtual ~ITaskView() {}

                    virtual void activate() = 0;
                    virtual void deactivate() = 0;

                    virtual void showCancelButton() = 0;
                    virtual void hideCancelButton() = 0;

                    virtual void setText( QString text ) = 0;
                    virtual void setProgressDisplayText( QString text ) = 0;

                    virtual void setTaskInfo( RedBullPlayer::Modules::TaskManager::TaskInfo taskInfo ) = 0;
                    virtual RedBullPlayer::Modules::TaskManager::TaskInfo taskInfo() const = 0;
                    virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle ) = 0;

                signals:
                    void cancelClicked( QUuid guid );
            };
        }
    }
}

#endif // ITASKVIEW_H
