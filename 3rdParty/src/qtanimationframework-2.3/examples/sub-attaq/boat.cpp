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
#include "boat.h"
#include "boat_p.h"
#include "bomb.h"
#include "pixmapitem.h"
#include "graphicsscene.h"
#include "animationmanager.h"
#include "custompropertyanimation.h"
#include "qanimationstate.h"

//Qt
#if defined(QT_EXPERIMENTAL_SOLUTION)
# include "qtpropertyanimation.h"
# include "qtstatemachine.h"
# include "qthistorystate.h"
# include "qtfinalstate.h"
# include "qtstate.h"
# include "qtpauseanimation.h"
#include "qtsequentialanimationgroup.h"
#else
#include <QtPropertyAnimation>
#include <QtStateMachine>
#include <QtHistoryState>
#include <QtFinalState>
#include <QtState>
#include <QtSequentialAnimationGroup>
#endif

static QtAbstractAnimation *setupDestroyAnimation(Boat *boat)
{
    QtSequentialAnimationGroup *group = new QtSequentialAnimationGroup(boat);
#if QT_VERSION >=0x040500
    PixmapItem *step1 = new PixmapItem(QString("explosion/boat/step1"),GraphicsScene::Big, boat);
    step1->setZValue(6);
    PixmapItem *step2 = new PixmapItem(QString("explosion/boat/step2"),GraphicsScene::Big, boat);
    step2->setZValue(6);
    PixmapItem *step3 = new PixmapItem(QString("explosion/boat/step3"),GraphicsScene::Big, boat);
    step3->setZValue(6);
    PixmapItem *step4 = new PixmapItem(QString("explosion/boat/step4"),GraphicsScene::Big, boat);
    step4->setZValue(6);
    step1->setOpacity(0);
    step2->setOpacity(0);
    step3->setOpacity(0);
    step4->setOpacity(0);
    CustomPropertyAnimation *anim1 = new CustomPropertyAnimation(boat);
    anim1->setMemberFunctions((QGraphicsItem*)step1, &QGraphicsItem::opacity, &QGraphicsItem::setOpacity);
    anim1->setDuration(100);
    anim1->setEndValue(1);
    CustomPropertyAnimation *anim2 = new CustomPropertyAnimation(boat);
    anim2->setMemberFunctions((QGraphicsItem*)step2, &QGraphicsItem::opacity, &QGraphicsItem::setOpacity);
    anim2->setDuration(100);
    anim2->setEndValue(1);
    CustomPropertyAnimation *anim3 = new CustomPropertyAnimation(boat);
    anim3->setMemberFunctions((QGraphicsItem*)step3, &QGraphicsItem::opacity, &QGraphicsItem::setOpacity);
    anim3->setDuration(100);
    anim3->setEndValue(1);
    CustomPropertyAnimation *anim4 = new CustomPropertyAnimation(boat);
    anim4->setMemberFunctions((QGraphicsItem*)step4, &QGraphicsItem::opacity, &QGraphicsItem::setOpacity);
    anim4->setDuration(100);
    anim4->setEndValue(1);
    group->addAnimation(anim1);
    group->addAnimation(anim2);
    group->addAnimation(anim3);
    group->addAnimation(anim4);
#else
    // work around for a bug where we don't transition if the duration is zero.
    QtPauseAnimation *anim = new QtPauseAnimation(group);
    anim->setDuration(1);
    group->addAnimation(anim);
#endif
    AnimationManager::self()->registerAnimation(group);
    return group;
}



