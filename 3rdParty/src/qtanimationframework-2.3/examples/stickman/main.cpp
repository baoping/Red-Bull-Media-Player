/****************************************************************************
**
** This file is part of a Qt Solutions component.
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** 
** Contact:  Qt Software Information (qt-info@nokia.com)
** 
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** 
****************************************************************************/

#include "animation.h"
#include "node.h"
#include "lifecycle.h"
#include "stickman.h"
#include "graphicsview.h"

#include <QtCore>
#include <QtGui>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    StickMan *stickMan = new StickMan;
    stickMan->setDrawSticks(false);

    QGraphicsTextItem *textItem = new QGraphicsTextItem();
    textItem->setHtml("<font color=\"white\"><b>Stickman</b>"
        "<p>"
        "Tell the stickman what to do!"
        "</p>"
        "<p><i>"
        "<li>Press <font color=\"purple\">J</font> to make the stickman jump.</li>"
        "<li>Press <font color=\"purple\">D</font> to make the stickman dance.</li>"
        "<li>Press <font color=\"purple\">C</font> to make him chill out.</li>"
        "<li>Press <font color=\"purple\">Return</font> to make him return to his original position.</li>"
        "<li>When you are done, press <font color=\"purple\">Escape</font>.</li>"
        "</i></p>"
        "<p>If he is unlucky, the stickman will get struck by lightning, and never jump, dance or chill out again."
        "</p></font>");
    qreal w = textItem->boundingRect().width();
    QRectF stickManBoundingRect = stickMan->mapToScene(stickMan->boundingRect()).boundingRect();
    textItem->setPos(-w / 2.0, stickManBoundingRect.bottom() + 25.0);

    QGraphicsScene *scene = new QGraphicsScene();
    scene->addItem(stickMan);
    scene->addItem(textItem);
    scene->setBackgroundBrush(Qt::black);

    GraphicsView *view = new GraphicsView();
    view->setRenderHints(QPainter::Antialiasing);
    view->setTransformationAnchor(QGraphicsView::NoAnchor);
    view->setScene(scene);
    view->showFullScreen();
    view->setFocus();    
    view->setSceneRect(scene->sceneRect());
    
    LifeCycle *cycle = new LifeCycle(stickMan, view);
    cycle->setResetKey(Qt::Key_Return);
    cycle->setDeathAnimation("animations/dead");
    
    cycle->addActivity("animations/jumping", Qt::Key_J);
    cycle->addActivity("animations/dancing", Qt::Key_D);
    cycle->addActivity("animations/chilling", Qt::Key_C);
    cycle->start();

    return app.exec();
}
