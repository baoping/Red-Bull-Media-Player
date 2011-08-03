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
#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

// Qt Includes
#include <QWidget>
#include <QImage>
#include <QDateTime>
#include <QKeyEvent>

// Project Includes
#include "MirrorWidget.h"
#include "../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Widgets {
        class TransformWidget : public QWidget {
                Q_OBJECT
                Q_PROPERTY( QLineF transform READ transform WRITE setTransform )

            public:
                TransformWidget( QWidget *parent, QImage img, QString mediaType, RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem,
                                 bool isDownload, bool hasMainRegionFocus, bool hideAtStart, int id );

                QLineF transform();
                void setTransform( QLineF newValue );

                void setTransform( QTransform newValue );
                void setTransform( int transformedWidth, int frontHeight, int backHeight, int topRightDiff );

                void scaleToSize( QSize newSize );

                void setFadeStyle( RedBullPlayer::Widgets::MirrorWidget::FadeStyle style );
                void setTransformMode( Qt::TransformationMode newMode );

                static int getTransformWidth( QTransform transform, QImage origImg );
                static int getTransformFrontHeight( QTransform transform, QImage origImg );
                static int getTransformBackHeight( QTransform transform, QImage origImg );
                static int getTransformTopRightDiff( QTransform transform, QImage origImg );

                void changeCoverImage( QImage newImage );
                void setIsNewFlag( bool isNew );
                void setId( int newId );
                void select();
                void unSelect();
                void changeMainUIRegionFocusGraphics( bool hasMainUIRegionFocus );
                void refreshMediaItemMembers( RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem, bool immediateUpdate );

                QUuid mediaGuid() const {
                    return _mediaGuid;
                }

                QDateTime createdAt();
                bool isNew();

            signals:
                void clicked( int id );
                void doubleClicked( int id );

            protected:
                void paintEvent( QPaintEvent *e );
                void mousePressEvent ( QMouseEvent * event ) ;
                void mouseDoubleClickEvent ( QMouseEvent * event );
                QTransform convertToTransform( int transformedWidth, int frontHeight, int backHeight, int topRightDiff );
                void enterEvent( QEvent *e );
                void leaveEvent( QEvent *e );

                void setupPixmaps( QString mediaType, QImage coverImage );
                void setupCoverImage( QString mediaType, QImage coverImage );
                void setupHighlightPixmap();
                void setupIsNewIcon();
                void setupMainRegionFocusPixmap();
                void setupContentTypeIcon( QString mediaType );
                void setupDisplayStrings();
                void setLengthString();
                void setIsDownloadString();
                void setFileSizeString();
                void setFileNameString();
                void setCreatedAtString();
                QString getToolTip();
                QString getToolTipField( QString fieldContent, QString fieldLabel );
                void initMediaItemMembers( RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem );

            private:
                QUuid _mediaGuid;
                QImage _img;
                QImage _mirroredImg;
                QTransform _transform;
                QTransform _transformIsNewIcon;
                RedBullPlayer::Widgets::MirrorWidget::FadeStyle _fadeStyle;
                Qt::TransformationMode _transformMode;

                bool _hasHightLight;
                bool _isSelected;
                bool _hasMainRegionFocus;

                QImage _icon;
                QImage _newIcon;
                QPixmap _noHighLightOverlayPixmap;
                QPixmap _noMainRegionFocusPixmap;

                int _id;
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

                QString _lengthString;
                QString _isDownloadString;
                QString _fileSizeString;
                QString _fileNameString;
                QString _createdAtString;
        };
    }
}

#endif // TRANSFORMWIDGET_H
