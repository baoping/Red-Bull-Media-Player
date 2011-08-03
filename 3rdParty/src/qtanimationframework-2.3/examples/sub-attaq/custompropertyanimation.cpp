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



#include "custompropertyanimation.h"

// Qt
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE


CustomPropertyAnimation::CustomPropertyAnimation(QObject *parent) :
    QtVariantAnimation(parent), animProp(0)
{
}

CustomPropertyAnimation::~CustomPropertyAnimation()
{
}

void CustomPropertyAnimation::setProperty(AbstractProperty *_animProp)
{
    if (animProp == _animProp)
        return;
    delete animProp;
    animProp = _animProp;
}

/*!
    \reimp
 */
void CustomPropertyAnimation::updateCurrentValue(const QVariant &value)
{
    if (!animProp || state() == QtAbstractAnimation::Stopped)
        return;

    animProp->write(value);
}


/*!
    \reimp
*/
void CustomPropertyAnimation::updateState(QtAbstractAnimation::State oldState, QtAbstractAnimation::State newState)
{
    // Initialize start value
    if (oldState == QtAbstractAnimation::Stopped) {
        if (!animProp)
            return;
        QVariant def = animProp->read();
        if (def.isValid()) {
            const int t = def.userType();
            KeyValues values = keyValues();
            //this ensures that all the keyValues are of type t
            for (int i = 0; i < values.count(); ++i) {
                QtVariantAnimation::KeyValue &pair = values[i];
                if (pair.second.userType() != t)
                    pair.second.convert(static_cast<QVariant::Type>(t));
            }
            //let's now update the key values
            setKeyValues(values);
        }

        if (animProp && !startValue().isValid() && currentTime() == 0
            || (currentTime() == duration() && currentLoop() == (loopCount() - 1))) {
                setStartValue(def);
        }
    }

    QtVariantAnimation::updateState(oldState, newState);
}



#include "moc_custompropertyanimation.cpp"

QT_END_NAMESPACE
