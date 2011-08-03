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
#include "bomb.h"
#include "submarine.h"
#include "pixmapitem.h"
#include "animationmanager.h"
#include "qanimationstate.h"

//Qt

#if defined(QT_EXPERIMENTAL_SOLUTION)
#include "qtpropertyanimation.h"
#include "qtsequentialanimationgroup.h"
#include "qtstatemachine.h"
#include "qtfinalstate.h"
#else
#include <QtCore/QtSequentialAnimationGroup>
#include <QtCore/QtPropertyAnimation>
#include <QtCore/QtStateMachine>
#include <QtCore/QtFinalState>
#endif

Bomb::Bomb(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QtGraphicsWidget(parent,wFlags), launchAnimation(0)
{
    pixmapItem = new PixmapItem(QString("bomb"),GraphicsScene::Big, this);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFlags(QGraphicsItem::ItemIsMovable);
    setZValue(2);
    resize(pixmapItem->boundingRect().size());
}

void Bomb::launch(Bomb::Direction direction)
{
    launchAnimation = new QtSequentialAnimationGroup();
    AnimationManager::self()->registerAnimation(launchAnimation);
    qreal delta = direction == Right ? 20 : - 20;
    QtPropertyAnimation *anim = new QtPropertyAnimation(this, "pos");
    anim->setEndValue(QPointF(x() + delta,y() - 20));
    anim->setDuration(150);
    launchAnimation->addAnimation(anim);
    anim = new QtPropertyAnimation(this, "pos");
    anim->setEndValue(QPointF(x() + delta*2, y() ));
    anim->setDuration(150);
    launchAnimation->addAnimation(anim);
    anim = new QtPropertyAnimation(this, "pos");
    anim->setEndValue(QPointF(x() + delta*2,scene()->height()));
    anim->setDuration(y()/2*60);
    launchAnimation->addAnimation(anim);
    connect(anim,SIGNAL(valueChanged(const QVariant &)),this,SLOT(onAnimationLaunchValueChanged(const QVariant &)));

    //We setup the state machine of the bomb
    QtStateMachine *machine = new QtStateMachine(this);

    //This state is when the launch animation is playing
    QAnimationState *launched = new QAnimationState(machine->rootState());
    launched->setAnimation(launchAnimation);

    //End
    QtFinalState *final = new QtFinalState(machine->rootState());

    machine->setInitialState(launched);

    //### Add a nice animation when the bomb is destroyed
    launched->addTransition(this, SIGNAL(bombExplosed()),final);

    //If the animation is finished, then we move to the final state
    launched->addTransition(launched, SIGNAL(animationFinished()), final);

    //The machine has finished to be executed, then the boat is dead
    connect(machine,SIGNAL(finished()),this, SIGNAL(bombExecutionFinished()));

    machine->start();

}

void Bomb::onAnimationLaunchValueChanged(const QVariant &)
{
    foreach (QGraphicsItem * item , collidingItems(Qt::IntersectsItemBoundingRect)) {
        if (item->type() == SubMarine::Type) {
            SubMarine *s = static_cast<SubMarine *>(item);
            destroy();
            s->destroy();
        }
    }
}

void Bomb::destroy()
{
    launchAnimation->stop();
    emit bombExplosed();
}
