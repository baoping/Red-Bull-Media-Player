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



#ifndef QABSTRACTTRANSITION_P_H
#define QABSTRACTTRANSITION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef QT_STATEMACHINE_SOLUTION
#include <private/qobject_p.h>
#endif

#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

class QtAbstractState;
class QtState;
class QtStateMachine;

class QtAbstractTransition;
class Q_ANIMATION_EXPORT QtAbstractTransitionPrivate
#ifndef QT_STATEMACHINE_SOLUTION
    : public QObjectPrivate
#endif
{
    Q_DECLARE_PUBLIC(QtAbstractTransition)
public:
    QtAbstractTransitionPrivate();

    static QtAbstractTransitionPrivate *get(QtAbstractTransition *q);
    static const QtAbstractTransitionPrivate *get(const QtAbstractTransition *q);

    bool callEventTest(QEvent *e) const;
    void callOnTransition();
    QtState *sourceState() const;
    QtStateMachine *machine() const;

    QList<QtAbstractState*> targetStates;

#ifndef QT_NO_ANIMATION
    QList<QtAbstractAnimation*> animations;
#endif

#ifdef QT_STATEMACHINE_SOLUTION
    QtAbstractTransition *q_ptr;
#endif
};

QT_END_NAMESPACE

#endif
