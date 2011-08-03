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
#include "ListViewTransformWidget.h"

// Qt Includes
#include <QPainter>
#include <math.h>
#include <QApplication>
#include <QDesktopWidget>

//Projects
#include "../Container/Constants.h"
#include "../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Modules::MediaLibrary;

ListViewTransformWidget::ListViewTransformWidget( QWidget *parent, QString mediaType, IMedia *mediaItem, bool isDownload,
        bool hasMainRegionFocus, int id,
        ITextStyle *textNormalStyle, ITextStyle *textSelectedStyle ) : QWidget( parent ) {
    setCursor( Qt::PointingHandCursor );
    setStyleSheet( "QToolTip { background-color: #FBFAFD; font: Tahoma; }" );
    _id = id;
    _mediaType = mediaType;
    _isDownload = isDownload;
    _hasMainRegionFocus = hasMainRegionFocus;
    _isSelected = false;
    initMediaItemMembers( mediaItem );
    setupDisplayStrings();
    setupStyle( textNormalStyle, textSelectedStyle );
    setToolTip( getToolTip() );
    setupPixmaps( mediaType );
}

void ListViewTransformWidget::setupDisplayStrings() {
    setLengthString();
    setFileSizeString();
    setFileNameString();
    setCreatedAtString();
    setIsDownloadString();
}

void ListViewTransformWidget::initMediaItemMembers( IMedia *mediaItem ) {
    _mediaGuid = mediaItem->guid();
    _title = mediaItem->title();
    _artist = mediaItem->artist();
    _album = mediaItem->album();
    _createdAt = mediaItem->createdAt();
    _length = mediaItem->length();
    _isNew = mediaItem->isNew();
    _fileName = mediaItem->fileName();
    _fileSize = mediaItem->fileSize();
}

void ListViewTransformWidget::setupPixmaps( QString mediaType ) {
    setupIsNewIcon();
    setupMainRegionFocusPixmap();
    setupContentTypeIcon( mediaType );
}

void ListViewTransformWidget::setupIsNewIcon() {
    _newIcon = QPixmap( Constants::LISTVIEW_PLAYLIST_IMAGE_DIR + Constants::LISTVIEW_PLAYLIST_NEW_ICON );

    if( !_newIcon.isNull() )
        _newIcon = _newIcon.scaledToHeight( Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT, Qt::SmoothTransformation );
}

void ListViewTransformWidget::setupMainRegionFocusPixmap() {
    _noMainRegionFocusPixmap = QPixmap( Constants::LISTVIEW_PLAYLIST_IMAGE_DIR + Constants::LISTVIEW_PLAYLIST_MAINFOCUS_OVERLAY );
}

void ListViewTransformWidget::setupContentTypeIcon( QString mediaType ) {
    if( mediaType == Constants::MEDIA_TYPE_VIDEO_NAME ) {
        _icon = QPixmap( Constants::LISTVIEW_PLAYLIST_IMAGE_DIR + Constants::LISTVIEW_PLAYLIST_MOVIE_ICON );
    } else {
        if( mediaType == Constants::MEDIA_TYPE_AUDIO_NAME ) {
            _icon =  QPixmap( Constants::LISTVIEW_PLAYLIST_IMAGE_DIR + Constants::LISTVIEW_PLAYLIST_MUSIC_ICON );
        } else {
            if( mediaType == Constants::MEDIA_TYPE_IMAGE_NAME )
                _icon = QPixmap( Constants::LISTVIEW_PLAYLIST_IMAGE_DIR + Constants::LISTVIEW_PLAYLIST_PICTURE_ICON );
        }
    }

    if( !_icon.isNull() )
        _icon = _icon.scaledToHeight( ( int )Constants::LISTVIEW_PLAYLIST_ICON_HEIGHT_PROCENT_FROM_BOUNDINGBOX * Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT / 100,
                                      Qt::SmoothTransformation );
}

