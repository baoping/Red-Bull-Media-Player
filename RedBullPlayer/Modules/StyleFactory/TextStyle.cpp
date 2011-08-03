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
#include "TextStyle.h"

using namespace RedBullPlayer::Modules::StyleFactory;

TextStyle::TextStyle() {
    _fontName = "Tahoma";
    _pointSize = 10;
    _bold = false;
    _italic = false;
    _foreColor = QColor( Qt::black );
    _rgbaColorString = "rgba(255,255,255,255)";
}

void TextStyle::addAlignment( Qt::AlignmentFlag flag ) {
    _alignment = ( _alignment | flag );
}

QString TextStyle::styleSheet() const {
    QString styleSheet = "";
    QString fontFamily = QString( "font-family:%1;" ).arg( fontName() );
    QString textColor =  QString( "color:rgb(%1,%2,%3);" )
                         .arg( foreColor().red() )
                         .arg( foreColor().green() )
                         .arg( foreColor().blue() );
    QString fontSize = QString( "font-size:%1px;" ).arg( pointSize() );
    QString fontWeight = "";

    if( bold() )
        fontWeight = QString( "font-weight:bold;" );

    QString fontStyle = "";

    if( italic() )
        fontStyle = QString( "font-style:italic;" );

    styleSheet += ( textColor + fontFamily + fontWeight + fontStyle + fontSize );
    return styleSheet;
}

