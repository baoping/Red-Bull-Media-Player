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
#ifndef REDBULLDIALOG_H
#define REDBULLDIALOG_H

#include <QDialog>
#include <QFlags>
#include <QPaintEvent>
#include <QCheckBox>
#include <QHash>

namespace Ui {
    class RedBullDialog;
}
namespace RedBullPlayer {
    namespace Widgets {
        class RedBullDialog : public QDialog {
                Q_OBJECT
            public:
                // FIXME: Reenable Cancel once images are available
                enum Button {
                    Ok = 0x0 /*,
		Cancel = 0x1, */
                };
                Q_DECLARE_FLAGS( Buttons, Button )

                RedBullDialog( QString styleSheet, RedBullPlayer::Widgets::RedBullDialog::Buttons buttons, QWidget *parent = 0 );
                RedBullDialog( QString text, QString styleSheet, RedBullPlayer::Widgets::RedBullDialog::Buttons buttons, QWidget *parent = 0 );
                ~RedBullDialog();

                RedBullDialog::Buttons buttons() const;

                void addLabel( QString text );
                void addWidget( QWidget* widget );
                void addCheckBox( QString referenceName, QString text, bool defaultValue = false );
                void setChecked( QString referenceName, bool value );

                bool getChecked( QString referenceName );

            public slots:
                virtual void reject();
                virtual void accept();

            protected:
                void setButtons( RedBullDialog::Buttons );
                void changeEvent( QEvent *e );
                void paintEvent( QPaintEvent *e );

            private: //functions
                void bootstrap( QString styleSheet, RedBullPlayer::Widgets::RedBullDialog::Buttons buttons );

                void setupButtons();


            private: //data

                QString _styleSheet;

                QHash<QString, QCheckBox*> _checkBoxes;

                RedBullDialog::Buttons _buttons;
                Ui::RedBullDialog *ui;
                QPixmap _backgroundImage;
        };

    }
}
Q_FLAGS( RedBullPlayer::Widgets::RedBullDialog::Buttons )
Q_DECLARE_OPERATORS_FOR_FLAGS( RedBullPlayer::Widgets::RedBullDialog::Buttons )
#endif // REDBULLDIALOG_H