void ListViewTransformWidget::setupStyle( ITextStyle *textNormalStyle, ITextStyle *textSelectedStyle ) {
    setupNormalStyle( textNormalStyle );
    setupSelectedStyle( textSelectedStyle );
}

void ListViewTransformWidget::setupNormalStyle( ITextStyle *textNormalStyle ) {
    _textNormalStyle = textNormalStyle;
    _textNormalColor = textNormalStyle->foreColor();
    QFont::Weight fontWeight = QFont::Normal;

    if( textNormalStyle->bold() )
        fontWeight = QFont::Bold;

    _textNormalFont = QFont( textNormalStyle->fontName(), textNormalStyle->pointSize(), fontWeight, textNormalStyle->italic() );
    _textNormalFont.setPixelSize( textNormalStyle->pointSize() );
}

void ListViewTransformWidget::setupSelectedStyle( ITextStyle *textSelectedStyle ) {
    _textSelectedStyle = textSelectedStyle;
    _textSelectedColor = textSelectedStyle->foreColor();
    QFont::Weight fontWeight = QFont::Normal;

    if( textSelectedStyle->bold() )
        fontWeight = QFont::Bold;

    _textSelectedFont = QFont( textSelectedStyle->fontName(), textSelectedStyle->pointSize(), fontWeight, textSelectedStyle->italic() );
    _textSelectedFont.setPixelSize( textSelectedStyle->pointSize() );
}

void ListViewTransformWidget::refreshMediaItemMembers( IMedia *mediaItem, bool immediateUpdate ) {
    initMediaItemMembers( mediaItem );
    setupDisplayStrings();
    setToolTip( getToolTip() );

    if( immediateUpdate )
        update();
}

