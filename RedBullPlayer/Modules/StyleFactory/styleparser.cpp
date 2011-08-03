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
#include "styleparser.h"

#include <QFile>

#include "TextStyle.h"

#include "../../Container/Constants.h"
#include "../../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;

StyleParser::StyleParser( QObject *parent ) :
    QObject( parent ) {
}

/**
 * Parse an configuration Xml for ITextStyles
 *
 * \return an QMap of ITextStyles with the name as key (or an empty list in case of parse errors)
 */
QMap<QString, ITextStyle*>* StyleParser::parseConfigFile( QFileInfo configFile ) {
    QMap<QString, ITextStyle*>* ret = new QMap<QString, ITextStyle*>();

    if ( !QFile::exists( configFile.absoluteFilePath() ) ) {
        return ret;
    }

    QFile file( configFile.absoluteFilePath() );

    if ( !file.open( QFile::ReadOnly ) )
        return ret;

    QXmlStreamReader xml( file.readAll() );
    file.close();

    while ( !xml.atEnd() && !xml.hasError() ) {
        xml.readNext(); //read next token

        //TextStyle
        if ( xml.isStartElement() && ( xml.name() == "TextStyle" ) ) {
            ITextStyle* textStyle = parseTextStyleElement( &xml );

            if ( !ret->contains( textStyle->name() ) ) {
                ret->insert( textStyle->name(), textStyle );
            }
        }
    }

    return ret;
}

/**
 * Parse a single <TextStyle> element
 *
 * \return an ITextStyle representing the <TextStyle> element
 */
ITextStyle* StyleParser::parseTextStyleElement( QXmlStreamReader* xml ) {
    TextStyle* ts = new TextStyle();
    ts->setName( xml->attributes().value( "name" ).toString() );
    bool hAlign = false;
    bool vAlign = false;

    while ( !( xml->isEndElement() && ( xml->name() == "TextStyle" ) ) ) {
        xml->readNext();

        if ( xml->isStartElement() ) {
            if ( xml->name() == "Font" ) {
                ts->setFontName( xml->readElementText() );
            }

            if ( xml->name() == "Size" ) {
                ts->setPointSize( xml->readElementText().toInt() );
            }

            if ( xml->name() == "Bold" ) {
                ts->setBold( true );
            }

            if ( xml->name() == "Italic" ) {
                ts->setItalic( true );
            }

            if ( xml->name() == "Color" ) {
                int r = xml->attributes().value( "r" ).toString().toInt();
                int g = xml->attributes().value( "g" ).toString().toInt();
                int b = xml->attributes().value( "b" ).toString().toInt();
                int a = xml->attributes().value( "a" ).toString().toInt();
                ts->setForeColor( QColor( r, g, b, a ) );
                ts->setRgbaColorString( QString( "rgba(%1,%2,%3,%4)" ).arg( r )
                                        .arg( g )
                                        .arg( b )
                                        .arg( a ) );
            }

            if ( xml->name() == "HAlign" ) {
                QString a = xml->readElementText();
                hAlign = true;

                if ( a.toLower() == "left" )
                    ts->addAlignment( Qt::AlignLeft );
                else if ( a.toLower() == "right" )
                    ts->addAlignment( Qt::AlignRight );
                else if ( a.toLower() == "center" )
                    ts->addAlignment( Qt::AlignHCenter );
                else
                    hAlign = false;
            }

            if ( xml->name() == "VAlign" ) {
                QString a = xml->readElementText();
                vAlign = true;

                if ( a.toLower() == "top" )
                    ts->addAlignment( Qt::AlignTop );
                else if ( a.toLower() == "bottom" )
                    ts->addAlignment( Qt::AlignBottom );
                else if ( a.toLower() == "center" )
                    ts->addAlignment( Qt::AlignVCenter );
                else
                    vAlign = false;
            }
        }
    }

    //Set some default values
    if ( !hAlign )
        ts->addAlignment( Qt::AlignLeft );

    if ( !vAlign )
        ts->addAlignment( Qt::AlignTop );

    return ts;
}
