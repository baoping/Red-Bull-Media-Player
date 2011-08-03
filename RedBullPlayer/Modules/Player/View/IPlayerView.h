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
#ifndef IPLAYERVIEW_H
#define IPLAYERVIEW_H

#include <QObject>
#include <QStackedWidget>
#include <QWidget>
#include <QLabel>
#include <QSize>

namespace RedBullPlayer {
    namespace Modules {
        namespace Player {
            class IPlayerView : public QStackedWidget  {
                    Q_OBJECT

                public:
                    IPlayerView( QWidget* parent ) : QStackedWidget( parent )  { }
                    virtual ~IPlayerView() {}

                    virtual void showDummyPage() = 0;
                    virtual void showVideoPage() = 0;
                    virtual void showImagePage() = 0;
                    virtual void showAudioPage() = 0;

                    virtual void setImage( QString fileName ) = 0;
                    virtual void clearImage() = 0;

                    virtual void setBufferingAnimation( QString bufferingAnimationFile, int frameCount ) = 0;

                    virtual QWidget* videoPage() const = 0;
                    virtual QWidget* audioPage() const = 0;
                    virtual QWidget* imagePage() const = 0;

                    //for show Cursor before VIdeo Close Hack anti Vlc hides Mouse forever
                    virtual bool isVideoPageDisplayed() = 0;
                signals:
                    void mouseMovedOnPlayerView();

            };
        }
    }
}

#endif // IPLAYERVIEW_H
