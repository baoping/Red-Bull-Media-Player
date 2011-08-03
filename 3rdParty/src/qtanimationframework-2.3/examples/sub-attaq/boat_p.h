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



#ifndef BOAT_P_H
#define BOAT_P_H

//Own
#include "bomb.h"
#include "graphicsscene.h"

// Qt
#if defined(QT_EXPERIMENTAL_SOLUTION)
# include "qtkeyeventtransition.h"
#else
# include <QtGui/QtKeyEventTransition>
#endif

static const int MAX_BOMB = 5;


//These transtion test if we have to stop the boat (i.e current speed is 1)
class KeyStopTransition : public QtKeyEventTransition
{
public:
    KeyStopTransition(Boat *boat, QEvent::Type type, int key)
    : QtKeyEventTransition(boat,type, key)
    {
        this->boat = boat;
        this->key = key;
#if defined(Q_OS_MAC)
        setModifiers(Qt::KeypadModifier);
#endif
    }
protected:
    virtual bool eventTest(QEvent *event) const
    {
        Q_UNUSED(event);
        if (!QtKeyEventTransition::eventTest(event))
            return false;
        if (boat->currentSpeed() == 1)
            return true;
        else
            return false;
    }
private:
    Boat * boat;
    int key;
};

//These transtion test if we have to move the boat (i.e current speed was 0 or another value)
 class KeyMoveTransition : public QtKeyEventTransition
{
public:
    KeyMoveTransition(Boat *boat, QEvent::Type type, int key)
    : QtKeyEventTransition(boat,type, key)
    {
        this->boat = boat;
        this->key = key;
#if defined(Q_OS_MAC)
        setModifiers(Qt::KeypadModifier);
#endif
    }
protected:
    virtual bool eventTest(QEvent *event) const
    {
        Q_UNUSED(event);
        if (!QtKeyEventTransition::eventTest(event))
            return false;
        if (boat->currentSpeed() >= 0)
            return true;
        else
            return false;

    }
    void onTransition()
    {
        //We decrease the speed if needed
        if (key == Qt::Key_Left && boat->currentDirection() == Boat::Right)
            boat->setCurrentSpeed(boat->currentSpeed() - 1);
        else if (key == Qt::Key_Right && boat->currentDirection() == Boat::Left)
            boat->setCurrentSpeed(boat->currentSpeed() - 1);
        else if (boat->currentSpeed() < 3)
            boat->setCurrentSpeed(boat->currentSpeed() + 1);
        boat->updateBoatMovement();
    }
private:
    Boat * boat;
    int key;
};

//This transition trigger the bombs launch
 class KeyLaunchTransition : public QtKeyEventTransition
{
public:
    KeyLaunchTransition(Boat *boat, QEvent::Type type, int key)
    : QtKeyEventTransition(boat,type, key)
    {
        this->boat = boat;
        this->key = key;
#if defined(Q_OS_MAC)
        setModifiers(Qt::KeypadModifier);
#endif
    }
protected:
    virtual bool eventTest(QEvent *event) const
    {
        Q_UNUSED(event);
        if (!QtKeyEventTransition::eventTest(event))
            return false;
        //We have enough bomb?
        if (boat->bombsLaunched() < MAX_BOMB)
            return true;
        else
            return false;
    }
private:
    Boat * boat;
    int key;
};

//This state is describing when the boat is moving right
class MoveStateRight : public QtState
{
public:
    MoveStateRight(Boat *boat,QtState *parent = 0) : QtState(parent)
    {
        this->boat = boat;
    }
protected:
    void onEntry()
    {
        boat->setCurrentDirection(Boat::Right);
        boat->updateBoatMovement();
    }
private:
    Boat * boat;
};

 //This state is describing when the boat is moving left
class MoveStateLeft : public QtState
{
public:
    MoveStateLeft(Boat *boat,QtState *parent = 0) : QtState(parent)
    {
        this->boat = boat;
    }
protected:
    void onEntry()
    {
        boat->setCurrentDirection(Boat::Left);
        boat->updateBoatMovement();
    }
private:
    Boat * boat;
};

//This state is describing when the boat is in a stand by position
class StopState : public QtState
{
public:
    StopState(Boat *boat,QtState *parent = 0) : QtState(parent)
    {
        this->boat = boat;
    }
protected:
    void onEntry()
    {
        boat->setCurrentSpeed(0);
        boat->setCurrentDirection(Boat::None);
        boat->updateBoatMovement();
    }
private:
    Boat * boat;
};

//This state is describing the launch of the torpedo on the right
class LaunchStateRight : public QtState
{
public:
    LaunchStateRight(Boat *boat,QtState *parent = 0) : QtState(parent)
    {
        this->boat = boat;
    }
protected:
    void onEntry()
    {
        Bomb *b = new Bomb();
        b->setPos(boat->x()+boat->size().width(),boat->y());
        GraphicsScene *scene = static_cast<GraphicsScene *>(boat->scene());
        scene->addItem(b);
        b->launch(Bomb::Right);
        boat->setBombsLaunched(boat->bombsLaunched() + 1);
    }
private:
    Boat * boat;
};

//This state is describing the launch of the torpedo on the left
class LaunchStateLeft : public QtState
{
public:
    LaunchStateLeft(Boat *boat,QtState *parent = 0) : QtState(parent)
    {
        this->boat = boat;
    }
protected:
    void onEntry()
    {
        Bomb *b = new Bomb();
        b->setPos(boat->x() - b->size().width(), boat->y());
        GraphicsScene *scene = static_cast<GraphicsScene *>(boat->scene());
        scene->addItem(b);
        b->launch(Bomb::Left);
        boat->setBombsLaunched(boat->bombsLaunched() + 1);
    }
private:
    Boat * boat;
};

#endif // BOAT_P_H
