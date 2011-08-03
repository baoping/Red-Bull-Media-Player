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
#ifndef ISTATEVIEWFACTORY_H
#define ISTATEVIEWFACTORY_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QWidget>

#include "../TaskManager/ITask.h"
#include "IStateView.h"
#include "../StyleFactory/ITextStyle.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class IStateViewFactory : public QObject  {
                  Q_OBJECT

                public:
                    IStateViewFactory( QObject* parent = 0 ) : QObject( parent ) { }
                    virtual ~IStateViewFactory() {}

                    virtual QUuid guid() const = 0;

                    virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle ) = 0;

                    virtual IStateView* createViewFor( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose,
                                                       QString viewTitle,
                                                       QWidget* parent = 0 ) = 0;
            };
        }
    }
}

#endif // ISTATEVIEWFACTORY_H
