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

#include "lifecycle.h"
#include "stickman.h"
#include "node.h"
#include "animation.h"
#include "graphicsview.h"

#include <QtCore>
#include <QtGui>
#if defined(QT_EXPERIMENTAL_SOLUTION)
#include "qtstatemachine.h"
#include "qtstate.h"
#include "qteventtransition.h"
#include "qtsignaltransition.h"
#include "qtsignalevent.h"
#include "qtpropertyanimation.h"
#include "qtparallelanimationgroup.h"
#endif

class KeyPressTransition: public QtSignalTransition
{
public:
    KeyPressTransition(GraphicsView *receiver, Qt::Key key)
        : QtSignalTransition(receiver, SIGNAL(keyPressed(int))), m_key(key)
    {
    }
    KeyPressTransition(GraphicsView *receiver, Qt::Key key, QtAbstractState *target)
        : QtSignalTransition(receiver, SIGNAL(keyPressed(int)), QList<QtAbstractState*>() << target), m_key(key)
    {
    }

    virtual bool eventTest(QEvent *e) const
    {
        if (QtSignalTransition::eventTest(e)) {
            QVariant key = static_cast<QtSignalEvent*>(e)->arguments().at(0);
            return (key.toInt() == int(m_key));
        } 

        return false;
    }
private:
    Qt::Key m_key;
};

class LightningStrikesTransition: public QtEventTransition
{
public:
    LightningStrikesTransition(QtAbstractState *target)
        : QtEventTransition(this, QEvent::Timer, QList<QtAbstractState*>() << target)
    {
        qsrand((uint)QDateTime::currentDateTime().toTime_t());
        startTimer(1000);
    }

    virtual bool eventTest(QEvent *e) const
    {
        return QtEventTransition::eventTest(e) && ((qrand() % 50) == 0);
    }
};

LifeCycle::LifeCycle(StickMan *stickMan, GraphicsView *keyReceiver)
    : m_stickMan(stickMan), m_keyReceiver(keyReceiver)
{
    // Create animation group to be used for all transitions
    m_animationGroup = new QtParallelAnimationGroup();
    const int stickManNodeCount = m_stickMan->nodeCount();
    for (int i=0; i<stickManNodeCount; ++i) {
        QtPropertyAnimation *pa = new QtPropertyAnimation(m_stickMan->node(i), "position");
        m_animationGroup->addAnimation(pa);    
    }

    // Set up intial state graph
    m_machine = new QtStateMachine();
    m_machine->setGlobalRestorePolicy(QtState::RestoreProperties);

    m_alive = new QtState(m_machine->rootState());
    m_alive->setObjectName("alive");
    
    // Make it blink when lightning strikes before entering dead animation
    QtState *lightningBlink = new QtState(m_machine->rootState());    
    lightningBlink->setRestorePolicy(QtState::DoNotRestoreProperties);
    lightningBlink->assignProperty(m_stickMan->scene(), "backgroundBrush", Qt::white);
    lightningBlink->assignProperty(m_stickMan, "penColor", Qt::black);
    lightningBlink->assignProperty(m_stickMan, "fillColor", Qt::white);
    lightningBlink->assignProperty(m_stickMan, "isDead", true);
    
    QTimer *timer = new QTimer(lightningBlink);
    timer->setSingleShot(true);
    timer->setInterval(100);
    lightningBlink->invokeMethodOnEntry(timer, "start");
    lightningBlink->invokeMethodOnExit(timer, "stop");
  
    m_dead = new QtState(m_machine->rootState());
    m_dead->setRestorePolicy(QtState::DoNotRestoreProperties);
    m_dead->assignProperty(m_stickMan->scene(), "backgroundBrush", Qt::black);
    m_dead->assignProperty(m_stickMan, "penColor", Qt::white);
    m_dead->assignProperty(m_stickMan, "fillColor", Qt::black);
    m_dead->setObjectName("dead");
           
    // Idle state (sets no properties)
    m_idle = new QtState(m_alive);
    m_idle->setObjectName("idle");

    m_alive->setInitialState(m_idle);

    // Lightning strikes at random
    m_alive->addTransition(new LightningStrikesTransition(lightningBlink));
    //m_alive->addTransition(new KeyPressTransition(m_keyReceiver, Qt::Key_L, lightningBlink));
    connectByAnimation(lightningBlink, m_dead, new QtSignalTransition(timer, SIGNAL(timeout())));

    m_machine->setInitialState(m_alive);
}

void LifeCycle::setResetKey(Qt::Key resetKey)
{
    // When resetKey is pressed, enter the idle state and do a restoration animation
    // (requires no animation pointer, since no property is being set in the idle state)
    KeyPressTransition *trans = new KeyPressTransition(m_keyReceiver, resetKey, m_idle);
    trans->addAnimation(m_animationGroup);
    m_alive->addTransition(trans);
}

void LifeCycle::setDeathAnimation(const QString &fileName)
{
    QtState *deathAnimation = makeState(m_dead, fileName);
    m_dead->setInitialState(deathAnimation);
}

void LifeCycle::start()
{
    m_machine->start();
}

void LifeCycle::connectByAnimation(QtState *s1, QtAbstractState *s2, 
                                   QtAbstractTransition *transition)
{
    if (transition == 0) {
        transition = s1->addTransition(s2);
    } else {
        transition->setTargetState(s2);
        s1->addTransition(transition);
    }
    transition->addAnimation(m_animationGroup);
}

void LifeCycle::addActivity(const QString &fileName, Qt::Key key)
{
    QtState *state = makeState(m_alive, fileName);
    connectByAnimation(m_alive, state, new KeyPressTransition(m_keyReceiver, key));
}

QtState *LifeCycle::makeState(QtState *parentState, const QString &animationFileName)
{
    QtState *topLevel = new QtState(parentState);
    
    Animation animation;
    {
        QFile file(animationFileName);
        if (file.open(QIODevice::ReadOnly))
            animation.load(&file);
    }

    const int frameCount = animation.totalFrames();
    QtState *previousState = 0;
    for (int i=0; i<frameCount; ++i) {
        QtState *frameState = new QtState(topLevel);
        frameState->setObjectName(QString::fromLatin1("frame %0").arg(i));

        animation.setCurrentFrame(i);
        const int nodeCount = animation.nodeCount();
        for (int j=0; j<nodeCount; ++j)
            frameState->assignProperty(m_stickMan->node(j), "position", animation.nodePos(j));

        if (previousState == 0) {
            topLevel->setInitialState(frameState);
        } else {
            connectByAnimation(previousState, frameState, 
                new QtSignalTransition(m_machine, SIGNAL(animationsFinished())));
        }
        previousState = frameState;
    }

    // Loop
    connectByAnimation(previousState, topLevel->initialState(), 
        new QtSignalTransition(m_machine, SIGNAL(animationsFinished())));

    return topLevel;

}

LifeCycle::~LifeCycle()
{
    delete m_machine;
    delete m_animationGroup;
}
