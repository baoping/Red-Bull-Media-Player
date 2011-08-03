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
#include "SettingsViewSetupFileParser.h"

#include <QFile>

#include "SectionSetupObject.h"
#include "SettingSetupObject.h"
#include "SubSectionSetupObject.h"

using namespace RedBullPlayer::Modules::SettingsDetailView;

SettingsViewSetupFileParser::SettingsViewSetupFileParser( QObject *parent ) : ISetupFileParser( parent ) {
}

SettingsViewSetupFileParser::~SettingsViewSetupFileParser() {
}

bool SettingsViewSetupFileParser::parseSetupFile( QString pathToSetupFile ) {
    _sections = QList<ISetupSectionObject *>();
    _subsections = QMap<QString, QList<ISetupSubSectionObject *> *>();
    _settings = QMap<QString, QList<ISetupSettingObject *> *>();
    _type = "";

    if( setCurrentXmlStreamReader( pathToSetupFile ) )
        return parseCurrentSetupFile();
    else
        return false;
}

bool SettingsViewSetupFileParser::setCurrentXmlStreamReader( QString pathToSetupFile ) {
    QFileInfo setupFileInfo = QFileInfo( pathToSetupFile );

    if ( !QFile::exists( setupFileInfo.absoluteFilePath() ) )
        return false;

    QFile file( setupFileInfo.absoluteFilePath() );

    if ( !file.open( QFile::ReadOnly ) )
        return false;

    _currentSetupFile = new QXmlStreamReader( file.readAll() );
    file.close();
    return true;
}

bool SettingsViewSetupFileParser::parseCurrentSetupFile() {
    while( !_currentSetupFile->isEndDocument() && !_currentSetupFile->hasError() ) {
        if( _currentSetupFile->name() == "Settings" && _currentSetupFile->isStartElement() )
            _type = _currentSetupFile->attributes().value( "type" ).toString();

        if( _currentSetupFile->name() == "Section" && _currentSetupFile->isStartElement() )
            parseSectionElement( _currentSetupFile );

        _currentSetupFile->readNext();
    }

    if( _currentSetupFile->hasError() )
        return false;
    else
        return true;
}

void SettingsViewSetupFileParser::parseSectionElement( QXmlStreamReader *xmlReader ) {
    SectionSetupObject *section = new SectionSetupObject( xmlReader->attributes().value( "id" ).toString(),
            xmlReader->attributes().value( "title" ).toString(),
            xmlReader->attributes().value( "help" ).toString(),
            this );
    QString sectionId = xmlReader->attributes().value( "id" ).toString();
    _sections.append( section );

    do {
        xmlReader->readNext();

        if( xmlReader->name() == "SubSection" && !xmlReader->isEndElement() )
            parseSubSectionElement( xmlReader, sectionId );
    } while( xmlReader->name() != "Section" );
}

void SettingsViewSetupFileParser::parseSubSectionElement( QXmlStreamReader *xmlReader, QString sectionId ) {
    SubSectionSetupObject *subSection = new SubSectionSetupObject( xmlReader->attributes().value( "id" ).toString(),
            xmlReader->attributes().value( "title" ).toString(),
            this );
    addSubSection( sectionId, subSection );
    QString subSectionId = xmlReader->attributes().value( "id" ).toString();

    do {
        if( xmlReader->name() == "Setting" && !xmlReader->isEndElement() )
            parseSettingElement( xmlReader, subSectionId );

        xmlReader->readNext();
    } while( xmlReader->name() != "SubSection" );
}

void SettingsViewSetupFileParser::parseSettingElement( QXmlStreamReader *xmlReader, QString subSectionId ) {
    SettingSetupObject *setting = new SettingSetupObject( xmlReader->attributes().value( "id" ).toString(),
            xmlReader->attributes().value( "type" ).toString(),
            xmlReader->attributes().value( "label" ).toString(),
            this );
    addSetting( subSectionId, setting );
}

void SettingsViewSetupFileParser::addSubSection( QString sectionId, ISetupSubSectionObject *subSection ) {
    if( !_subsections.contains( sectionId ) ) {
        QList<ISetupSubSectionObject *> *newList = new QList<ISetupSubSectionObject*>();
        newList->append( subSection );
        _subsections.insert( sectionId, newList );
    } else {
        QList<ISetupSubSectionObject *> *list = _subsections.value( sectionId );
        list->append( subSection );
    }
}

void SettingsViewSetupFileParser::addSetting( QString subSectionId, ISetupSettingObject *setting ) {
    if( !_settings.contains( subSectionId ) ) {
        QList<ISetupSettingObject *> *newList = new QList<ISetupSettingObject *>();
        newList->append( setting );
        _settings.insert( subSectionId, newList );
    } else {
        QList<ISetupSettingObject *> *list = _settings.value( subSectionId );
        list->append( setting );
    }
}

QString SettingsViewSetupFileParser::getSettingsType() {
    return _type;
}

QList<ISetupSectionObject *> SettingsViewSetupFileParser::getSections() {
    return _sections;
}

QList<ISetupSubSectionObject *> SettingsViewSetupFileParser::getSubSectionsOfSection( QString sectionId ) {
    if( _subsections.contains( sectionId ) )
        return *( _subsections.value( sectionId ) );
    else
        return QList<ISetupSubSectionObject *>();
}

QList<ISetupSettingObject *> SettingsViewSetupFileParser::getSettingsOfSubSection( QString subsectionId ) {
    if( _settings.contains( subsectionId ) )
        return *( _settings.value( subsectionId ) );
    else
        return QList<ISetupSettingObject *>();
}

