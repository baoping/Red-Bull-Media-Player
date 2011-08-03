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
#ifndef STATEDISPLAYBUTTONFACTORY_H
#define STATEDISPLAYBUTTONFACTORY_H

#include <QObject>
#include <QWidget>
#include <QBoxLayout>
#include "../../../Interfaces/Enumerations/TaskPurposeEnum.h"
#include "../../../Widgets/StatusDisplayButton.h"
#include "../../../Interfaces/StyleFactory/IStyleFactory.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace TopUI {
            class StateDisplayButtonFactory : public QObject {
                    Q_OBJECT
                public:
                    StateDisplayButtonFactory( QWidget* buttonParent, RedBullPlayer::Modules::StyleFactory::IStyleFactory *styleFactory, QObject *parent = 0 );

                    RedBullPlayer::Widgets::StatusDisplayButton* createButtonFor( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose );

                protected:
                    RedBullPlayer::Widgets::StatusDisplayButton* createDownloadStatusCountButton( QString icon, QString disabledIcon );

                private:
                    QColor getCountButtonTextColor();
                    QFont getCountButtonFont();

                private:
                    QWidget* _buttonParent;
                    RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;

            };
        }
    }
}

#endif // STATEDISPLAYBUTTONFACTORY_H
