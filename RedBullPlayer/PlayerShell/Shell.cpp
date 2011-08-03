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
#include "Shell.h"
#include "ui_Shell.h"

// QT Inluceds
#include <QDebug>
#include <QEvent>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDesktopWidget>

// Project includes
#include "../Container/Container.h"
#include "../Events/EventTypes.h"
#include "../Events/ShellWidgetMovedEvent.h"
#include "../Events/KeyPressedEvent.h"
#include "../EventAggregator/Aggregator.h"
#include "../ToolLib/SimpleLogger.h"
#include "../Container/Constants.h"
#include "../ToolLib/ConfigManager.h"
#include "../ToolLib/SimpleLogger.h"
#include "../Events/Statistics/PlayerStartedStatisticsEntryEvent.h"

using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Events;
using namespace RedBullPlayer::EventAggregator;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;

Shell::Shell( QWidget *parent )
    : QWidget( parent ), ui( new Ui::Shell ) {
    ui->setupUi( this );
    _circleProgress = 0;
    _lodingProgress = 0;
    _error = new Error();

    //Enable/disable Logging
    if ( ConfigManager::instance()->loggingEnabled() )
        SimpleLogger::instance()->enable();

    SimpleLogger::instance()->setStartedMark();

    if ( ConfigManager::instance()->frameless() ) {
        setWindowFlags( Qt::FramelessWindowHint );
    }

    _rootRegion = new RedBullPlayer::PlayerShell::Region( this, "Root", this );
    registerRegion( _rootRegion );
    RedBullPlayer::Container::Container::instance()->setShellObject( this );
    setAttribute( Qt::WA_DeleteOnClose );
    _oldSize = size();
    _isNormalSize = true;
    _oldGeometry = QRect(); // is Null verify in set Normal Size Variant for mac to Resize to Maximal Size on first resize
}

Shell::~Shell() {
    delete ui;
    delete _error;
}

QString Shell::installerStubAbsolutePath() {
    if ( qApp->arguments().count() == 2 ) {
        return qApp->arguments().at( 1 );
    }

    return "";
}

void Shell::setProgressWidget( RedBullPlayer::Widgets::CircleProgress* circleProgress ) {
    _circleProgress = circleProgress;
    setupSplashScreenConnections();
}


void Shell::setupSplashScreenConnections() {
    if ( ! _circleProgress ) {
        return;
    }

    connect( RedBullPlayer::Container::Container::instance(), SIGNAL( startLoading( int ) ),
             this, SLOT( startLoading( int ) ) );
    connect( RedBullPlayer::Container::Container::instance(), SIGNAL( loadingPlugin( QString ) ),
             this, SLOT( loadingPlugin( QString ) ) );
    connect( RedBullPlayer::Container::Container::instance(), SIGNAL( initializingPlugin( QString ) ),
             this, SLOT( initializingPlugin( QString ) ) );
}

void Shell::startLoading( int pluginCount ) {
    _circleProgress->setMinimum( 0 );
    _circleProgress->setMaximum( pluginCount * 2 );
}
void Shell::loadingPlugin( QString pluginName ) {
    _circleProgress->doProgress( ++_lodingProgress, tr( "Loading: %1" ).arg( pluginName ) );
}
void Shell::initializingPlugin( QString pluginName ) {
    _circleProgress->doProgress( ++_lodingProgress, tr( "Initializing: %1" ).arg( pluginName ) );
}

void Shell::Bootstrap() {
    RedBullPlayer::Container::Container::instance()->BootstrapModules();
}

void Shell::registerRegion( RedBullPlayer::PlayerShell::Region* region ) {
    Q_ASSERT( region != 0 );

    if ( ! _regions.contains( region ) &&  findRegion( region->name() ) == 0 ) {
        _regions.append( region );
    }
}

RedBullPlayer::PlayerShell::Region* Shell::findRegion( QString name ) {
    Q_ASSERT( ! name.isNull() );
    Q_ASSERT( ! name.isEmpty() );
    RedBullPlayer::PlayerShell::Region* region;
    foreach( region, _regions ) {
        if ( region->name() == name ) {
            return region;
        }
    }
    return 0;
}

RedBullPlayer::PlayerShell::Region* Shell::RootRegion() {
    return _rootRegion;
}

void Shell::invokeKeyReleaseEvent( QKeyEvent * event ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_SHELL, Q_FUNC_INFO );
    keyReleaseEvent( event );
}

void Shell::moveEvent( QMoveEvent */*e*/ ) {
    ShellWidgetMovedEvent e;
    Aggregator::instance()->sendEvent( &e );
}

void Shell::keyReleaseEvent ( QKeyEvent * event ) {
    SimpleLogger::instance()->debug( Constants::LOGGER_SHELL, Q_FUNC_INFO );
    KeyPressedEvent e( event );
    Aggregator::instance()->sendEvent( &e );
}

void Shell::resizeToNormal() {
    showNormal();

    if( !_oldGeometry.isNull() )
        setGeometry( _oldGeometry );

    _isNormalSize = true;
}

void Shell::resizeToMaximum() {
    _oldSize = size();
    QRect rect = QApplication::desktop()->availableGeometry();
    setGeometry( rect );
    _isNormalSize = false;
}

void Shell::move( const QPoint newPoint ) {
    if( isFullScreen() )
        return;

    QWidget::move( newPoint );
}

void Shell::resize( const QSize newSize ) {
#ifdef Q_OS_LINUX
    setWindowState( Qt::WindowNoState );
#endif
    SimpleLogger::instance()->debug( Constants::LOGGER_SHELL, "SHELL RESIZING CALLED" );
    /*if(isFullScreen())
        resizeToNormal();
    else*/
    QWidget::resize( newSize );
    repaint();
}

void Shell::switchToNormalScreenForResize() {
    SimpleLogger::instance()->debug( Constants::LOGGER_SHELL, "switch normalScreeen for resize called" );

    if( !_isNormalSize ) {
        showNormal();
        setGeometry( QApplication::desktop()->screenGeometry( QApplication::desktop()->screenNumber( this ) ) );
        _isNormalSize = true;
    }
}

void Shell::toggleFullscreen() {
    if( _isNormalSize ) {
        _oldGeometry = geometry();
        showFullScreen();
        _isNormalSize = false;
    } else {
        resizeToNormal();
    }
}

void Shell::toggleSize() {
    if( _isNormalSize ) {
        resizeToMaximum();
    } else {
        resizeToNormal();
    }
}

void Shell::setWindowSize() {
    QString windowState =  ConfigManager::instance()->windowStartState();

    if( windowState == "maximized" ) {
        qDebug() << "showing maximized";
        resizeToMaximum();
        _isNormalSize = true;
    } else {
        if( windowState == "fullscreen" ) {
            qDebug() << "showing fullscreen";
            showFullScreen();
            _isNormalSize = false;
        } else {
            showNormal();
        }
    }
}


