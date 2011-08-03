/*
 *  Red Bull Media Player
 *  Copyright (C) 2011, Red Bull
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef AUTOTEST_H
#define AUTOTEST_H

#include <QTest>
#include <QList>
#include <QString>
#include <QStringList>
#include <QSharedPointer>

namespace AutoTest {
    typedef QList<QObject*> TestList;

    inline TestList& testList() {
        static TestList list;
        return list;
    }

    inline bool findObject( QObject* object ) {
        TestList& list = testList();

        if ( list.contains( object ) ) {
            return true;
        }

        foreach ( QObject * test, list ) {
            if ( test->objectName() == object->objectName() ) {
                return true;
            }
        }
        return false;
    }

    inline void addTest( QObject* object ) {
        TestList& list = testList();

        if ( !findObject( object ) ) {
            list.append( object );
        }
    }

    inline int run( int argc, char *argv[] ) {
        int ret = 0;
        foreach ( QObject * test, testList() ) {
            ret += QTest::qExec( test, argc, argv );
        }
        return ret;
    }
}

template <class T>
class Test {
    public:
        QSharedPointer<T> child;

        Test( const QString& name ) : child( new T ) {
            child->setObjectName( name );
            AutoTest::addTest( child.data() );
        }
};

#define DECLARE_TEST(className) static Test<className> t(#className);

#define TEST_MAIN \
int main(int argc, char *argv[]) \
{ \
  QApplication a(argc, argv); \
  return AutoTest::run(argc, argv); \
}

#endif // AUTOTEST_H
