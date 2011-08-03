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
#ifndef SETTINGSVIEWSETUPFILEPARSER_H
#define SETTINGSVIEWSETUPFILEPARSER_H

#include <QFileInfo>
#include <QXmlStreamReader>
#include <QMap>

#include "../../../Interfaces/UserSettings/ISetupFileParser.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace SettingsDetailView {
            class SettingsViewSetupFileParser : public ISetupFileParser {
                    Q_OBJECT

                public:
                    SettingsViewSetupFileParser( QObject *parent );
                    ~SettingsViewSetupFileParser();

                    virtual bool parseSetupFile( QString pathToSetupFile );
                    virtual QString getSettingsType();
                    virtual QList<ISetupSectionObject *> getSections();
                    virtual QList<ISetupSubSectionObject *> getSubSectionsOfSection( QString sectionId );
                    virtual QList<ISetupSettingObject *> getSettingsOfSubSection( QString subSectionId );

                protected:
                    bool setCurrentXmlStreamReader( QString pathToSetupFile );
                    bool parseCurrentSetupFile();
                    void parseSectionElement( QXmlStreamReader *xmlReader );
                    void parseSubSectionElement( QXmlStreamReader *xmlReader, QString sectionId );
                    void parseSettingElement( QXmlStreamReader *xmlReader, QString subSectionId );
                    void addSubSection( QString sectionId, ISetupSubSectionObject *subSection );
                    void addSetting( QString subSectionId, ISetupSettingObject *setting );

                private:
                    QList<ISetupSectionObject *> _sections;
                    QMap<QString, QList<ISetupSubSectionObject *> *> _subsections;
                    QMap<QString, QList<ISetupSettingObject *> *>_settings;
                    QString _type;
                    QXmlStreamReader* _currentSetupFile;
            };
        }
    }
}

#endif // SETTINGSVIEWSETUPFILEPARSER_H
