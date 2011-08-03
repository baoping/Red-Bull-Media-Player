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
#include "ControllBarUI.h"
#include "ui_ControllBarUI.h"

// Qt Includes
#include <QDebug>
#include <QPainter>

// Project Includes

#include "../../Container/Constants.h"
#include "../../Container/Container.h"
#include "../../ToolLib/SimpleLogger.h"
#include "../../Interfaces/StyleFactory/ITextStyle.h"
#include "../../Interfaces/Enumerations/MediaPlayerStateEnum.h"
#include "../../Interfaces/MediaLibrary/IMedia.h"

using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Modules::Player;
using namespace RedBullPlayer::Modules::MediaLibrary;
using namespace RedBullPlayer::PlayerShell;
using namespace RedBullPlayer::Widgets;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;
using namespace RedBullPlayer::Modules::Player;

ControllBarUI::ControllBarUI( Shell* shell,
                              IStyleFactory* styleFactory,
                              QWidget *parent ) :
    QWidget( parent ),
    _ui( new Ui::ControllBarUI ) {
    _ui->setupUi( this );
    setAttribute( Qt::WA_TranslucentBackground, true );
    _styleFactory = styleFactory;
    setupButtonIcons();
    setupSignalConnections();
    _currentMediaTitle = "";
    shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "SeekBarRegion", _ui->_seekBarWidget ) );
    shell->registerRegion( new RedBullPlayer::PlayerShell::Region( this, "VolumeBarRegion", _ui->_volumeBarRegion ) );
}

ControllBarUI::~ControllBarUI() {
    delete _ui;
}

void ControllBarUI::paintEvent( QPaintEvent *e ) {
    QPainter painter( this );
    painter.setBrush( QBrush( QColor( 255, 255, 255, 0 ) ) );
    painter.drawRect( -1, -1, width() + 1, height() + 1 );
}

void ControllBarUI::setTimeText( qint64 time, qint64 length ) {
    if ( ( time == 0 ) && ( length == 0 ) ) {
        _ui->_timeDisplayWidget->setText( "" );
    } else {
        QString format = getTimeFormat( length );
        QString text = QString( "%1 %2/ %3</span>" ).arg( ControllBarUI::formatTime( time, format ) )
                       .arg( _spanTotalTimeDisplayStartTag )
                       .arg( ControllBarUI::formatTime( length, format ) );
        _ui->_timeDisplayWidget->setText( text );
    }
}

void ControllBarUI::setTimeText( QString text ) {
    _ui->_timeDisplayWidget->setText( text );
}

void ControllBarUI::showPlayButton() {
    _playButton->setImage( Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_PLAY );
    _playButton->setMouseOverImage( Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_PLAY );
    _playButton->update();
    SimpleLogger::instance()->debug(Constants::LOGGER_CONTROLBAR,"showPlaybuttonCalled");
}

void ControllBarUI::showPauseButton() {
    _playButton->setImage( Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_PAUSE );
    _playButton->setMouseOverImage( Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_PAUSE );
    _playButton->update();
    SimpleLogger::instance()->debug(Constants::LOGGER_CONTROLBAR,"showPausebuttonCalled");
}

void ControllBarUI::setMediaTitle( QString title, QString artist ) {
    QString t = "";

    if ( !artist.isNull() && !artist.isEmpty() ) {
        t = QString( "%1 - %2" ).arg( artist ).arg( title );
    } else {
        t = QString( "%1" ).arg( title );
    }

    QFontMetrics fm( _ui->_titleLabel->font() );
    _currentMediaTitle = fm.elidedText( t, Qt::ElideRight, _ui->_titleLabel->width() );

    if( _ui->_titleLabel->text() != Constants::CONTROLL_BAR_BUFFERING_TITLE ) {
        _ui->_titleLabel->setText( _currentMediaTitle );
    }
}

void ControllBarUI::enableSnapshotButtons( bool enabled ) {
    _snapshotButton->setEnabled( enabled );
    _resolutionButton->setEnabled( enabled );
}


bool ControllBarUI::mute() {
    return _muteButton->isChecked();
}

void ControllBarUI::setMute( bool mute ) {
    _muteButton->setChecked( mute );

    if( mute )
        _muteButton->setMouseOverImageForToggledHover( Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_VOLUME );
    else
        _muteButton->setMouseOverImageForToggledHover( Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_VOLUME );
}


