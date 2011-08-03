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
#include "GridViewTransformWidget.h"

// Qt Includes
#include <QPainter>
#include <QDebug>
#include <math.h>
#include <QToolTip>
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


GridViewTransformWidget::GridViewTransformWidget( QWidget *parent, QImage img, QString mediaType, IMedia *mediaItem,
        bool isDownload, bool hasMainRegionFocus, bool hideAtStart, int id, ITextStyle *textStyle ):
    QFrame( parent ) {
    setCursor( Qt::PointingHandCursor );
    setStyleSheet( "QToolTip { background-color: #FBFAFD; font: Tahoma; }" );
    _id = id;
    _mediaType = mediaType;
    _isDownload = isDownload;
    _hasMainRegionFocus = hasMainRegionFocus;
    _isSelected = false;
    _hasHightLight = false;
    _transformMode = Qt::SmoothTransformation;
    _hasDefaultPlaceHolderImage = false;
    initMediaItemMembers( mediaItem );
    setupDisplayStrings();
    setupPixmaps( mediaType, img );
    setupStyle( textStyle );
    setToolTip( getToolTip() );
}

void GridViewTransformWidget::initMediaItemMembers( IMedia *mediaItem ) {
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

void GridViewTransformWidget::setupDisplayStrings() {
    setLengthString();
    setFileSizeString();
    setFileNameString();
    setCreatedAtString();
    setIsDownloadString();
}

void GridViewTransformWidget::setIsDownloadString() {
    if( _isDownload )
        _isDownloadString = "Yes";
    else
        _isDownloadString = "No";
}

void GridViewTransformWidget::setCreatedAtString() {
    _createdAtString = _createdAt.toString( Constants::LISTVIEW_PLAYLIST_DATE_DISPLAY_FORMAT );
}

void GridViewTransformWidget::setFileSizeString() {
    float filesizeMB = ( float )_fileSize / 1048576;
    _fileSizeString = QString( "%1 MB" ).arg( filesizeMB, 0, 'f', 1, ' ' );
}

void GridViewTransformWidget::setFileNameString() {
    _fileNameString = _fileName.right( _fileName.count()  - ( _fileName.lastIndexOf( "/" ) + 1 ) );
}

void GridViewTransformWidget::setLengthString() {
    _lengthString = "N/A";

    if( _mediaType != Constants::MEDIA_TYPE_IMAGE_NAME && _length > 0 ) {
        int lengthInSeconds = ( int )_length / 1000;
        QString hoursString = QString::number( ( int )( lengthInSeconds / 3600 ) ) + ":" ;
        int minutes = ( int )( ( lengthInSeconds % 3600 ) / 60  );
        QString minutesString = "";

        if( minutes > 9 ) {
            minutesString = QString::number( minutes ) + ":";
        } else {
            minutesString = "0" + QString::number( minutes ) + ":";
        }

        int seconds = lengthInSeconds   % 60 ;
        QString secondsString = "";

        if( seconds > 9 )
            secondsString = QString::number( seconds );
        else
            secondsString = "0" + QString::number( seconds );

        _lengthString = hoursString + minutesString + secondsString;
    }
}

QString GridViewTransformWidget::getToolTip() {
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

QString GridViewTransformWidget::getToolTipField( QString fieldContent, QString fieldLabel ) {
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

void GridViewTransformWidget::setupPixmaps( QString mediaType, QImage coverImage ) {
    setupCoverImage( mediaType, coverImage );
    setupContentTypeIcon( mediaType );
    setupIsNewIcon();
    setupHighlightPixmap();
    setupMainRegionFocusPixmap();
    setupSelectedBorderPixmaps();
}

void GridViewTransformWidget::setupCoverImage( QString mediaType, QImage coverImage ) {
    _img = QPixmap::fromImage( coverImage, 0 );

    if( _img.isNull() ) {
        _hasDefaultPlaceHolderImage = true;

        if( mediaType == Constants::MEDIA_TYPE_VIDEO_NAME )
            _img = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_MOVIE_THUMBNAIL );

        if ( mediaType == Constants::MEDIA_TYPE_AUDIO_NAME )
            _img = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_DEFAULT_MUSIC_THUMBNAIL );
    }
}

void GridViewTransformWidget::setupContentTypeIcon( QString mediaType ) {
    if( mediaType == Constants::MEDIA_TYPE_VIDEO_NAME ) {
        _icon = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_MOVIE_ICON );
    } else {
        if( mediaType == Constants::MEDIA_TYPE_AUDIO_NAME ) {
            _icon =  QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_MUSIC_ICON );
        } else {
            if( mediaType == Constants::MEDIA_TYPE_IMAGE_NAME )
                _icon = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_PICTURE_ICON );
        }
    }
}

void GridViewTransformWidget::setupIsNewIcon() {
    _newIcon = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_NEW_ICON );
}

void GridViewTransformWidget::setupHighlightPixmap() {
    _noHighLightOverlayPixmap = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_OVERLAYPATTERN );
}

