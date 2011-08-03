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
#include "TransformWidget.h"

// Qt Includes
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

#include <math.h>
#include "../Container/Constants.h"
#include "../ToolLib/SimpleLogger.h"

using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::Tools;

TransformWidget::TransformWidget( QWidget *parent, QImage img, QString mediaType, IMedia *mediaItem, bool isDownload, bool hasMainRegionFocus, bool hideAtStart, int id ) :
    QWidget( parent ) {
    setCursor( Qt::PointingHandCursor );
    setStyleSheet( "QToolTip { background-color: #FBFAFD; font: Tahoma; } background-color:transparent;" );
    _id = id;
    _mediaType = mediaType;
    _isDownload = isDownload;
    _hasHightLight = false;
    _isSelected = false;
    _hasMainRegionFocus = hasMainRegionFocus;
    _transform = QTransform( 1, 0, 0, 0, 1, 0, 0, 0 );
    _transformIsNewIcon = QTransform( 1, 0, 0, 0, 1, 0, 0, 0 );
    _fadeStyle = MirrorWidget::NoFade;
    _transformMode = Qt::SmoothTransformation;
    initMediaItemMembers( mediaItem );
    setupDisplayStrings();
    setupPixmaps( mediaType, img );
    setToolTip( getToolTip() );

    if( hideAtStart ) {
        hide();
    }
}

void TransformWidget::initMediaItemMembers( IMedia *mediaItem ) {
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

void TransformWidget::setupDisplayStrings() {
    setLengthString();
    setFileSizeString();
    setFileNameString();
    setCreatedAtString();
    setIsDownloadString();
}

void TransformWidget::setIsDownloadString() {
    if( _isDownload )
        _isDownloadString = "Yes";
    else
        _isDownloadString = "No";
}

void TransformWidget::setCreatedAtString() {
    _createdAtString = _createdAt.toString( Constants::LISTVIEW_PLAYLIST_DATE_DISPLAY_FORMAT );
}

void TransformWidget::setFileNameString() {
    _fileNameString = _fileName.right( _fileName.count()  - ( _fileName.lastIndexOf( "/" ) + 1 ) );
}

void TransformWidget::setFileSizeString() {
    float filesizeMB = ( float )_fileSize / 1048576;
    _fileSizeString = QString( "%1 MB" ).arg( filesizeMB, 0, 'f', 1, ' ' );
}

void TransformWidget::setLengthString() {
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

QString TransformWidget::getToolTip() {
    QString toolTip = "";

    if( !_title.isEmpty() )
        toolTip += QString( "Title : %1" ).arg( getToolTipField( _title, "Title : " ) );

    if( _mediaType == Constants::MEDIA_TYPE_VIDEO_NAME || _mediaType == Constants::MEDIA_TYPE_AUDIO_NAME || _mediaType == Constants::MEDIA_TYPE_IMAGE_NAME ) {
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
    }

    return toolTip;
}

QString TransformWidget::getToolTipField( QString fieldContent, QString fieldLabel ) {
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

void TransformWidget::setupPixmaps( QString mediaType, QImage coverImage ) {
    setupCoverImage( mediaType, coverImage );
    setupHighlightPixmap();
    setupIsNewIcon();
    setupMainRegionFocusPixmap();
    setupContentTypeIcon( mediaType );
}

void TransformWidget::setupContentTypeIcon( QString mediaType ) {
    if( mediaType == Constants::MEDIA_TYPE_VIDEO_NAME ) {
        _icon = QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_MOVIE_ICON );
    } else {
        if( mediaType == Constants::MEDIA_TYPE_AUDIO_NAME ) {
            _icon = QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_MUSIC_ICON );
        } else {
            if( mediaType == Constants::MEDIA_TYPE_IMAGE_NAME )
                _icon = QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_PICTURE_ICON );
        }
    }
}

void TransformWidget::setupCoverImage( QString mediaType, QImage coverImage ) {
    _img = coverImage;

    if( _img.isNull() ) {
        if ( mediaType == Constants::MEDIA_TYPE_VIDEO_NAME )
            _img =  QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_DEFAULT_MOVIE_THUMBNAIL );
        else if ( mediaType == Constants::MEDIA_TYPE_AUDIO_NAME )
            _img =  QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_DEFAULT_MUSIC_THUMBNAIL );
    }
}

void TransformWidget::setupHighlightPixmap() {
    _noHighLightOverlayPixmap = QPixmap( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_OVERLAYPATTERN );
}

void TransformWidget::setupMainRegionFocusPixmap() {
    _noMainRegionFocusPixmap = QPixmap( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_MAINFOCUS_OVERLAY );
}

