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



//Own
#include "graphicsscene.h"
#include "states.h"
#include "boat.h"
#include "submarine.h"
#include "torpedo.h"
#include "bomb.h"
#include "pixmapitem.h"
#include "custompropertyanimation.h"
#include "animationmanager.h"
#include "qanimationstate.h"

//Qt
#if defined(QT_EXPERIMENTAL_SOLUTION)
#include "qtpropertyanimation.h"
#include "qtsequentialanimationgroup.h"
#include "qtparallelanimationgroup.h"
#include "qtstatemachine.h"
#include "qtfinalstate.h"
#include "qtpauseanimation.h"
#else
#include <QtPropertyAnimation>
#include <QtSequentialAnimationGroup>
#include <QtParallelAnimationGroup>
#include <QtStateMachine>
#include <QtFinalState>
#include <QtPauseAnimation>
#endif
#include <QAction>
#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QXmlStreamReader>

//helper function that creates an animation for position and inserts it into group
static CustomPropertyAnimation *addGraphicsItemPosAnimation(QtSequentialAnimationGroup *group,
                                                              QGraphicsItem *item, const QPointF &endPos)
{
    CustomPropertyAnimation *ret = new CustomPropertyAnimation(group);
    ret->setMemberFunctions(item, &QGraphicsItem::pos, &QGraphicsItem::setPos);
    ret->setEndValue(endPos);
    ret->setDuration(200);
    ret->setEasingCurve(QtEasingCurve::OutElastic);
    group->addPause(50);
    return ret;
}

//helper function that creates an animation for opacity and inserts it into group
static void addGraphicsItemFadeoutAnimation(QtAnimationGroup *group, QGraphicsItem *item)
{
#if QT_VERSION >=0x040500
    CustomPropertyAnimation *anim = new CustomPropertyAnimation(group);
    anim->setMemberFunctions(item, &QGraphicsItem::opacity, &QGraphicsItem::setOpacity);
    anim->setDuration(800);
    anim->setEndValue(0);
    anim->setEasingCurve(QtEasingCurve::OutQuad);
#else
    // work around for a bug where we don't transition if the duration is zero.
    QtPauseAnimation *anim = new QtPauseAnimation(group);
    anim->setDuration(1);
#endif
}

GraphicsScene::GraphicsScene(int x, int y, int width, int height, Mode mode)
    : QGraphicsScene(x,y,width,height), mode(mode), newAction(0), quitAction(0), boat(0)
{
    backgroundItem = new PixmapItem(QString("background"),mode);
    backgroundItem->setZValue(1);
    backgroundItem->setPos(0,0);
    addItem(backgroundItem);

    PixmapItem *surfaceItem = new PixmapItem(QString("surface"),mode);
    surfaceItem->setZValue(3);
    surfaceItem->setPos(0,sealLevel() - surfaceItem->boundingRect().height()/2);
    addItem(surfaceItem);

    //parse the xml that contain all data of the game
    QXmlStreamReader reader;
    QFile file(QDir::currentPath() + "/data.xml");
    file.open(QIODevice::ReadOnly);
    reader.setDevice(&file);
    LevelDescription currentLevel;
    while (!reader.atEnd()) {
         reader.readNext();
         if (reader.tokenType() == QXmlStreamReader::StartElement) {
             if (reader.name() == "submarine")
             {
                 SubmarineDescription desc;
                 desc.name = reader.attributes().value("name").toString();
                 desc.points = reader.attributes().value("points").toString().toInt();
                 desc.type = reader.attributes().value("type").toString().toInt();
                 submarinesData.append(desc);
             }
             if (reader.name() == "level")
             {
                 currentLevel.id = reader.attributes().value("id").toString().toInt();
                 currentLevel.name = reader.attributes().value("name").toString();
             }
             if (reader.name() == "subinstance")
             {
                 currentLevel.submarines.append(qMakePair(reader.attributes().value("type").toString().toInt(),reader.attributes().value("nb").toString().toInt()));
             }
         }
         if (reader.tokenType() == QXmlStreamReader::EndElement) {
            if (reader.name() == "level")
            {
                levelsData.insert(currentLevel.id,currentLevel);
                currentLevel.submarines.clear();
            }
         }
   }
}