void GridViewTransformWidget::setupMainRegionFocusPixmap() {
    _noMainRegionFocusPixmap = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLIST_MAINFOCUS_OVERLAY );
}

void GridViewTransformWidget::setupSelectedBorderPixmaps() {
    _selectedBorderTopPixmap = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLISTITEM_BORDER_TOP );
    _selectedBorderLeftPixmap = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLISTITEM_BORDER_LEFT );
    _selectedBorderBottomPixmap = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLISTITEM_BORDER_BOTTOM );
    _selectedBorderRightPixmap = QPixmap( Constants::GRIDVIEW_PLAYLIST_IMAGE_DIR + Constants::GRIDVIEW_PLAYLISTITEM_BORDER_RIGHT );
}

void GridViewTransformWidget::setupStyle( ITextStyle *textStyle ) {
    _textStyle = textStyle;
    _textColor = textStyle->foreColor();
    QFont::Weight fontWeight = QFont::Normal;

    if( textStyle->bold() )
        fontWeight = QFont::Bold;

    _textFont = QFont( textStyle->fontName(), textStyle->pointSize(), fontWeight, textStyle->italic() );
    _textFont.setPixelSize( textStyle->pointSize() );
}

void GridViewTransformWidget::paintEvent( QPaintEvent *e ) {
    QPainter p( this );
    int thumbnailSquareWidth = width() - 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING;
    QPixmap newIcon = getScaledIsNewIcon( thumbnailSquareWidth );
    int newIconHangingUpPartHeight = ( int )( newIcon.height() / 2 );
    int titleHeight = ( int )( height() - ( thumbnailSquareWidth
                                            + Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING
                                            + 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_TITLE_MARGIN_TOP
                                            + newIconHangingUpPartHeight ) ) ;
    //Thumbnail Drawing
    QPixmap img;
    Qt::TransformationMode transformMode = Qt::FastTransformation;

    if( _hasDefaultPlaceHolderImage )
        transformMode = Qt::SmoothTransformation;

    if( !_img.isNull() )
        img = _img.scaled( thumbnailSquareWidth, thumbnailSquareWidth, Qt::KeepAspectRatio, transformMode );

    if( ( img.width() < thumbnailSquareWidth && img.width() > 0 && !_img.isNull() )
            || ( img.height() < thumbnailSquareWidth && img.height() > 0 && !_img.isNull() ) ) {
        if( _mediaType == Constants::MEDIA_TYPE_IMAGE_NAME ) {
            int cropImageX = 0;
            int cropImageY = 0;
            bool cropHeight = false;
            bool cropWidth = false;

            if( img.height() < thumbnailSquareWidth ) {
                cropWidth = true;
            }

            if( img.width() < thumbnailSquareWidth ) {
                cropHeight = true;
            }

            img = _img.scaled( thumbnailSquareWidth, thumbnailSquareWidth, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation );

            if( cropWidth ) {
                cropImageX = ( img.width() - thumbnailSquareWidth ) / 2;
            } else {
                if( cropHeight ) {
                    cropImageY = ( img.height() - thumbnailSquareWidth ) / 2;
                }
            }

            img = img.copy( cropImageX, cropImageY, thumbnailSquareWidth, thumbnailSquareWidth );
            p.drawPixmap( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + newIconHangingUpPartHeight, img );
        } else {
            p.fillRect( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + newIconHangingUpPartHeight,
                        thumbnailSquareWidth, thumbnailSquareWidth, QColor( 0, 0, 0, 255 ) );

            if( img.height() < thumbnailSquareWidth ) {
                p.drawPixmap( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING,
                              Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + newIconHangingUpPartHeight + ( thumbnailSquareWidth - img.height() ) / 2,
                              img );
            } else {
                p.drawPixmap( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + ( thumbnailSquareWidth - img.width() ) / 2,
                              Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + newIconHangingUpPartHeight,
                              img );
            }
        }
    } else {
        p.drawPixmap( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + newIconHangingUpPartHeight, img );
    }

    //Text Drawing
    p.setPen( _textColor );
    p.setFont( _textFont );
    QFontMetrics fontMetrics = p.fontMetrics();
    p.drawText( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + 2,
                thumbnailSquareWidth + newIconHangingUpPartHeight
                + Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + Constants::GRIDVIEW_PLAYLIST_ITEM_TITLE_MARGIN_TOP , // 2* Padding daca vrem ca textu sa fie lipit de itemul de jos
                thumbnailSquareWidth, titleHeight, Qt::AlignLeft | Qt::AlignVCenter,
                fontMetrics.elidedText( _title, Qt::ElideRight, thumbnailSquareWidth ) );

    //Selected Graphics
    if( _isSelected ) {
        p.drawPixmap( 0, newIconHangingUpPartHeight, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, thumbnailSquareWidth + 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING,
                      _selectedBorderLeftPixmap.scaled( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, thumbnailSquareWidth + 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Qt::IgnoreAspectRatio, Qt::FastTransformation ) );
        p.drawPixmap( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + thumbnailSquareWidth + newIconHangingUpPartHeight
                      , width() - 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING,
                      _selectedBorderBottomPixmap.scaled( width() - 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Qt::IgnoreAspectRatio, Qt::FastTransformation ) );
        p.drawPixmap( width() - Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, newIconHangingUpPartHeight, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, thumbnailSquareWidth  + 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING,
                      _selectedBorderRightPixmap.scaled( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, thumbnailSquareWidth + 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Qt::IgnoreAspectRatio, Qt::FastTransformation ) );
        p.drawPixmap( Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, newIconHangingUpPartHeight
                      , width() - 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING,
                      _selectedBorderTopPixmap.scaled( width() - 2 * Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING, Qt::IgnoreAspectRatio, Qt::FastTransformation ) );
    }

    //icon Drawing
    int iconWidth = ( int )( 0.23 * thumbnailSquareWidth );
    int iconHeight = ( int )( 0.20 * thumbnailSquareWidth );
    QPixmap icon = _icon.scaled( iconWidth, iconHeight, Qt::KeepAspectRatio, Qt::FastTransformation );
    int iconXPosition = Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + ( int )( 0.75 * thumbnailSquareWidth );
    int iconYPosition = thumbnailSquareWidth + newIconHangingUpPartHeight - ( int )( 0.5 * icon.height() );
    p.drawPixmap( iconXPosition, iconYPosition, icon );

    //new Item Icon Drawing
    if( _isNew ) {
        int newIconXPosition = Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING + ( int )( thumbnailSquareWidth - newIcon.width() );
        p.drawPixmap( newIconXPosition, 0, newIcon );
    }

    //Overlay drawing
    if( !_hasMainRegionFocus ) {
        p.setBrush( QBrush( _noMainRegionFocusPixmap ) );
        p.drawRect( -1, -1, width() + 1, height() + 1 );
    } else {
        if( !_hasHightLight && !_isSelected ) {
            p.setBrush( QBrush( _noHighLightOverlayPixmap ) );
            p.drawRect( -1, -1, width() + 1, height() + 1 );
        }
    }
}

