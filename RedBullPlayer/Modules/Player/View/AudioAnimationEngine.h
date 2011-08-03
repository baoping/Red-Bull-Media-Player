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
#ifndef AUDIOANIMATIONENGINE_H
#define AUDIOANIMATIONENGINE_H

// Qt Includes
#include <QObject>
#include <QLabel>
#include <QTimer>

class QtParallelAnimationGroup;
class QtPropertyAnimation;

class AudioAnimationEngine : public QObject {
        Q_OBJECT

    public:
        AudioAnimationEngine( QWidget* paintSurface, QObject *parent );
        ~AudioAnimationEngine();

        void startAnimation( QPixmap img );
        void stopAnimations();
        void pauseAnimation();
        void unpauseAnimation();

    public slots:
        void timeoutSlot();
        void centeredAnimationFinished();

    protected:
        void clearItems();
        void showEllipseAnimation();
        void showFigure8Animation();
        void showRandomAnimation();
        void showTiledAnimation();
        void showCenteredAnimation();
        void addAnimation( QWidget *item, int x, int y, int index, QtParallelAnimationGroup *group );
        void stopAllAnimations();
        void stopAnimation( QtParallelAnimationGroup *anim );

    protected:
        QList<QLabel *> _items;
        QtParallelAnimationGroup *_ellipseAnim;
        QtParallelAnimationGroup *_figure8Anim;
        QtParallelAnimationGroup *_randomAnim;
        QtParallelAnimationGroup *_tiledAnim;
        QtParallelAnimationGroup *_centeredAnim;
        QtPropertyAnimation *_mainPixmapAnim;
        QTimer *_timer;
        int IMAGE_WIDTH;
        int ITEMS_COUNT;
        bool _active;
        int _lastAnim;
        QLabel *_mainPixmap;
        QWidget* _paintSurface;
};

#endif // AUDIOANIMATIONENGINE_H
