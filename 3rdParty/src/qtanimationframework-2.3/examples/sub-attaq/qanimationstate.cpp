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



#include "qanimationstate.h"

#if defined(QT_EXPERIMENTAL_SOLUTION)
# include "qtstate.h"
# include "qtstate_p.h"
#else
# include <QtCore/qstate.h>
# include <private/qstate_p.h>
#endif


QT_BEGIN_NAMESPACE

/*!
\class QAnimationState

\brief The QAnimationState class provides state that handle an animation and emit
a signal when this animation is finished.



QAnimationState provides a state that handle an animation. It will start this animation
when the state is entered and stop it when it is leaved. When the animation has finished the
state emit animationFinished signal.
QAnimationState is part of \l{The State Machine Framework}.

\code
QtStateMachine machine;
QAnimationState *s = new QAnimationState(machine->rootState());
QtPropertyAnimation *animation = new QtPropertyAnimation(obj, "pos");
s->setAnimation(animation);
QtState *s2 = new QtState(machine->rootState());
s->addTransition(s, SIGNAL(animationFinished()), s2);
machine.start();
\endcode

\sa QtState, {The Animation Framework}
*/


#ifndef QT_NO_ANIMATION

class QAnimationStatePrivate : public QtStatePrivate
{
    Q_DECLARE_PUBLIC(QAnimationState)
public:
    QAnimationStatePrivate()
        : animation(0)
    {

    }
    ~QAnimationStatePrivate() {}

    QtAbstractAnimation *animation;
};

/*!
  Constructs a new state with the given \a parent state.
*/
QAnimationState::QAnimationState(QtState *parent)
    : QtState(*new QAnimationStatePrivate, parent)
{
}

/*!
  Destroys the animation state.
*/
QAnimationState::~QAnimationState()
{
}

/*!
  Set an \a animation for this QAnimationState. If an animation was previously handle by this
  state then it won't emit animationFinished for the old animation. The QAnimationState doesn't
  take the ownership of the animation.
*/
void QAnimationState::setAnimation(QtAbstractAnimation *animation)
{
    Q_D(QAnimationState);

    if (animation == d->animation)
        return;

    //Disconnect from the previous animation if exist
    if(d->animation)
        disconnect(d->animation, SIGNAL(finished()), this, SIGNAL(animationFinished()));

    d->animation = animation;

    if (d->animation) {
        //connect the new animation
        connect(d->animation, SIGNAL(finished()), this, SIGNAL(animationFinished()));
    }
}

/*!
  Returns the animation handle by this animation state, or 0 if there is no animation.
*/
QtAbstractAnimation* QAnimationState::animation() const
{
    Q_D(const QAnimationState);
    return d->animation;
}

/*!
  \reimp
*/
void QAnimationState::onEntry()
{
    Q_D(QAnimationState);
    if (d->animation)
        d->animation->start();
}

/*!
  \reimp
*/
void QAnimationState::onExit()
{
    Q_D(QAnimationState);
    if (d->animation)
        d->animation->stop();
}

/*!
  \reimp
*/
bool QAnimationState::event(QEvent *e)
{
    return QtState::event(e);
}

QT_END_NAMESPACE

#endif