qreal GraphicsScene::sealLevel() const
{
     if (mode == Big)
        return 220;
     else
        return 160;
}

void GraphicsScene::setupScene(const QList<QAction *> &actions)
{
    newAction = actions.at(0);
    quitAction = actions.at(1);

    QGraphicsItem *logo_s = addWelcomeItem(QPixmap(":/logo-s"));
    QGraphicsItem *logo_u = addWelcomeItem(QPixmap(":/logo-u"));
    QGraphicsItem *logo_b = addWelcomeItem(QPixmap(":/logo-b"));
    QGraphicsItem *logo_dash = addWelcomeItem(QPixmap(":/logo-dash"));
    QGraphicsItem *logo_a = addWelcomeItem(QPixmap(":/logo-a"));
    QGraphicsItem *logo_t = addWelcomeItem(QPixmap(":/logo-t"));
    QGraphicsItem *logo_t2 = addWelcomeItem(QPixmap(":/logo-t2"));
    QGraphicsItem *logo_a2 = addWelcomeItem(QPixmap(":/logo-a2"));
    QGraphicsItem *logo_q = addWelcomeItem(QPixmap(":/logo-q"));
    QGraphicsItem *logo_excl = addWelcomeItem(QPixmap(":/logo-excl"));
    logo_s->setZValue(3);
    logo_u->setZValue(4);
    logo_b->setZValue(5);
    logo_dash->setZValue(6);
    logo_a->setZValue(7);
    logo_t->setZValue(8);
    logo_t2->setZValue(9);
    logo_a2->setZValue(10);
    logo_q->setZValue(11);
    logo_excl->setZValue(12);
    logo_s->setPos(QPointF(-1000, -1000));
    logo_u->setPos(QPointF(-800, -1000));
    logo_b->setPos(QPointF(-600, -1000));
    logo_dash->setPos(QPointF(-400, -1000));
    logo_a->setPos(QPointF(1000, 2000));
    logo_t->setPos(QPointF(800, 2000));
    logo_t2->setPos(QPointF(600, 2000));
    logo_a2->setPos(QPointF(400, 2000));
    logo_q->setPos(QPointF(200, 2000));
    logo_excl->setPos(QPointF(0, 2000));

    QtSequentialAnimationGroup * lettersGroupMoving = new QtSequentialAnimationGroup(this);
    QtParallelAnimationGroup * lettersGroupFading = new QtParallelAnimationGroup(this);

    //creation of the animations for moving letters
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_s, QPointF(300, 150));
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_u, QPointF(350, 150));
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_b, QPointF(400, 120));
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_dash, QPointF(460, 150));
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_a, QPointF(350, 250));
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_t, QPointF(400, 250));
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_t2, QPointF(430, 250));
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_a2, QPointF(465, 250));
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_q, QPointF(510, 250));
    addGraphicsItemPosAnimation(lettersGroupMoving, logo_excl, QPointF(570, 220));

    //creation of the animations for fading out the letters
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_s);
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_u);
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_b);
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_dash);
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_a);
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_t);
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_t2);
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_a2);
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_q);
    addGraphicsItemFadeoutAnimation(lettersGroupFading, logo_excl);
    connect(lettersGroupFading, SIGNAL(finished()), this, SLOT(onIntroAnimationFinished()));

    QtStateMachine *machine = new QtStateMachine(this);

    //This state is when the player is playing
    PlayState *gameState = new PlayState(this,machine->rootState());

    //Final state
    QtFinalState *final = new QtFinalState(machine->rootState());

    //Animation when the player enter in the game
    QAnimationState *lettersMovingState = new QAnimationState(machine->rootState());
    lettersMovingState->setAnimation(lettersGroupMoving);

    //Animation when the welcome screen disappear
    QAnimationState *lettersFadingState = new QAnimationState(machine->rootState());
    lettersFadingState->setAnimation(lettersGroupFading);

    //if new game then we fade out the welcome screen and start playing
    lettersMovingState->addTransition(newAction, SIGNAL(triggered()),lettersFadingState);
    lettersFadingState->addTransition(lettersFadingState, SIGNAL(animationFinished()),gameState);

    //New Game is triggered then player start playing
    gameState->addTransition(newAction, SIGNAL(triggered()),gameState);

    //Wanna quit, then connect to CTRL+Q
    gameState->addTransition(quitAction, SIGNAL(triggered()),final);
    lettersMovingState->addTransition(quitAction, SIGNAL(triggered()),final);

    //Welcome screen is the initial state
    machine->setInitialState(lettersMovingState);

    machine->start();

    //We reach the final state, then we quit
    connect(machine,SIGNAL(finished()),this, SLOT(onQuitGameTriggered()));
}

