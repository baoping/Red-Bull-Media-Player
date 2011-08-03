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
#ifndef STATEVIEWFACTORY_H
#define STATEVIEWFACTORY_H

//QT Includes
#include <QObject>
#include <QList>
#include <QHash>

// Project Includes
#include "../../Interfaces/StateDisplay/IStateViewFactory.h"
#include "HoverBoxUI.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StateDisplay {
            class StateViewFactory : public IStateViewFactory {

                    Q_OBJECT

                public:
                    StateViewFactory( QObject* parent = 0 );

                    virtual QUuid guid() const {
                        return _guid;
                    }

                    virtual void setTextStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle );

                    virtual IStateView* createViewFor( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose, QString viewTitle,  QWidget* parent = 0 );

                private:
                    QUuid _guid;

                    RedBullPlayer::Modules::StyleFactory::ITextStyle* _textStyle;

                    QHash<RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum, HoverBoxUI*> _viewRepository;
            };
        }
    }
}

#endif // STATEVIEWFACTORY_H
