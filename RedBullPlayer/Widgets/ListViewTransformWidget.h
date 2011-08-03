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
#ifndef LISTVIEWTRANSFORMWIDGET_H
#define LISTVIEWTRANSFORMWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QKeyEvent>
#include <QPixmap>

//Project includes
#include "../Interfaces/StyleFactory/ITextStyle.h"
#include "../Interfaces/MediaLibrary/IMedia.h"

namespace RedBullPlayer {
    namespace Widgets {
        class ListViewTransformWidget : public QWidget {
                Q_OBJECT
            public:
                ListViewTransformWidget( QWidget *parent, QString mediaType, RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem,
                                         bool isDownload, bool hasMainRegionFocus, int id,
                                         RedBullPlayer::Modules::StyleFactory::ITextStyle *textNormalStyle,
                                         RedBullPlayer::Modules::StyleFactory::ITextStyle *textSelectedStyle );

                void select();
                void unSelect();
                void setIsNewFlag( bool isNew, bool immediateUpdate );
                void setLength( int lengthSeconds, bool immediateUpdate );
                void setId( int newId );
                QDateTime createdAt();
                bool isNew();
                void changeMainUIRegionFocusGraphics( bool hasMainUIRegionFocus );
                void refreshMediaItemMembers( RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem, bool immediateUpdate );
                QUuid mediaGuid() const {
                    return _mediaGuid;
                }

            signals:
                void clicked( int id );
                void doubleClicked( int id );

            protected:
                void paintEvent( QPaintEvent *e );
                void mousePressEvent ( QMouseEvent * event ) ;
                void mouseDoubleClickEvent ( QMouseEvent * event );

                void setLengthString();
                void setIsDownloadString();
                void setFileSizeString();
                void setFileNameString();
                void setCreatedAtString();
                QString getToolTip();
                QString getToolTipField( QString fieldContent, QString fieldLabel );
                void setupStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textNormalStyle,
                                 RedBullPlayer::Modules::StyleFactory::ITextStyle *textSelectedStyle );
                void setupNormalStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textNormalStyle );
                void setupSelectedStyle( RedBullPlayer::Modules::StyleFactory::ITextStyle *textSelectedStyle );
                void setupPixmaps( QString mediaType );
                void setupIsNewIcon();
                void setupMainRegionFocusPixmap();
                void setupContentTypeIcon( QString mediaType );
                void initMediaItemMembers( RedBullPlayer::Modules::MediaLibrary::IMedia *mediaItem );
                void setupDisplayStrings();

            private:
                QUuid _mediaGuid;
                QPixmap _icon;
                QString _mediaType;
                QString _title;
                QString _artist;
                QString _album;
                QDateTime _createdAt;
                int _length;
                bool _isDownload;
                bool _isNew;
                QPixmap _newIcon;
                QString _fileName;
                int _fileSize;
                int _id;
                RedBullPlayer::Modules::StyleFactory::ITextStyle *_textNormalStyle;
                QColor _textNormalColor;
                QFont _textNormalFont;
                RedBullPlayer::Modules::StyleFactory::ITextStyle *_textSelectedStyle;
                QColor _textSelectedColor;
                QFont _textSelectedFont;

                bool _hasMainRegionFocus;
                bool _isSelected;
                QPixmap _noMainRegionFocusPixmap;

                QString _lengthString;
                QString _isDownloadString;
                QString _fileSizeString;
                QString _fileNameString;
                QString _createdAtString;
        };
    }
}

#endif // LISTVIEWTRANSFORMWIDGET_H