void ControllBarUI::setupButtonIcons() {
    QBoxLayout *layout = qobject_cast<QBoxLayout*>( _ui->_buttonContainer->layout() );

    if( layout == NULL ) {
        Error error;
        error.setErrorMessage( tr( "Cannot display controll bar buttons because buttonContainer layout cannot be cast to QBoxLayout" ), Error::Fatal );
        Error::raise( &error );
        return;
    }

    QBoxLayout *layoutMuteButtons = qobject_cast<QBoxLayout*>( _ui->_muteButtonRegion->layout() );

    if( layoutMuteButtons == NULL ) {
        Error error;
        error.setErrorMessage( tr( "Cannot display mute buttons because mute buttonsRegin layout cannot be cast to QBoxLayout" ), Error::Fatal );
        Error::raise( &error );
        return;
    }

    _backwardButton = new ToogleButton( _ui->_buttonContainer,
                                        Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_BACKWARD,
                                        Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_BACKWARD );
    _backwardButton->setDisabledImage( Constants::CONTROLL_BAR_IMAGE_DIR + "disabled" + Constants::CONTROLL_BAR_BACKWARD );
    layout->insertWidget( 0, _backwardButton );

    _playButton = new CommandButton( _ui->_buttonContainer,
                                     Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_PLAY );
    _playButton->setMouseOverImage( Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_PLAY );
    _playButton->setDisabledImage( Constants::CONTROLL_BAR_IMAGE_DIR + "disabled" + Constants::CONTROLL_BAR_PLAY );
    layout->insertWidget( 1, _playButton );

    _forwardButton = new ToogleButton( _ui->_buttonContainer,
                                       Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_FORWARD,
                                       Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_FORWARD );
    _forwardButton->setDisabledImage( Constants::CONTROLL_BAR_IMAGE_DIR + "disabled" + Constants::CONTROLL_BAR_FORWARD );
    layout->insertWidget( 2, _forwardButton );

    _previewButton = new ToogleButton( _ui->_buttonContainer,
                                       Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_PREVIEW,
                                       Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_PREVIEW );
    _previewButton->setDisabledImage( Constants::CONTROLL_BAR_IMAGE_DIR + "disabled" + Constants::CONTROLL_BAR_PREVIEW );
    layout->insertWidget( 4, _previewButton );

    _snapshotButton = new CommandButton( _ui->_buttonContainer,
                                         Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_SNAPSHOT );
    _snapshotButton->setMouseOverImage( Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_SNAPSHOT );
    _snapshotButton->setDisabledImage( Constants::CONTROLL_BAR_IMAGE_DIR + "disabled" + Constants::CONTROLL_BAR_SNAPSHOT );
    layout->insertWidget( 5, _snapshotButton );

    _fullscreenButton = new ToogleButton( _ui->_buttonContainer,
                                          Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_FULLSCREEN,
                                          Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_FULLSCREEN );
    _fullscreenButton->setDisabledImage( Constants::CONTROLL_BAR_IMAGE_DIR + "disabled" + Constants::CONTROLL_BAR_FULLSCREEN );
    layout->insertWidget( 6, _fullscreenButton );

    _resolutionButton = new CommandButton( _ui->_buttonContainer,
                                           Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_RESOLUTION );
    _resolutionButton->setMouseOverImage( Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_RESOLUTION );
    _resolutionButton->setDisabledImage( Constants::CONTROLL_BAR_IMAGE_DIR + "disabled" + Constants::CONTROLL_BAR_RESOLUTION );
    layout->insertWidget( 7, _resolutionButton );

    _muteButton = new ToogleButton( _ui->_buttonContainer,
                                    Constants::CONTROLL_BAR_IMAGE_DIR + Constants::CONTROLL_BAR_VOLUME,
                                    Constants::CONTROLL_BAR_IMAGE_DIR + "active" + Constants::CONTROLL_BAR_VOLUME );
    _muteButton->setDisabledImage( Constants::CONTROLL_BAR_IMAGE_DIR + "disabled" + Constants::CONTROLL_BAR_VOLUME );
    layoutMuteButtons->insertWidget( 0, _muteButton );
}

void ControllBarUI::setupSignalConnections() {
    connect( _backwardButton, SIGNAL( clicked() ), this, SIGNAL( backwardClicked() ) );
    connect( _backwardButton, SIGNAL( clicked() ), this, SIGNAL( action() ) );
    connect( _playButton, SIGNAL( clicked() ), this, SIGNAL( playClicked() ) );
    connect( _playButton, SIGNAL( clicked() ), this, SIGNAL( action() ) );
    connect( _forwardButton, SIGNAL( clicked() ), this, SIGNAL( forwardClicked() ) );
    connect( _forwardButton, SIGNAL( clicked() ), this, SIGNAL( action() ) );
    connect( _previewButton, SIGNAL( clicked() ), this, SIGNAL( previewClicked() ) );
    connect( _previewButton, SIGNAL( clicked() ), this, SIGNAL( action() ) );
    connect( _snapshotButton, SIGNAL( clicked() ), this, SIGNAL( snapshotClicked() ) );
    connect( _snapshotButton, SIGNAL( clicked() ), this, SIGNAL( action() ) );
    connect( _fullscreenButton, SIGNAL( clicked() ), this, SIGNAL( fullscreenClicked() ) );
    connect( _fullscreenButton, SIGNAL( clicked() ), this, SIGNAL( action() ) );
    connect( _resolutionButton, SIGNAL( clicked() ), this, SIGNAL( resolutionClicked() ) );
    connect( _resolutionButton, SIGNAL( clicked() ), this, SIGNAL( action() ) );
    connect( _muteButton, SIGNAL( clicked() ), this, SIGNAL( muteClicked() ) );
    connect( _muteButton, SIGNAL( clicked() ), this, SIGNAL( action() ) );
}

void ControllBarUI::checkFullScreenButton( bool checked ) {
    _fullscreenButton->setChecked( checked );
}

