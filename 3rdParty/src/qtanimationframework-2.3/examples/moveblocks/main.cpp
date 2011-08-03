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



#include <QtCore>
#include <QtGui>
#if defined(QT_EXPERIMENTAL_SOLUTION)
#include "qtstatemachine.h"
#include "qtstate.h"
#include "qtabstracttransition.h"
#include "qtpropertyanimation.h"
#include "qtsequentialanimationgroup.h"
#include "qtparallelanimationgroup.h"
#include "qtgraphicswidget.h"
#endif
#include <time.h>

class StateSwitchEvent: public QEvent
{
public:
    StateSwitchEvent()
        : QEvent(Type(StateSwitchType))
    {
    }

    StateSwitchEvent(int rand)
        : QEvent(Type(StateSwitchType)),
          m_rand(rand)
    {
    }

    enum { StateSwitchType = QEvent::User + 256 };

    int rand() const { return m_rand; }

private:
    int m_rand;
};


class QGraphicsRectWidget : public QtGraphicsWidget
{
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
               QWidget *)
    {
        painter->fillRect(rect(), Qt::blue);
    }
};

class StateSwitchTransition: public QtAbstractTransition
{
public:
    StateSwitchTransition(int rand)
        : QtAbstractTransition(),
          m_rand(rand)
    {
    }

protected:
    virtual bool eventTest(QEvent *event) const
    {
        return (event->type() == QEvent::Type(StateSwitchEvent::StateSwitchType))
            && (static_cast<StateSwitchEvent *>(event)->rand() == m_rand);
    }

    virtual void onTransition() {}

private:
    int m_rand;
};

class StateSwitcher : public QtState
{
    Q_OBJECT
public:
    StateSwitcher(QtStateMachine *machine)
        : QtState(machine->rootState()), m_machine(machine),
          m_stateCount(0), m_lastIndex(0)
    { }

    virtual void onEntry()
    {
        int n;
        while ((n = (qrand() % m_stateCount + 1)) == m_lastIndex)
        { }
        m_lastIndex = n;
        m_machine->postEvent(new StateSwitchEvent(n));
    }
    virtual void onExit() {}

    void addState(QtState *state, QtAbstractAnimation *animation) {
        StateSwitchTransition *trans = new StateSwitchTransition(++m_stateCount);
        trans->setTargetState(state);
        addTransition(trans);
        trans->addAnimation(animation);
    }


private:
    QtStateMachine *m_machine;
    int m_stateCount;
    int m_lastIndex;
};

