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
#ifndef TEXTSTYLE_H
#define TEXTSTYLE_H

// Qt includes
#include <QString>
#include <QColor>

// Project includes
#include "../../Interfaces/StyleFactory/ITextStyle.h"

namespace RedBullPlayer {
    namespace Modules {
        namespace StyleFactory {
            class TextStyle : public ITextStyle {
                public:
                    TextStyle();

                    virtual QString name() const {
                        return _name;
                    }
                    virtual QString fontName() const {
                        return _fontName;
                    }
                    virtual int pointSize() const {
                        return _pointSize;
                    }
                    virtual bool bold() const {
                        return _bold;
                    }
                    virtual bool italic() const {
                        return _italic;
                    }
                    virtual QColor foreColor() const {
                        return _foreColor;
                    }
                    virtual QString rgbaColorString() const {
                        return _rgbaColorString;
                    }
                    virtual Qt::Alignment alignment() const {
                        return _alignment;
                    }
                    virtual QString styleSheet() const;

                    void setName( QString name ) {
                        _name = name;
                    }
                    void setFontName( QString fontName ) {
                        _fontName = fontName;
                    }
                    void setPointSize( int pointSize ) {
                        _pointSize = pointSize;
                    }
                    void setBold( bool bold ) {
                        _bold = bold;
                    }
                    void setItalic( bool italic ) {
                        _italic = italic;
                    }
                    void setForeColor ( QColor color ) {
                        _foreColor = color;
                    }
                    void addAlignment( Qt::AlignmentFlag flag );
                    void setRgbaColorString( QString rgbaColorString ) {
                        _rgbaColorString = rgbaColorString;
                    }
                private:
                    QString _name;
                    QString _fontName;
                    int _pointSize;
                    bool _bold;
                    bool _italic;
                    QColor _foreColor;
                    Qt::Alignment _alignment;
                    QString _rgbaColorString;
            };
        }
    }
}

#endif // TEXTSTYLE_H