void TransformWidget::setupIsNewIcon() {
    _newIcon = QImage( Constants::COVERVIEW_PLAYLIST_IMAGE_DIR + Constants::COVERVIEW_PLAYLIST_NEW_ICON );
}

void TransformWidget::paintEvent( QPaintEvent */*e*/ ) {
    QPainter p( this );
    float contentTypeIconScaleFactor = ( float )Constants::COVERVIEW_PLAYLIST_ITEM_CONTENTTYPE_ICON_SIZE_PROCENT / ( float )100;
    float isNewIconScaleFactor = ( float )Constants::COVERVIEW_PLAYLIST_ITEM_ISNEW_ICON_SIZE_PROCENT / ( float )100;
    QImage newIcon = _newIcon.scaledToWidth( ( int )( isNewIconScaleFactor * width() ), Qt::SmoothTransformation ).transformed( _transformIsNewIcon, Qt::SmoothTransformation );
    QImage img = _img.transformed( _transform, _transformMode );
    int widthDistance = ( width() - img.width() ) / 2;
    int heightDistance = ( height() / 2 )  - img.height() + ( newIcon.height() / 2 );
    p.drawImage( widthDistance, heightDistance , img );
    _mirroredImg = MirrorWidget::mirrorImage( _img, 50, MirrorWidget::MirrorOverX, _fadeStyle );
    int w = TransformWidget::getTransformWidth( _transform, _img );
    int h1 = TransformWidget::getTransformFrontHeight( _transform, _img );
    int h2 = TransformWidget::getTransformBackHeight( _transform, _img );
    int d1 = TransformWidget::getTransformTopRightDiff( _transform, _img );
    int hmax = h1 > h2 ? h1 : h2;
    int hmin = h1 < h2 ? h1 : h2;

    if( h1 == hmax ) {
        int d2 = hmax - hmin - d1;
        _mirroredImg = _mirroredImg.transformed( convertToTransform( w, hmax, hmin, -1 * d2 ), _transformMode );
        p.drawImage( widthDistance, heightDistance + img.height() - d2 - 3, _mirroredImg );
        //p.drawImage(0, img.height() - d2, mirroredImg);
    } else {
        int d2 = hmax - hmin + d1;
        _mirroredImg = _mirroredImg.transformed( convertToTransform( w, hmin, hmax, d2 ), _transformMode );
        p.drawImage( widthDistance, heightDistance + img.height() - d2 - 3, _mirroredImg ); // -3 is Magic number for reflection to stick to image (Quickfix for release)
    }

    //draw new Icon
    if( _isNew ) {
        p.drawImage( widthDistance + img.width() - newIcon.width(), heightDistance - ( int )( 0.5 * newIcon.height() ), newIcon );
    }

    //draw ContentTypeIcons
    if( !_icon.isNull() ) {
        QImage icon = _icon.scaledToWidth( ( int )( contentTypeIconScaleFactor * width() ), Qt::SmoothTransformation );
        p.drawImage( widthDistance + img.width() - icon.width() - 5, heightDistance + img.height() - ( icon.height() + 5 ), icon );
    }

    //draw Overlay if case
    /* if(!_hasMainRegionFocus)
    {
         p.setBrush(QBrush(_noMainRegionFocusPixmap));
         p.drawRect(-1, -1,width()+1,height()+1);
    }
    else
    {*/
    if( !_hasHightLight && !_isSelected ) {
        p.setBrush( QBrush( _noHighLightOverlayPixmap ) );
        p.drawRect( -1, -1, width() + 1, height() + 1 );
    }

    //}
}

QLineF TransformWidget::transform() {
    return QLineF( getTransformWidth( _transform, _img ), getTransformFrontHeight( _transform, _img ),
                   getTransformBackHeight( _transform, _img ), getTransformTopRightDiff( _transform, _img ) );
}

void TransformWidget::setTransform( QLineF newValue ) {
    setTransform( (int) newValue.x1(), (int) newValue.y1(), (int) newValue.x2(), (int) newValue.y2() );
}

