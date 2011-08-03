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
#include "styleparsertest.h"

// QT includes
#include <QFileInfo>
#include <QMap>
#include <QFile>

#include "styleparser.h"
#include "ITextStyle.h"

StyleParserTest::StyleParserTest() {
}

void StyleParserTest::test_file_should_exists() {
    QFileInfo configFile( "testStyles.xml" );
    QVERIFY( QFile::exists( configFile.absoluteFilePath() ) );
}

void StyleParserTest::should_have_any_textStyles() {
    QFileInfo configFile( "testStyles.xml" );
    RedBullPlayer::Modules::StyleFactory::StyleParser parser( this );
    QMap<QString, RedBullPlayer::Modules::StyleFactory::ITextStyle*>* textStyles = parser.parseConfigFile( configFile );
    QVERIFY( textStyles->count() > 0 );
}

void StyleParserTest::should_have_3_textStyles() {
    QFileInfo configFile( "testStyles.xml" );
    RedBullPlayer::Modules::StyleFactory::StyleParser parser( this );
    QMap<QString, RedBullPlayer::Modules::StyleFactory::ITextStyle*>* textStyles = parser.parseConfigFile( configFile );
    QVERIFY( textStyles->count() == 3 );
}

void StyleParserTest::should_have_SearchUI_HeaderText_style() {
    QFileInfo configFile( "testStyles.xml" );
    RedBullPlayer::Modules::StyleFactory::StyleParser parser( this );
    QMap<QString, RedBullPlayer::Modules::StyleFactory::ITextStyle*>* textStyles = parser.parseConfigFile( configFile );
    QVERIFY( textStyles->contains( "SearchUI_HeaderText" ) );
}

void StyleParserTest::should_have_SearchUI_HeaderText_and_all_values() {
    QFileInfo configFile( "testStyles.xml" );
    RedBullPlayer::Modules::StyleFactory::StyleParser parser( this );
    QMap<QString, RedBullPlayer::Modules::StyleFactory::ITextStyle*>* textStyles = parser.parseConfigFile( configFile );
    QVERIFY( textStyles->contains( "SearchUI_HeaderText" ) );
    RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle = textStyles->value( "SearchUI_HeaderText" );
    QVERIFY( textStyle->fontName() == "Tahoma" );
    QVERIFY( textStyle->pointSize() == 12 );
    QVERIFY( textStyle->bold() );
    QVERIFY( textStyle->italic() );
    QVERIFY( textStyle->foreColor() == QColor( 255, 0, 0, 0 ) );
    QVERIFY( ( textStyle->alignment() & Qt::AlignLeft ) == Qt::AlignLeft );
    QVERIFY( ( textStyle->alignment() & Qt::AlignTop ) == Qt::AlignTop );
}

void StyleParserTest::should_have_CoverView_HeaderText_and_Tahoma_as_font() {
    QFileInfo configFile( "testStyles.xml" );
    RedBullPlayer::Modules::StyleFactory::StyleParser parser( this );
    QMap<QString, RedBullPlayer::Modules::StyleFactory::ITextStyle*>* textStyles = parser.parseConfigFile( configFile );
    QVERIFY( textStyles->contains( "CoverView_HeaderText" ) );
    RedBullPlayer::Modules::StyleFactory::ITextStyle* textStyle = textStyles->value( "CoverView_HeaderText" );
    QVERIFY( textStyle->fontName() == "Tahoma" );
    QVERIFY( !textStyle->bold() );
}