void GridViewTransformWidget::resizeEvent( QResizeEvent *e ) {
}


void GridViewTransformWidget::enterEvent( QEvent *e ) {
    _hasHightLight = true;
    update();
}

void GridViewTransformWidget::leaveEvent( QEvent *e ) {
    _hasHightLight = false;
    update();
}


void GridViewTransformWidget::scaleToSize( QSize newSize ) {
    /*_img = _img.scaled(newSize.width() - (2*Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING),
                       newSize.height()- (2*Constants::GRIDVIEW_PLAYLIST_ITEM_PADDING),
                       Qt::KeepAspectRatio, Qt::SmoothTransformation);*/
}

void GridViewTransformWidget::setTransformMode( Qt::TransformationMode newMode ) {
    _transformMode = newMode;
}

void GridViewTransformWidget::mousePressEvent ( QMouseEvent *  ) {
    emit clicked( _id );
}

void GridViewTransformWidget::mouseDoubleClickEvent ( QMouseEvent * ) {
    emit doubleClicked( _id );
}


void GridViewTransformWidget::changeMainUIRegionFocusGraphics( bool hasMainUIRegionFocus ) {
    _hasMainRegionFocus = hasMainUIRegionFocus;
    update();
}

void GridViewTransformWidget::changeCoverImage( QImage newImage ) {
    _img = QPixmap::fromImage( newImage, 0 );
    _hasDefaultPlaceHolderImage = false;
    update();
}

void GridViewTransformWidget::select() {
    _isSelected = true;
    update();
}

void GridViewTransformWidget::unSelect() {
    _isSelected = false;
    update();
}

void GridViewTransformWidget::setIsNewFlag( bool isNew ) {
    _isNew = isNew;
    update();
}

void GridViewTransformWidget:: setId( int newId ) {
    _id = newId;
}

QDateTime GridViewTransformWidget::createdAt() {
    return _createdAt;
}

bool GridViewTransformWidget::isNew() {
    return _isNew;
}

void GridViewTransformWidget::refreshMediaItemMembers( IMedia *mediaItem, bool immediateUpdate ) {
    initMediaItemMembers( mediaItem );
    setupDisplayStrings();
    setToolTip( getToolTip() );

    if( immediateUpdate )
        update();
}

QPixmap GridViewTransformWidget::getScaledIsNewIcon( int thumbnailWidth ) {
    int iconWidth = ( int )( Constants::GRIDVIEW_PLAYLIST_ITEM_ISNEW_ICON_WIDTH_PROCENT_FROM_THUMBNAIL_WIDTH * thumbnailWidth / 100 );
    QPixmap scaledIcon = _newIcon.scaledToWidth( iconWidth, Qt::SmoothTransformation );
    return scaledIcon;
}