void TransformWidget::setTransform( int transformedWidth, int frontHeight, int backHeight, int topRightDiff ) {
    /* SimpleLogger::instance()->debug(Constants::LOGGER_COVERVIEWPLAYLIST,QString("setTransform for Item with title : %1 with params: %2 %3 %4 %5")
                                     .arg(_title).arg(transformedWidth).arg(frontHeight).arg(backHeight).arg(topRightDiff));*/
    int width = transformedWidth;
    int height1 = frontHeight;
    int height2 = backHeight;

    // this all is to keep the aspect ratio of tranformed image ...
    if( !_img.isNull() ) {
        int maxHeight = frontHeight > backHeight ? frontHeight : backHeight;
        int distance = abs( height1 - height2 );
        float widthRatio = ( float )_img.width() / ( float )transformedWidth;
        float heightRatio = ( float )_img.height() / ( float )maxHeight;

        if( widthRatio > heightRatio ) {
            int temp = width * _img.height() / _img.width();

            if( height1 > height2 ) {
                height1 = temp;
                height2 = temp - distance;
            } else {
                height2 = temp;
                height1 = temp - distance;
            }
        } else {
            width = maxHeight * _img.width() / _img.height();
        }
    }

    //  ...this all is to keep the aspect ratio of tranformed image
    //setTransform(convertToTransform(transformedWidth, frontHeight, backHeight, topRightDiff));
    setTransform( convertToTransform( width, height1, height2, topRightDiff ) );
}

void TransformWidget::setTransform( QTransform newValue ) {
    _transform = newValue;
    _transformIsNewIcon = QTransform( 1, newValue.m12(), newValue.m21(), 1, 0, 0 );
    update();
}

QTransform TransformWidget::convertToTransform( int transformedWidth, int frontHeight, int backHeight, int topRightDiff ) {
    qreal m22 = ( qreal )frontHeight / ( qreal )_img.height();
    qreal m13 = ( qreal )( m22 * _img.height() - backHeight ) / ( qreal )( _img.width() * backHeight );
    qreal m12 = ( qreal )topRightDiff * ( m13 * _img.width() + 1 ) / ( qreal )_img.width();
    qreal m11 = ( qreal ) transformedWidth * ( _img.width() * m13 + 1 ) / ( qreal ) _img.width();
    return QTransform( m11, m12, m13, 0, m22, 0, 0, 0 );
}

void TransformWidget::scaleToSize( QSize newSize ) {
    if( !_img.isNull() )
        _img = _img.scaled( newSize.width(), newSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
}


void TransformWidget::setFadeStyle( RedBullPlayer::Widgets::MirrorWidget::FadeStyle style ) {
    _fadeStyle = style;
}


void TransformWidget::setTransformMode( Qt::TransformationMode newMode ) {
    _transformMode = newMode;
}

void TransformWidget::mousePressEvent ( QMouseEvent *  ) {
    emit clicked( _id );
}

void TransformWidget::mouseDoubleClickEvent ( QMouseEvent * ) {
    emit doubleClicked( _id );
}


int TransformWidget::getTransformWidth( QTransform transform, QImage origImg ) {
    return (int) ( transform.m11() * origImg.width() / ( origImg.width() * transform.m13() + 1 ) );
}


int TransformWidget::getTransformFrontHeight( QTransform transform, QImage origImg ) {
    return (int) ( transform.m22() * origImg.height() );
}


int TransformWidget::getTransformBackHeight( QTransform transform, QImage origImg ) {
    return (int) ( transform.m22() * origImg.height() / ( origImg.width() * transform.m13() + 1 ) );
}


int TransformWidget::getTransformTopRightDiff( QTransform transform, QImage origImg ) {
    return (int) ( transform.m12() * origImg.width() / ( origImg.width() * transform.m13() + 1 ) );
}

void TransformWidget::changeMainUIRegionFocusGraphics( bool hasMainUIRegionFocus ) {
    _hasMainRegionFocus = hasMainUIRegionFocus;
    update();
}

void TransformWidget::changeCoverImage( QImage newImage ) {
    _img = newImage.scaled( _img.width(), _img.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
    update();
}

void TransformWidget::enterEvent( QEvent *e ) {
    _hasHightLight = true;
    update();
}

void TransformWidget::leaveEvent( QEvent *e ) {
    _hasHightLight = false;
    update();
}

void TransformWidget::refreshMediaItemMembers( IMedia *mediaItem, bool immediateUpdate ) {
    initMediaItemMembers( mediaItem );
    setupDisplayStrings();
    setToolTip( getToolTip() );

    if( immediateUpdate )
        update();
}

void TransformWidget::setIsNewFlag( bool isNew ) {
    _isNew = isNew;
    update();
}

void TransformWidget::setId( int newId ) {
    _id = newId;
}


QDateTime TransformWidget::createdAt() {
    return _createdAt;
}

bool TransformWidget::isNew() {
    return _isNew;
}

void TransformWidget::select() {
    _isSelected = true;
    update();
}

void TransformWidget::unSelect() {
    _isSelected = false;
    update();
}