QtState *createGeometryState(QObject *w1, const QRect &rect1,
                            QObject *w2, const QRect &rect2,
                            QObject *w3, const QRect &rect3,
                            QObject *w4, const QRect &rect4,
                            QtState *parent)
{
    QtState *result = new QtState(parent);
    result->assignProperty(w1, "geometry", rect1);
    result->assignProperty(w1, "geometry", rect1);
    result->assignProperty(w2, "geometry", rect2);
    result->assignProperty(w3, "geometry", rect3);
    result->assignProperty(w4, "geometry", rect4);

    return result;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

#if 0
    QWidget window;
    QPalette palette;
    palette.setBrush(QPalette::Window, Qt::black);
    window.setPalette(palette);
    QPushButton *button1 = new QPushButton("A", &window);
    QPushButton *button2 = new QPushButton("B", &window);
    QPushButton *button3 = new QPushButton("C", &window);
    QPushButton *button4 = new QPushButton("D", &window);

    button1->setObjectName("button1");
    button2->setObjectName("button2");
    button3->setObjectName("button3");
    button4->setObjectName("button4");
#else
    QGraphicsRectWidget *button1 = new QGraphicsRectWidget;
    QGraphicsRectWidget *button2 = new QGraphicsRectWidget;
    QGraphicsRectWidget *button3 = new QGraphicsRectWidget;
    QGraphicsRectWidget *button4 = new QGraphicsRectWidget;
    button2->setZValue(1);
    button3->setZValue(2);
    button4->setZValue(3);
    QGraphicsScene scene(0, 0, 300, 300);
    scene.setBackgroundBrush(Qt::black);
    scene.addItem(button1);
    scene.addItem(button2);
    scene.addItem(button3);
    scene.addItem(button4);

    QGraphicsView window(&scene);
    window.setFrameStyle(0);
    window.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    window.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    window.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#endif
    QtStateMachine machine;

    QtState *group = new QtState();
    group->setObjectName("group");
    QTimer timer;
    timer.setInterval(1250);
    timer.setSingleShot(true);
    group->invokeMethodOnEntry(&timer, "start");

    QtState *state1;
    QtState *state2;
    QtState *state3;
    QtState *state4;
    QtState *state5;
    QtState *state6;
    QtState *state7;

    state1 = createGeometryState(button1, QRect(100, 0, 50, 50),
                                 button2, QRect(150, 0, 50, 50),
                                 button3, QRect(200, 0, 50, 50),
                                 button4, QRect(250, 0, 50, 50),
                                 group);
    state2 = createGeometryState(button1, QRect(250, 100, 50, 50),
                                 button2, QRect(250, 150, 50, 50),
                                 button3, QRect(250, 200, 50, 50),
                                 button4, QRect(250, 250, 50, 50),
                                 group);
    state3 = createGeometryState(button1, QRect(150, 250, 50, 50),
                                 button2, QRect(100, 250, 50, 50),
                                 button3, QRect(50, 250, 50, 50),
                                 button4, QRect(0, 250, 50, 50),
                                 group);
    state4 = createGeometryState(button1, QRect(0, 150, 50, 50),
                                 button2, QRect(0, 100, 50, 50),
                                 button3, QRect(0, 50, 50, 50),
                                 button4, QRect(0, 0, 50, 50),
                                 group);
    state5 = createGeometryState(button1, QRect(100, 100, 50, 50),
                                 button2, QRect(150, 100, 50, 50),
                                 button3, QRect(100, 150, 50, 50),
                                 button4, QRect(150, 150, 50, 50),
                                 group);
    state6 = createGeometryState(button1, QRect(50, 50, 50, 50),
                                 button2, QRect(200, 50, 50, 50),
                                 button3, QRect(50, 200, 50, 50),
                                 button4, QRect(200, 200, 50, 50),
                                 group);
    state7 = createGeometryState(button1, QRect(0, 0, 50, 50),
                                 button2, QRect(250, 0, 50, 50),
                                 button3, QRect(0, 250, 50, 50),
                                 button4, QRect(250, 250, 50, 50),
                                 group);
    group->setInitialState(state1);

    QtParallelAnimationGroup animationGroup;
    QtSequentialAnimationGroup *subGroup;

    QtPropertyAnimation *anim = new QtPropertyAnimation(button4, "geometry");
    anim->setDuration(1000);
    anim->setEasingCurve(QtEasingCurve::OutElastic);
    animationGroup.addAnimation(anim);

    subGroup = new QtSequentialAnimationGroup(&animationGroup);
    subGroup->addPause(100);
    anim = new QtPropertyAnimation(button3, "geometry");
    anim->setDuration(1000);
    anim->setEasingCurve(QtEasingCurve::OutElastic);
    subGroup->addAnimation(anim);

    subGroup = new QtSequentialAnimationGroup(&animationGroup);
    subGroup->addPause(150);
    anim = new QtPropertyAnimation(button2, "geometry");
    anim->setDuration(1000);
    anim->setEasingCurve(QtEasingCurve::OutElastic);
    subGroup->addAnimation(anim);

    subGroup = new QtSequentialAnimationGroup(&animationGroup);
    subGroup->addPause(200);
    anim = new QtPropertyAnimation(button1, "geometry");
    anim->setDuration(1000);
    anim->setEasingCurve(QtEasingCurve::OutElastic);
    subGroup->addAnimation(anim);

    StateSwitcher *stateSwitcher = new StateSwitcher(&machine);
    stateSwitcher->setObjectName("stateSwitcher");
    group->addTransition(&timer, SIGNAL(timeout()), stateSwitcher);
    stateSwitcher->addState(state1, &animationGroup);
    stateSwitcher->addState(state2, &animationGroup);
    stateSwitcher->addState(state3, &animationGroup);
    stateSwitcher->addState(state4, &animationGroup);
    stateSwitcher->addState(state5, &animationGroup);
    stateSwitcher->addState(state6, &animationGroup);
    stateSwitcher->addState(state7, &animationGroup);

    machine.addState(group);
    machine.setInitialState(group);
    machine.start();


    window.resize(300, 300);
    window.show();

    qsrand(time(0));

    return app.exec();
}

#include "main.moc"
