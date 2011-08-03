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
#include "splashscreen.h"
#include "ui_splashscreen.h"

#include <QPixmap>
#include <QTimer>
#include <QDesktopWidget>
#include <QApplication>

#include "../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;

SplashScreen::SplashScreen( QWidget *parent ) :
    QFrame( parent, Qt::SplashScreen | /* Qt::WindowStaysOnTopHint | */ Qt::FramelessWindowHint ),
    m_ui( new Ui::SplashScreen ) {
    m_ui->setupUi( this );
    _progress = new CircleProgress( this, new QPixmap( ":pointImage" ) );
    m_ui->progressLayout->insertWidget( 1, _progress, Qt::AlignHCenter );
    QRect rect = QApplication::desktop()->availableGeometry();
#ifdef SELFEXTRACT // Then there is no config ;)
    setGeometry( rect );
    showFullScreen();
#else

    if( ConfigManager::instance()->windowStartState() == "fullscreen" ) {
        setGeometry( rect );
        showFullScreen();
    } else {
        if( ConfigManager::instance()->windowStartState() == "normal" ) {
            setGeometry( ( rect.width() - 1024 ) / 2, ( rect.height() - 768 ) / 2, 1024, 768 );
            showNormal();
        }
    }

#endif
}

SplashScreen::~SplashScreen() {
    delete m_ui;
}

void SplashScreen::changeEvent( QEvent *e ) {
    QFrame::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

void SplashScreen::initializeProgress( double minimum, double maximum, double step ) {
    _progress->setMinimum( minimum );
    _progress->setMaximum( maximum );

    if ( step != 0 )
        _progress->setStep( step );
}

void SplashScreen::progress( double position, QString text ) {
    _progress->doProgress( position, text );
}

void SplashScreen::doStep() {
    _progress->doStep();
}

void SplashScreen::setProgressText( QString text ) {
    _progress->setText( text );
}
void SplashScreen::setWindowSize() {
#ifdef Q_OS_LINUX
    setWindowFlags( Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
#endif
    QRect rect = QApplication::desktop()->availableGeometry();
#ifdef SELFEXTRACT
    setGeometry( rect );
    showFullScreen();
#else

    if( ConfigManager::instance()->windowStartState() == "fullscreen" ) {
        setGeometry( rect );
        showFullScreen();
    } else {
        if( ConfigManager::instance()->windowStartState() == "normal" ) {
            setGeometry( ( rect.width() - 1024 ) / 2, ( rect.height() - 768 ) / 2, 1024, 768 );
            showNormal();
        }
    }

#endif
}