Boat::Boat(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QtGraphicsWidget(parent,wFlags), speed(0), bombsAlreadyLaunched(0), direction(Boat::None), movementAnimation(0)
{
    pixmapItem = new PixmapItem(QString("boat"),GraphicsScene::Big, this);
    setZValue(4);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFlags(QGraphicsItem::ItemIsMovable |  QGraphicsItem::ItemIsFocusable);
    resize(pixmapItem->boundingRect().size());

    //The movement animation used to animate the boat
    movementAnimation = new QtPropertyAnimation(this, "pos");
    AnimationManager::self()->registerAnimation(movementAnimation);

    //We setup the state machien of the boat
    QtStateMachine *machine = new QtStateMachine(this);
    QtState *moving = new QtState(machine->rootState());
    StopState *stopState = new StopState(this,moving);
    machine->setInitialState(moving);
    moving->setInitialState(stopState);
    MoveStateRight *moveStateRight = new MoveStateRight(this,moving);
    MoveStateLeft *moveStateLeft = new MoveStateLeft(this,moving);
    LaunchStateRight *launchStateRight = new LaunchStateRight(this,machine->rootState());
    LaunchStateLeft *launchStateLeft = new LaunchStateLeft(this,machine->rootState());

    //then setup the transitions for the rightMove state
    KeyStopTransition *leftStopRight = new KeyStopTransition(this,QEvent::KeyPress,Qt::Key_Left);
    leftStopRight->setTargetState(stopState);
    KeyMoveTransition *leftMoveRight = new KeyMoveTransition(this,QEvent::KeyPress,Qt::Key_Left);
    leftMoveRight->setTargetState(moveStateRight);
    KeyMoveTransition *rightMoveRight = new KeyMoveTransition(this,QEvent::KeyPress,Qt::Key_Right);
    rightMoveRight->setTargetState(moveStateRight);
    KeyMoveTransition *rightMoveStop = new KeyMoveTransition(this,QEvent::KeyPress,Qt::Key_Right);
    rightMoveStop->setTargetState(moveStateRight);

    //then setup the transitions for the leftMove state
    KeyStopTransition *rightStopLeft = new KeyStopTransition(this,QEvent::KeyPress,Qt::Key_Right);
    rightStopLeft->setTargetState(stopState);
    KeyMoveTransition *rightMoveLeft = new KeyMoveTransition(this,QEvent::KeyPress,Qt::Key_Right);
    rightMoveLeft->setTargetState(moveStateLeft);
    KeyMoveTransition *leftMoveLeft = new KeyMoveTransition(this,QEvent::KeyPress,Qt::Key_Left);
    leftMoveLeft->setTargetState(moveStateLeft);
    KeyMoveTransition *leftMoveStop = new KeyMoveTransition(this,QEvent::KeyPress,Qt::Key_Left);
    leftMoveStop->setTargetState(moveStateLeft);

    //We set up the right move state
    moveStateRight->addTransition(leftStopRight);
    moveStateRight->addTransition(leftMoveRight);
    moveStateRight->addTransition(rightMoveRight);
    stopState->addTransition(rightMoveStop);

    //We set up the left move state
    moveStateLeft->addTransition(rightStopLeft);
    moveStateLeft->addTransition(leftMoveLeft);
    moveStateLeft->addTransition(rightMoveLeft);
    stopState->addTransition(leftMoveStop);

    //The animation is finished, it means we reached the border of the screen, the boat is stopped so we move to the stop state
    moveStateLeft->addTransition(movementAnimation, SIGNAL(finished()), stopState);
    moveStateRight->addTransition(movementAnimation, SIGNAL(finished()), stopState);

    //We set up the keys for dropping bombs
    KeyLaunchTransition *upFireLeft = new KeyLaunchTransition(this,QEvent::KeyPress,Qt::Key_Up);
    upFireLeft->setTargetState(launchStateRight);
    KeyLaunchTransition *upFireRight = new KeyLaunchTransition(this,QEvent::KeyPress,Qt::Key_Up);
    upFireRight->setTargetState(launchStateRight);
    KeyLaunchTransition *upFireStop = new KeyLaunchTransition(this,QEvent::KeyPress,Qt::Key_Up);
    upFireStop->setTargetState(launchStateRight);
    KeyLaunchTransition *downFireLeft = new KeyLaunchTransition(this,QEvent::KeyPress,Qt::Key_Down);
    downFireLeft->setTargetState(launchStateLeft);
    KeyLaunchTransition *downFireRight = new KeyLaunchTransition(this,QEvent::KeyPress,Qt::Key_Down);
    downFireRight->setTargetState(launchStateLeft);
    KeyLaunchTransition *downFireMove = new KeyLaunchTransition(this,QEvent::KeyPress,Qt::Key_Down);
    downFireMove->setTargetState(launchStateLeft);

    //We set up transitions for fire up
    moveStateRight->addTransition(upFireRight);
    moveStateLeft->addTransition(upFireLeft);
    stopState->addTransition(upFireStop);

    //We set up transitions for fire down
    moveStateRight->addTransition(downFireRight);
    moveStateLeft->addTransition(downFireLeft);
    stopState->addTransition(downFireMove);

    //Finally the launch state should come back to its original state
    QtHistoryState *historyState = moving->addHistoryState();
    launchStateLeft->addTransition(historyState);
    launchStateRight->addTransition(historyState);

    QtFinalState *final = new QtFinalState(machine->rootState());

    //This state play the destroyed animation
    QAnimationState *destroyedState = new QAnimationState(machine->rootState());
    destroyedState->setAnimation(setupDestroyAnimation(this));

    //Play a nice animation when the boat is destroyed
    moving->addTransition(this, SIGNAL(boatDestroyed()),destroyedState);

    //Transition to final state when the destroyed animation is finished
    destroyedState->addTransition(destroyedState, SIGNAL(animationFinished()), final);

    //The machine has finished to be executed, then the boat is dead
    connect(machine,SIGNAL(finished()),this, SIGNAL(boatExecutionFinished()));

    machine->start();
}

void Boat::updateBoatMovement()
{
    if (speed == 0 || direction == Boat::None) {
        movementAnimation->stop();
        return;
    }

    movementAnimation->stop();
    movementAnimation->setStartValue(pos());

    if (direction == Boat::Left) {
        movementAnimation->setEndValue(QPointF(0,y()));
        movementAnimation->setDuration(x()/speed*15);
    }
    else /*if (direction == Boat::Right)*/ {
        movementAnimation->setEndValue(QPointF(scene()->width()-size().width(),y()));
        movementAnimation->setDuration((scene()->width()-size().width()-x())/speed*15);
    }
    movementAnimation->start();
}

void Boat::destroy()
{
    movementAnimation->stop();
    emit boatDestroyed();
}

int Boat::bombsLaunched() const
{
    return bombsAlreadyLaunched;
}

void Boat::setBombsLaunched(int number)
{
    if (number > MAX_BOMB) {
        qWarning("Boat::setBombsLaunched : It impossible to launch that number of bombs");
        return;
    }
    bombsAlreadyLaunched = number;
}

int Boat::currentSpeed() const
{
    return speed;
}

void Boat::setCurrentSpeed(int speed)
{
    if (speed > 3 || speed < 0) {
        qWarning("Boat::setCurrentSpeed: The boat can't run on that speed");
        return;
    }
    this->speed = speed;
}

enum Boat::Movement Boat::currentDirection() const
{
    return direction;
}

void Boat::setCurrentDirection(Movement direction)
{
    this->direction = direction;
}

int Boat::type() const
{
    return Type;
}
