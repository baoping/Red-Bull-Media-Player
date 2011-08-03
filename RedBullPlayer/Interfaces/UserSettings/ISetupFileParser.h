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
#ifndef ISETTINGSVIEWSETUPFILEPARSER_H
#define ISETTINGSVIEWSETUPFILEPARSER_H

#include <QObject>
#include <QString>
#include <QList>

#include "ISetupSectionObject.h"
#include "ISetupSubSectionObject.h"
#include "ISetupSettingObject.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class ISetupFileParser : public QObject  {
                    Q_OBJECT

                public:
                    ISetupFileParser( QObject* parent = 0 ) : QObject( parent ) { }
                    virtual ~ISetupFileParser() {}
                    virtual bool parseSetupFile( QString pathToSetupFile ) = 0;
                    virtual QString getSettingsType() = 0;
                    virtual QList<RedBullPlayer::Modules::SettingsDetailView::ISetupSectionObject *>
                    getSections() = 0;
                    virtual QList<RedBullPlayer::Modules::SettingsDetailView::ISetupSubSectionObject *>
                    getSubSectionsOfSection( QString sectionId ) = 0;
                    virtual QList<RedBullPlayer::Modules::SettingsDetailView::ISetupSettingObject *>
                    getSettingsOfSubSection( QString subSectionId ) = 0;

            };
        }
    }
}




#endif // ISETTINGSVIEWSETUPFILEPARSER_H
