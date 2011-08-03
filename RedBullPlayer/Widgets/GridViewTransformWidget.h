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
#ifndef GRIDVIEWTRANSFORMWIDGET_H
#define GRIDVIEWTRANSFORMWIDGET_H

#include <QWidget>
#include <QImage>
#include <QDateTime>
#include <QKeyEvent>
#include <QFrame>

//Project includes
#include "../Interfaces/StyleFactory/ITextStyle.h"
#include "../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Widgets {
        class GridViewTransformWidget : public QFrame {
                Q_OBJECT
            public:
                GridViewTransformWidget( QWidget *parent, QImage image, QString mediaType, RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem,
                                         bool isDownload, bool hasMainRegionFocus, bool hideAtStart, int id,
                                         RedBullPlayer::Modules::StyleFactory::ITextStyle *textStyle );

                void scaleToSize( QSize newSize );
                void setTransformMode( Qt::TransformationMode newMode );

                void changeMainUIRegionFocusGraphics( bool hasMainUIRegionFocus );
                void changeCoverImage( QImage newImage );
                void select();
                void unSelect();
                void setIsNewFlag( bool isNew );
                void setId( int newId );
                void refreshMediaItemMembers( RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem, bool immediateUpdate );

                QDateTime createdAt();
                bool isNew();
                QUuid mediaGuid() const {
                    return _mediaGuid;
                }

            signals:
                void clicked( int id );
                void doubleClicked( int id );

            protected:
                void paintEvent( QPaintEvent *e );
                void resizeEvent( QResizeEvent *e );
                void enterEvent( QEvent *e );
                void leaveEvent( QEvent *e );
                void mousePressEvent ( QMouseEvent * event ) ;
                void mouseDoubleClickEvent ( QMouseEvent * event );
                QTransform convertToTransform( int newWidth, int newHeight );
                void setupStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textStyle );
                void setupDisplayStrings();
                void setLengthString();
                void setIsDownloadString();
                void setFileSizeString();
                void setFileNameString();
                void setCreatedAtString();
                QString getToolTip();
                QString getToolTipField( QString fieldContent, QString fieldLabel );
                void initMediaItemMembers( RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem );
                void setupPixmaps( QString mediaType, QImage coverImage );
                void setupCoverImage( QString mediaType, QImage coverImage );
                void setupHighlightPixmap();
                void setupIsNewIcon();
                void setupMainRegionFocusPixmap();
                void setupContentTypeIcon( QString mediaType );
                void setupSelectedBorderPixmaps();
                QPixmap getScaledIsNewIcon( int thumbnailWidth );

            private:
                QUuid _mediaGuid;
                Qt::TransformationMode _transformMode;
                int _id;
                bool _hasHightLight;
                bool _hasMainRegionFocus;
                bool _isSelected;
                QString _mediaType;
                QString _title;
                QString _artist;
                QString _album;
                int _length;
                bool _isNew;
                QString _fileName;
                int _fileSize;
                bool _isDownload;
                QDateTime _createdAt;
                bool _hasDefaultPlaceHolderImage;

                RedBullPlayer::Modules::StyleFactory::ITextStyle *_textStyle;
                QColor _textColor;
                QFont _textFont;

                QPixmap _img;
                QPixmap _icon;
                QPixmap _newIcon;

                QPixmap _noHighLightOverlayPixmap;
                QPixmap _noMainRegionFocusPixmap;
                QPixmap _selectedBorderTopPixmap;
                QPixmap _selectedBorderLeftPixmap;
                QPixmap _selectedBorderBottomPixmap;
                QPixmap _selectedBorderRightPixmap;

                QString _lengthString;
                QString _isDownloadString;
                QString _fileSizeString;
                QString _fileNameString;
                QString _createdAtString;
        };
    }
}

#endif // GRIDVIEWTRANSFORMWIDGET_H
