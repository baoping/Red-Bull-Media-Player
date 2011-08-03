/********************************************************************************
** Form generated from reading ui file 'form.ui'
**
** Created: Thu Oct 22 17:52:09 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout;
    QListWidget *easingCurvePicker;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *lineRadio;
    QRadioButton *circleRadio;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label;
    QDoubleSpinBox *periodSpinBox;
    QLabel *label_2;
    QDoubleSpinBox *amplitudeSpinBox;
    QLabel *label_3;
    QDoubleSpinBox *overshootSpinBox;
    QSpacerItem *verticalSpacer;
    QGraphicsView *graphicsView;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(545, 471);
        gridLayout = new QGridLayout(Form);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        easingCurvePicker = new QListWidget(Form);
        easingCurvePicker->setObjectName(QString::fromUtf8("easingCurvePicker"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(easingCurvePicker->sizePolicy().hasHeightForWidth());
        easingCurvePicker->setSizePolicy(sizePolicy);
        easingCurvePicker->setMaximumSize(QSize(16777215, 120));
        easingCurvePicker->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        easingCurvePicker->setMovement(QListView::Static);
        easingCurvePicker->setProperty("isWrapping", QVariant(false));
        easingCurvePicker->setViewMode(QListView::IconMode);
        easingCurvePicker->setSelectionRectVisible(false);

        gridLayout->addWidget(easingCurvePicker, 0, 0, 1, 2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(Form);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lineRadio = new QRadioButton(groupBox_2);
        buttonGroup = new QButtonGroup(Form);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(lineRadio);
        lineRadio->setObjectName(QString::fromUtf8("lineRadio"));
        lineRadio->setChecked(true);

        verticalLayout_2->addWidget(lineRadio);

        circleRadio = new QRadioButton(groupBox_2);
        buttonGroup->addButton(circleRadio);
        circleRadio->setObjectName(QString::fromUtf8("circleRadio"));

        verticalLayout_2->addWidget(circleRadio);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(Form);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        periodSpinBox = new QDoubleSpinBox(groupBox);
        periodSpinBox->setObjectName(QString::fromUtf8("periodSpinBox"));
        periodSpinBox->setEnabled(false);
        periodSpinBox->setMinimum(-1);
        periodSpinBox->setSingleStep(0.1);
        periodSpinBox->setValue(-1);

        formLayout->setWidget(0, QFormLayout::FieldRole, periodSpinBox);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        amplitudeSpinBox = new QDoubleSpinBox(groupBox);
        amplitudeSpinBox->setObjectName(QString::fromUtf8("amplitudeSpinBox"));
        amplitudeSpinBox->setEnabled(false);
        amplitudeSpinBox->setMinimum(-1);
        amplitudeSpinBox->setSingleStep(0.1);
        amplitudeSpinBox->setValue(-1);

        formLayout->setWidget(1, QFormLayout::FieldRole, amplitudeSpinBox);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        overshootSpinBox = new QDoubleSpinBox(groupBox);
        overshootSpinBox->setObjectName(QString::fromUtf8("overshootSpinBox"));
        overshootSpinBox->setEnabled(false);
        overshootSpinBox->setMinimum(-1);
        overshootSpinBox->setSingleStep(0.1);
        overshootSpinBox->setValue(-1);

        formLayout->setWidget(2, QFormLayout::FieldRole, overshootSpinBox);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

        graphicsView = new QGraphicsView(Form);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(graphicsView, 1, 1, 1, 1);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Easing curves", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Form", "Path type", 0, QApplication::UnicodeUTF8));
        lineRadio->setText(QApplication::translate("Form", "Line", 0, QApplication::UnicodeUTF8));
        circleRadio->setText(QApplication::translate("Form", "Circle", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Form", "Properties", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form", "Period", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Form", "Amplitude", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Form", "Overshoot", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
