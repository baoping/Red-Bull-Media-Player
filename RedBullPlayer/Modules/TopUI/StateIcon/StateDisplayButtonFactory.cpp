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
#include "StateDisplayButtonFactory.h"

#include <QFontMetrics>

#include "../../../Widgets/DiskSpaceButton.h"
#include "../../../Container/Constants.h"
#include "../../../Widgets/StatusCountDisplayButton.h"
#include "../../../Widgets/InternetAvailabilityButton.h"

using namespace RedBullPlayer::Modules::TopUI;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Modules::TaskManager;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::StyleFactory;

StateDisplayButtonFactory::StateDisplayButtonFactory( QWidget* buttonParent, IStyleFactory *styleFactory, QObject *parent ) :
    QObject( parent ) {
    _buttonParent = buttonParent;
    _styleFactory = styleFactory;
}

StatusDisplayButton* StateDisplayButtonFactory::createButtonFor( RedBullPlayer::Modules::TaskManager::TaskPurpose::PurposeEnum purpose ) {
    QString icon = "";
    QString disabledIcon = "";

    switch( purpose )  {
        case TaskPurpose::UPDATE: {
                icon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_APPLICATIONUPDATE_ICON;
                disabledIcon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_APPLICATIONUPDATE_DISABLED_ICON;
                break;
            }

        case TaskPurpose::DOWNLOAD:  {
                icon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_CONTENTDOWNLOAD_ICON;
                disabledIcon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_CONTENTDOWNLOAD_DISABLED_ICON;
                break;
            }

        case TaskPurpose::IO:  {
                icon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_FILETASKS_ICON;
                disabledIcon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_FILETASKS_DISABLED_ICON;
                break;
            }

        case TaskPurpose::DISK_SPACE:  {
                icon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_DISK_SPACE_ICON;
                disabledIcon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_DISK_SPACE_PROGRESS_ICON;
                break;
            }

        case TaskPurpose::INTERNET_AVAILABILITY: {
                icon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_ONLINE_ICON;
                disabledIcon = Constants::STATUSDISPLAY_IMAGE_DIR + Constants::STATUSDISPLAY_OFFLINE_ICON;
                break;
            }

        case TaskPurpose::UNKNOWN : {
                break;
            }
    }

    if ( !icon.isEmpty() && !disabledIcon.isEmpty() ) {
        if ( purpose == TaskPurpose::DISK_SPACE ) {
            return new DiskSpaceButton( icon, disabledIcon, _buttonParent, 2, 10, 2, 2 );
        } else {
            if( purpose == TaskPurpose::DOWNLOAD ) {
                return createDownloadStatusCountButton( icon, disabledIcon );
            } else {
                if( purpose == TaskPurpose::INTERNET_AVAILABILITY ) {
                    return new InternetAvailabilityButton( icon, disabledIcon, _buttonParent );
                } else {
                    return new StatusDisplayButton( icon, disabledIcon, _buttonParent );
                }
            }
        }
    }

    return 0;
}

StatusDisplayButton *StateDisplayButtonFactory::createDownloadStatusCountButton( QString icon, QString disabledIcon ) {
    QColor borderColor = QColor( 205, 209, 210 );
    qreal circleBorderWidth = 2.0;
    QColor circleBackgoundColor = QColor( 9, 57, 131 );
    QColor textColor = QColor( 205, 209, 210 );
    QFont font = QFont( "Tahoma", 13, QFont::Bold, false );
    font.setPixelSize( 13 );

    if( _styleFactory != NULL ) {
        textColor = getCountButtonTextColor();
        font = getCountButtonFont();
    }

    QFontMetrics fontMetrics = QFontMetrics( font );
    int textHeight = fontMetrics.height();
    int textWidth = fontMetrics.width( "999" ); //max Value for the count Display
    int radius =  0;

    if( textHeight > textWidth )
        radius = (int) ( 0.7 * textHeight );
    else
        radius = (int) ( 0.7 * textWidth );

    QPixmap iconImage = QPixmap( icon );
    int buttonWidth = (int) ( iconImage.width() + 1.6 * radius + circleBorderWidth );
    int buttonHeight = (int) ( iconImage.height() + 1.6 * radius + circleBorderWidth );
    return new StatusCountDisplayButton( buttonWidth, buttonHeight,
                                         font, borderColor, circleBorderWidth,
                                         circleBackgoundColor, textColor,
                                         icon, disabledIcon, _buttonParent );
}

QColor StateDisplayButtonFactory::getCountButtonTextColor() {
    bool ok;
    ITextStyle *countStyle = _styleFactory->getTextStyle( "AvailableDownload_Count_DisplayText", &ok );

    if( ok )
        return countStyle->foreColor();
    else
        return QColor( 0, 0, 0 );
}

QFont StateDisplayButtonFactory::getCountButtonFont() {
    QFont font = QFont( "Tahoma", 13, QFont::Bold, false );
    font.setPixelSize( 13 );
    bool ok;
    ITextStyle *countStyle = _styleFactory->getTextStyle( "AvailableDownload_Count_DisplayText", &ok );

    if( ok ) {
        font.setFamily( countStyle->fontName() );
        font.setPixelSize( countStyle->pointSize() );

        if( countStyle->bold() )
            font.setBold( true );
        else
            font.setBold( false );

        if( countStyle->italic() )
            font.setItalic( true );
        else
            font.setItalic( false );
    }

    return font;
}