void ListViewTransformWidget::paintEvent( QPaintEvent *e ) {
    QPainter p( this );
    int currentWidth = width();
    int textOffsetX = _newIcon.width();
    currentWidth -= textOffsetX;

    //is new Icon drawing
    if( !_newIcon.isNull() && _isNew ) {
        p.drawPixmap( 0, ( int )( 0.5 * ( Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT - _newIcon.height() ) ), _newIcon );
    }

    //type icon drawing
    int currentWidthColumnIcon = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ICON * currentWidth  / 100 );

    if( !_icon.isNull() ) {
        p.drawPixmap( ( int )( 0.5 * ( currentWidthColumnIcon - _icon.width() ) ) + textOffsetX,
                      ( int )( 0.5 * ( Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT - _icon.height() ) ), _icon );
    }

    textOffsetX += currentWidthColumnIcon;

    //text Drawing
    if( _isSelected ) {
        p.setPen( _textSelectedColor );
        p.setFont( _textSelectedFont );
    } else {
        p.setPen( _textNormalColor );
        p.setFont( _textNormalFont );
    }

    QFontMetrics fontMetrics = p.fontMetrics();
    //title text drawing
    int currentWidthColumnTitle = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_TITLE * currentWidth / 100 );
    p.drawText( textOffsetX, 0,
                currentWidthColumnTitle - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( _title, Qt::ElideRight, currentWidthColumnTitle - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );
    //artist text Drawing
    textOffsetX += currentWidthColumnTitle;
    int currentWidthColumnArtist = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ARTIST  * currentWidth / 100 );
    p.drawText( textOffsetX, 0,
                currentWidthColumnArtist  - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( _artist, Qt::ElideRight, currentWidthColumnArtist - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );
    //album text Drawing
    textOffsetX += currentWidthColumnArtist;
    int currentWidthColumnAlbum = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_ALBUM  * currentWidth / 100 );
    p.drawText( textOffsetX, 0,
                currentWidthColumnAlbum  - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( _album, Qt::ElideRight, currentWidthColumnAlbum - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );
    //Draw Created at
    textOffsetX += currentWidthColumnAlbum;
    int currentWidthColumnCreatedAt = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_CREATED_AT * currentWidth / 100 );
    p.drawText( textOffsetX, 0,
                currentWidthColumnCreatedAt  - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( _createdAtString, Qt::ElideRight, currentWidthColumnCreatedAt - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );
    //draw length
    textOffsetX += currentWidthColumnCreatedAt;
    int currentWidthColumnLength = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_LENGTH * currentWidth / 100 );
    p.drawText( textOffsetX, 0,
                currentWidthColumnLength  - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT,
                Qt::AlignRight | Qt::AlignVCenter,
                fontMetrics.elidedText( _lengthString, Qt::ElideRight, currentWidthColumnLength - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );
    //draw Download Text
    textOffsetX += currentWidthColumnLength;
    int currentWidthColumnDownload = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_DOWNLOAD * currentWidth / 100 );
    p.drawText( textOffsetX, 0,
                currentWidthColumnDownload  - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT,
                Qt::AlignCenter | Qt::AlignVCenter,
                fontMetrics.elidedText( _isDownloadString, Qt::ElideRight, currentWidthColumnDownload ) );
    //draw FileName Text
    textOffsetX += currentWidthColumnDownload;
    int currentWidthColumnFilename = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_FILENAME * currentWidth / 100 );
    p.drawText( textOffsetX, 0,
                currentWidthColumnFilename  - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING, Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT,
                Qt::AlignLeft | Qt::AlignVCenter | Qt::ElideRight,
                fontMetrics.elidedText( _fileNameString, Qt::ElideRight, currentWidthColumnFilename - Constants::LISTVIEW_PLAYLIST_COLUMN_PADDING ) );
    //draw FileSize Text
    textOffsetX += currentWidthColumnFilename;
    int currentWidthFilesizeColumn = ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_FILESIZE * currentWidth / 100 ) +
                                     ( int )( Constants::LISTVIEW_PLAYLIST_WIDTH_COLUMN_PERCENT_IS_NEW * currentWidth  / 100 );
    p.drawText( textOffsetX, 0,
                currentWidthFilesizeColumn, Constants::LISTVIEW_PLAYLIST_ITEM_HEIGHT,
                Qt::AlignRight | Qt::AlignVCenter, fontMetrics.elidedText( _fileSizeString, Qt::ElideRight, currentWidthFilesizeColumn ) );

    //Overlay drawing
    if( !_hasMainRegionFocus ) {
        p.setBrush( QBrush( _noMainRegionFocusPixmap ) );
        p.drawRect( -1, -1, width() + 1, height() + 1 );
    }
}

void ListViewTransformWidget::mousePressEvent ( QMouseEvent *  ) {
    emit clicked( _id );
}

void ListViewTransformWidget::mouseDoubleClickEvent ( QMouseEvent * ) {
    emit doubleClicked( _id );
}

void ListViewTransformWidget::select() {
    _isSelected = true;
    update();
}

void ListViewTransformWidget::unSelect() {
    _isSelected = false;
    update();
}

void ListViewTransformWidget::setIsNewFlag( bool isNew, bool immediateUpdate ) {
    _isNew = isNew;

    if( immediateUpdate )
        update();
}

void ListViewTransformWidget::setLength( int lengthSeconds, bool immediateUpdate ) {
    _length = lengthSeconds;
    setLengthString();
    setToolTip( getToolTip() );

    if( immediateUpdate )
        update();
}

void ListViewTransformWidget::setId( int newId ) {
    _id = newId;
}

QDateTime ListViewTransformWidget::createdAt() {
    return _createdAt;
}

bool ListViewTransformWidget::isNew() {
    return _isNew;
}

void ListViewTransformWidget::changeMainUIRegionFocusGraphics( bool hasMainUIRegionFocus ) {
    _hasMainRegionFocus = hasMainUIRegionFocus;
    update();
}

