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
#ifndef CONTROLLBARUI_H
#define CONTROLLBARUI_H

#include <QtGui/QWidget>
#include <QTimer>

// Project Includes

#include "../../PlayerShell/Shell.h"
#include "../../Widgets/CommandButton.h"
#include "../../Widgets/ToogleButton.h"
#include "../../Interfaces/StyleFactory/IStyleFactory.h"


namespace Ui {
    class ControllBarUI;
}

class ControllBarUI : public QWidget {
        Q_OBJECT
    public:
        ControllBarUI( RedBullPlayer::PlayerShell::Shell* shell,
                       RedBullPlayer::Modules::StyleFactory::IStyleFactory* styleFactory,

                       QWidget *parent = 0 );
        ~ControllBarUI();
        static QString formatTime( qint64 time, QString format );
        static QString getTimeFormat( int time );


        void resetButtons();
        void setupStyle();
        void bufferingAnimationChangedVisibility( bool visible );

        void setMute( bool mute );
        bool mute();

        void setTimeText( qint64 time, qint64 length );
        void setTimeText( QString text );
        void setMediaTitle( QString title, QString artist = "" );

        void showPlayButton();
        void showPauseButton();

        void disableButtons();
        void enableButtons( bool enabled );
        void enableButtonsForTv( bool enabled );
        void enableButtonsForImage( bool enabled );
        void enableButtonsForAudio( bool enabled );

        void enableSnapshotButtons( bool enabled );

        void checkFullScreenButton( bool checked );

        void checkBackwardButton( bool checked );
        void checkForwardButton( bool checked );
        void checkMuteButton( bool checked );


    protected:
        void changeEvent( QEvent *e );
        void paintEvent( QPaintEvent *e );

        void setupButtonIcons();
        void setupSignalConnections();

        void setupTitleStyle();
        void setupTimeDisplayStyle();
        void setupCurrentTimeDisplayStyle();
        void setupTotalTimeDisplayStyle();

    private:
        Ui::ControllBarUI *_ui;

        RedBullPlayer::Widgets::ToogleButton *_backwardButton;
        RedBullPlayer::Widgets::CommandButton *_playButton;
        RedBullPlayer::Widgets::ToogleButton *_forwardButton;
        RedBullPlayer::Widgets::ToogleButton *_previewButton;
        RedBullPlayer::Widgets::CommandButton *_snapshotButton;
        RedBullPlayer::Widgets::ToogleButton *_fullscreenButton;
        RedBullPlayer::Widgets::CommandButton *_resolutionButton;
        RedBullPlayer::Widgets::ToogleButton *_muteButton;

        RedBullPlayer::Modules::StyleFactory::IStyleFactory *_styleFactory;

        QString _spanTotalTimeDisplayStartTag;
        QString _currentMediaTitle;

    signals:
        void action();
        void backwardClicked();
        void playClicked();
        void forwardClicked();
        void previewClicked();
        void fullscreenClicked();
        void snapshotClicked();
        void muteClicked();
        void resolutionClicked();
};

#endif // CONTROLLBARUI_H