void GraphicsScene::addItem(Bomb *bomb)
{
    bombs.insert(bomb);
    connect(bomb,SIGNAL(bombExecutionFinished()),this, SLOT(onBombExecutionFinished()));
    QGraphicsScene::addItem(bomb);
}

void GraphicsScene::addItem(Torpedo *torpedo)
{
    torpedos.insert(torpedo);
    connect(torpedo,SIGNAL(torpedoExecutionFinished()),this, SLOT(onTorpedoExecutionFinished()));
    QGraphicsScene::addItem(torpedo);
}

void GraphicsScene::addItem(SubMarine *submarine)
{
    submarines.insert(submarine);
    connect(submarine,SIGNAL(subMarineExecutionFinished()),this, SLOT(onSubMarineExecutionFinished()));
    QGraphicsScene::addItem(submarine);
}

void GraphicsScene::addItem(QGraphicsItem *item)
{
    QGraphicsScene::addItem(item);
}

void GraphicsScene::mousePressEvent (QGraphicsSceneMouseEvent * event)
{
    event->ignore();
}

void GraphicsScene::onQuitGameTriggered()
{
    qApp->closeAllWindows();
}

void GraphicsScene::onBombExecutionFinished()
{
    Bomb *bomb = qobject_cast<Bomb *>(sender());
    bombs.remove(bomb);
    bomb->deleteLater();
    if (boat)
        boat->setBombsLaunched(boat->bombsLaunched() - 1);
}

void GraphicsScene::onTorpedoExecutionFinished()
{
    Torpedo *torpedo = qobject_cast<Torpedo *>(sender());
    torpedos.remove(torpedo);
    torpedo->deleteLater();
}

void GraphicsScene::onSubMarineExecutionFinished()
{
    SubMarine *submarine = qobject_cast<SubMarine *>(sender());
    submarines.remove(submarine);
    if (submarines.count() == 0) {
        emit allSubMarineDestroyed(submarine->points());
    } else {
        emit subMarineDestroyed(submarine->points());
    }
    submarine->deleteLater();
}

int GraphicsScene::remainingSubMarines() const
{
    return submarines.count();
}

void GraphicsScene::clearScene()
{
    foreach (SubMarine *sub,submarines) {
        sub->destroy();
        delete sub;
    }

    foreach (Torpedo *torpedo,torpedos) {
        torpedo->destroy();
        delete torpedo;
    }

    foreach (Bomb *bomb,bombs) {
        bomb->destroy();
        delete bomb;
    }

    submarines.clear();
    bombs.clear();
    torpedos.clear();

    AnimationManager::self()->unregisterAllAnimations();

    if (boat) {
        delete boat;
        boat = 0;
    }
}

QGraphicsPixmapItem *GraphicsScene::addWelcomeItem(const QPixmap &pm)
{
    QGraphicsPixmapItem *item = addPixmap(pm);
    welcomeItems << item;
    return item;
}

void GraphicsScene::onIntroAnimationFinished()
{
    qDeleteAll(welcomeItems);
    welcomeItems.clear();
}