void ListViewTransformWidget::setLengthString() {
    _lengthString = "N/A";

    if( _mediaType != Constants::MEDIA_TYPE_IMAGE_NAME && _length > 0 ) {
        int lengthInSeconds = ( int ) _length / 1000;
        QString hoursString = QString::number( ( int )( lengthInSeconds / 3600 ) ) + ":" ;
        int minutes = ( int )( ( lengthInSeconds % 3600 ) / 60 );
        QString minutesString = "";

        if( minutes > 9 ) {
            minutesString = QString::number( minutes ) + ":";
        } else {
            minutesString = "0" + QString::number( minutes ) + ":";
        }

        int seconds = lengthInSeconds % 60;
        QString secondsString = "";

        if( seconds > 9 )
            secondsString = QString::number( seconds );
        else
            secondsString = "0" + QString::number( seconds );

        _lengthString = hoursString + minutesString + secondsString;
    }
}

void ListViewTransformWidget::setIsDownloadString() {
    if( _isDownload )
        _isDownloadString = "Yes";
    else
        _isDownloadString = "No";
}

void ListViewTransformWidget::setFileSizeString() {
    float filesizeMB = ( float )_fileSize / 1048576;
    _fileSizeString = QString( "%1 MB" ).arg( filesizeMB, 0, 'f', 2, ' ' );
}

void ListViewTransformWidget::setFileNameString() {
    _fileNameString = _fileName.right( _fileName.count()  - ( _fileName.lastIndexOf( "/" ) + 1 ) );
}

void ListViewTransformWidget::setCreatedAtString() {
    _createdAtString = _createdAt.toString( Constants::LISTVIEW_PLAYLIST_DATE_DISPLAY_FORMAT );
}

QString ListViewTransformWidget::getToolTip() {
    QString toolTip = "";

    if( !_title.isEmpty() )
        toolTip += QString( "Title : %1" ).arg( getToolTipField( _title, "Title : " ) );

    if( !_artist.isEmpty() )
        toolTip += QString( "\n Artist : %1" ).arg( getToolTipField( _artist, "Artist : " ) );

    if( !_album.isEmpty() )
        toolTip += QString( "\n Album : %1" ).arg( getToolTipField( _album, "Album : " ) );

    if( !_createdAtString.isEmpty() )
        toolTip += QString( "\n Added : %1" ).arg( _createdAtString );

    if( !_lengthString.isEmpty() && _lengthString != "N/A" )
        toolTip += QString( "\n Length : %1" ).arg( _lengthString );

    if( !_isDownloadString.isEmpty() )
        toolTip += QString( "\n Downloaded : %1" ).arg( _isDownloadString );

    if( !_fileNameString.isEmpty() )
        toolTip += QString( "\n Filename : %1" ).arg( getToolTipField( _fileNameString, "Filename : " ) );

    if( !_fileSizeString.isEmpty() && _fileSizeString != "0.0 MB" )
        toolTip += QString( "\n FileSize : %1" ).arg( _fileSizeString );

    return toolTip;
}

QString ListViewTransformWidget::getToolTipField( QString fieldContent, QString fieldLabel ) {
    QString toolTipField = "";
    QString rowBreak = QString( "\n " ) + QString( fieldLabel.count(), QChar( ' ' ) );
    QFontMetrics fontMetrics = QFontMetrics( QFont( "Tahoma" ) );
    int availableCharactersPerRow =  ( int )( ( ( int )( QApplication::desktop()->availableGeometry().width() / 3 )
                                     - fontMetrics.boundingRect( fieldLabel ).width() )
                                     / fontMetrics.averageCharWidth() );

    if( fieldContent.count() <= availableCharactersPerRow ) {
        toolTipField = fieldContent;
    } else {
        int numberOfRowBreaks = ( int )ceil( fieldContent.count() / availableCharactersPerRow );
        toolTipField += fieldContent.mid( 0, availableCharactersPerRow );

        for( int i = 1; i <= numberOfRowBreaks; i++ ) {
            toolTipField += rowBreak;
            toolTipField += fieldContent.mid( i * availableCharactersPerRow, availableCharactersPerRow );
        }
    }

    return toolTipField;
}