void ControllBarUI::checkBackwardButton( bool checked ) {
    _backwardButton->setChecked( checked );
}

void ControllBarUI::checkForwardButton( bool checked ) {
    _forwardButton->setChecked( checked );
}

void ControllBarUI::checkMuteButton( bool checked ) {
    _muteButton->setChecked( checked );
}

void ControllBarUI::bufferingAnimationChangedVisibility( bool visible ) {
    if( visible ) {
        _ui->_titleLabel->setText( Constants::CONTROLL_BAR_BUFFERING_TITLE );
        _ui->_timeDisplayWidget->clear();
    } else {
        _ui->_titleLabel->setText( _currentMediaTitle );
    }
}

void ControllBarUI::changeEvent( QEvent *e ) {
    QWidget::changeEvent( e );

    switch ( e->type() ) {
        case QEvent::LanguageChange:
            _ui->retranslateUi( this );
            break;

        default:
            break;
    }
}

QString ControllBarUI::formatTime( qint64 time, QString format ) {
    int sec = time / 1000;
    int min = sec / 60;
    int hour = min / 60;
    int msec = time;
    QTime qTime( hour % 24, min % 60, sec % 60, msec % 1000 );
    return qTime.toString( format );
}

void ControllBarUI::disableButtons(){
    _backwardButton->setEnabled( false );
    _playButton->setEnabled( false );
    _forwardButton->setEnabled( false );
    _previewButton->setEnabled( false );
    _snapshotButton->setEnabled( false );
    _fullscreenButton->setEnabled( false );
    _resolutionButton->setEnabled( false );
    _muteButton->setEnabled( false );
}

void ControllBarUI::enableButtons( bool enabled ) {
    _backwardButton->setEnabled( enabled );
    _playButton->setEnabled( enabled );
    _forwardButton->setEnabled( enabled );
    _previewButton->setEnabled( enabled );
    _snapshotButton->setEnabled( enabled );
    _fullscreenButton->setEnabled( enabled );
    _resolutionButton->setEnabled( enabled );
    _muteButton->setEnabled( enabled );
}

void ControllBarUI::enableButtonsForTv( bool enabled ) {
    _playButton->setEnabled( enabled );
    _snapshotButton->setEnabled( !enabled );
    _fullscreenButton->setEnabled( enabled );
    _resolutionButton->setEnabled( !enabled );
    _muteButton->setEnabled( enabled );
}

void ControllBarUI::enableButtonsForImage( bool enabled ) {
    _fullscreenButton->setEnabled( enabled );
}

void ControllBarUI::enableButtonsForAudio( bool enabled ) {
    _backwardButton->setEnabled( enabled );
    _playButton->setEnabled( enabled );
    _forwardButton->setEnabled( enabled );
    _fullscreenButton->setEnabled( enabled );
    _muteButton->setEnabled( enabled );
}

QString ControllBarUI::getTimeFormat( int time ) {
    int hours = time / 3600000;
    QString retVal = "mm:ss";

    if( hours > 0 ) {
        retVal = "h:mm:ss";
    }

    return retVal;
}

void ControllBarUI::resetButtons() {
    _previewButton->setChecked( false );
}

void ControllBarUI::setupStyle() {
    setupTitleStyle();
    setupTimeDisplayStyle();
}

void ControllBarUI::setupTitleStyle() {
    bool ok;
    ITextStyle *titleTextStyle = _styleFactory->getTextStyle( "ControlBar_HeaderText", &ok );

    if( ok ) {
        QString titleStyle = QString( "background-color:transparent;" );
        titleStyle += titleTextStyle->styleSheet();
        _ui->_titleLabel->setStyleSheet( titleStyle );
        _ui->_titleLabel->setAlignment( titleTextStyle->alignment() );
    }
}

void ControllBarUI::setupTimeDisplayStyle() {
    setupCurrentTimeDisplayStyle();
    setupTotalTimeDisplayStyle();
}

void ControllBarUI::setupCurrentTimeDisplayStyle() {
    bool ok;
    ITextStyle *timeDisplayTextStyle = _styleFactory->getTextStyle( "CtrlBar_TimeLine_Text_aktiv", &ok );

    if( ok ) {
        QString timeDisplayStyle = QString( "background-color:transparent;" );
        timeDisplayStyle += timeDisplayTextStyle->styleSheet();
        _ui->_timeDisplayWidget->setStyleSheet( timeDisplayStyle );
        _ui->_timeDisplayWidget->setAlignment( timeDisplayTextStyle->alignment() );
    }
}

void ControllBarUI::setupTotalTimeDisplayStyle() {
    _spanTotalTimeDisplayStartTag = "<span>";
    bool ok;
    ITextStyle *totalTimeDisplayTextStyle = _styleFactory->getTextStyle( "CtrlBar_TimeLine_Text_gesamt", &ok );

    if( ok ) {
        _spanTotalTimeDisplayStartTag = "<span style=\"";
        _spanTotalTimeDisplayStartTag += ( totalTimeDisplayTextStyle->styleSheet() + "\">" );
    }
}
