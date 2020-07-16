/********************************************************************************
** Form generated from reading UI file 'PrefDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFDIALOG_H
#define UI_PREFDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>

QT_BEGIN_NAMESPACE

class Ui_PrefDialog
{
public:
    QGroupBox *groupBox;
    QLabel *label_3;
    QLineEdit *edit_OnConnect;
    QLineEdit *edit_OnExit;
    QLabel *label_4;
    QLineEdit *edit_OnSetConfig;
    QLabel *label_5;
    QLineEdit *edit_OnResume;
    QLabel *label_6;
    QLineEdit *edit_OnSuspend;
    QLabel *label_13;
    QLineEdit *edit_OnStart;
    QLabel *label_14;
    QPushButton *okButton;
    QGroupBox *groupBox_3;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLineEdit *edit_NameBtn1;
    QLineEdit *edit_CmdBtn1;
    QLineEdit *edit_CmdBtn2;
    QLineEdit *edit_NameBtn2;
    QLineEdit *edit_CmdBtn3;
    QLineEdit *edit_NameBtn3;
    QLineEdit *edit_CmdBtn4;
    QLineEdit *edit_NameBtn4;
    QLabel *label_9;
    QLabel *label_2;
    QPushButton *cancelButton;
    QGroupBox *groupBox_2;
    QSlider *slider_UserLevel;
    QLabel *label_UserLevel;
    QLabel *label;

    void setupUi(QDialog *PrefDialog)
    {
        if (PrefDialog->objectName().isEmpty())
            PrefDialog->setObjectName(QString::fromUtf8("PrefDialog"));
        PrefDialog->resize(503, 348);
        PrefDialog->setMinimumSize(QSize(503, 348));
        PrefDialog->setMaximumSize(QSize(503, 348));
        PrefDialog->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        groupBox = new QGroupBox(PrefDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 221, 331));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 30, 141, 16));
        QFont font;
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        label_3->setFont(font);
        edit_OnConnect = new QLineEdit(groupBox);
        edit_OnConnect->setObjectName(QString::fromUtf8("edit_OnConnect"));
        edit_OnConnect->setGeometry(QRect(10, 50, 201, 20));
        edit_OnConnect->setFont(font);
        edit_OnExit = new QLineEdit(groupBox);
        edit_OnExit->setObjectName(QString::fromUtf8("edit_OnExit"));
        edit_OnExit->setGeometry(QRect(10, 100, 201, 20));
        edit_OnExit->setFont(font);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 80, 131, 16));
        label_4->setFont(font);
        edit_OnSetConfig = new QLineEdit(groupBox);
        edit_OnSetConfig->setObjectName(QString::fromUtf8("edit_OnSetConfig"));
        edit_OnSetConfig->setGeometry(QRect(10, 150, 201, 20));
        edit_OnSetConfig->setFont(font);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 130, 131, 16));
        label_5->setFont(font);
        edit_OnResume = new QLineEdit(groupBox);
        edit_OnResume->setObjectName(QString::fromUtf8("edit_OnResume"));
        edit_OnResume->setGeometry(QRect(10, 200, 201, 20));
        edit_OnResume->setFont(font);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 180, 131, 16));
        label_6->setFont(font);
        edit_OnSuspend = new QLineEdit(groupBox);
        edit_OnSuspend->setObjectName(QString::fromUtf8("edit_OnSuspend"));
        edit_OnSuspend->setGeometry(QRect(10, 250, 201, 20));
        edit_OnSuspend->setFont(font);
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 230, 131, 16));
        label_13->setFont(font);
        edit_OnStart = new QLineEdit(groupBox);
        edit_OnStart->setObjectName(QString::fromUtf8("edit_OnStart"));
        edit_OnStart->setGeometry(QRect(10, 300, 201, 20));
        edit_OnStart->setFont(font);
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 280, 131, 16));
        label_14->setFont(font);
        okButton = new QPushButton(PrefDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(410, 310, 75, 24));
        okButton->setAutoDefault(false);
        groupBox_3 = new QGroupBox(PrefDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(240, 10, 251, 191));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 42, 61, 16));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 70, 61, 21));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 130, 61, 20));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(70, 20, 41, 16));
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(140, 20, 71, 16));
        edit_NameBtn1 = new QLineEdit(groupBox_3);
        edit_NameBtn1->setObjectName(QString::fromUtf8("edit_NameBtn1"));
        edit_NameBtn1->setGeometry(QRect(70, 40, 61, 20));
        edit_NameBtn1->setFont(font);
        edit_CmdBtn1 = new QLineEdit(groupBox_3);
        edit_CmdBtn1->setObjectName(QString::fromUtf8("edit_CmdBtn1"));
        edit_CmdBtn1->setGeometry(QRect(140, 40, 101, 20));
        edit_CmdBtn1->setFont(font);
        edit_CmdBtn2 = new QLineEdit(groupBox_3);
        edit_CmdBtn2->setObjectName(QString::fromUtf8("edit_CmdBtn2"));
        edit_CmdBtn2->setGeometry(QRect(140, 70, 101, 20));
        edit_CmdBtn2->setFont(font);
        edit_NameBtn2 = new QLineEdit(groupBox_3);
        edit_NameBtn2->setObjectName(QString::fromUtf8("edit_NameBtn2"));
        edit_NameBtn2->setGeometry(QRect(70, 70, 61, 20));
        edit_NameBtn2->setFont(font);
        edit_CmdBtn3 = new QLineEdit(groupBox_3);
        edit_CmdBtn3->setObjectName(QString::fromUtf8("edit_CmdBtn3"));
        edit_CmdBtn3->setGeometry(QRect(140, 100, 101, 20));
        edit_CmdBtn3->setFont(font);
        edit_NameBtn3 = new QLineEdit(groupBox_3);
        edit_NameBtn3->setObjectName(QString::fromUtf8("edit_NameBtn3"));
        edit_NameBtn3->setGeometry(QRect(70, 100, 61, 20));
        edit_NameBtn3->setFont(font);
        edit_CmdBtn4 = new QLineEdit(groupBox_3);
        edit_CmdBtn4->setObjectName(QString::fromUtf8("edit_CmdBtn4"));
        edit_CmdBtn4->setGeometry(QRect(140, 130, 101, 20));
        edit_CmdBtn4->setFont(font);
        edit_NameBtn4 = new QLineEdit(groupBox_3);
        edit_NameBtn4->setObjectName(QString::fromUtf8("edit_NameBtn4"));
        edit_NameBtn4->setGeometry(QRect(70, 130, 61, 20));
        edit_NameBtn4->setFont(font);
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 100, 61, 21));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 160, 231, 21));
        label_2->setWordWrap(true);
        cancelButton = new QPushButton(PrefDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(320, 310, 75, 24));
        cancelButton->setAutoDefault(false);
        groupBox_2 = new QGroupBox(PrefDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(240, 210, 251, 91));
        slider_UserLevel = new QSlider(groupBox_2);
        slider_UserLevel->setObjectName(QString::fromUtf8("slider_UserLevel"));
        slider_UserLevel->setGeometry(QRect(10, 30, 51, 21));
        slider_UserLevel->setMinimum(1);
        slider_UserLevel->setMaximum(3);
        slider_UserLevel->setPageStep(2);
        slider_UserLevel->setOrientation(Qt::Horizontal);
        label_UserLevel = new QLabel(groupBox_2);
        label_UserLevel->setObjectName(QString::fromUtf8("label_UserLevel"));
        label_UserLevel->setGeometry(QRect(0, 60, 71, 20));
        label_UserLevel->setAlignment(Qt::AlignCenter);
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 10, 171, 71));
        label->setWordWrap(true);
        QWidget::setTabOrder(edit_OnConnect, edit_OnExit);
        QWidget::setTabOrder(edit_OnExit, edit_OnSetConfig);
        QWidget::setTabOrder(edit_OnSetConfig, edit_OnResume);
        QWidget::setTabOrder(edit_OnResume, edit_OnSuspend);
        QWidget::setTabOrder(edit_OnSuspend, edit_OnStart);
        QWidget::setTabOrder(edit_OnStart, edit_NameBtn1);
        QWidget::setTabOrder(edit_NameBtn1, edit_CmdBtn1);
        QWidget::setTabOrder(edit_CmdBtn1, edit_NameBtn2);
        QWidget::setTabOrder(edit_NameBtn2, edit_CmdBtn2);
        QWidget::setTabOrder(edit_CmdBtn2, edit_NameBtn3);
        QWidget::setTabOrder(edit_NameBtn3, edit_CmdBtn3);
        QWidget::setTabOrder(edit_CmdBtn3, edit_NameBtn4);
        QWidget::setTabOrder(edit_NameBtn4, edit_CmdBtn4);
        QWidget::setTabOrder(edit_CmdBtn4, slider_UserLevel);
        QWidget::setTabOrder(slider_UserLevel, okButton);

        retranslateUi(PrefDialog);

        okButton->setDefault(true);
        cancelButton->setDefault(false);


        QMetaObject::connectSlotsByName(PrefDialog);
    } // setupUi

    void retranslateUi(QDialog *PrefDialog)
    {
        PrefDialog->setWindowTitle(QApplication::translate("PrefDialog", "Preferences", nullptr));
        groupBox->setTitle(QApplication::translate("PrefDialog", "Script Files", nullptr));
        label_3->setText(QApplication::translate("PrefDialog", "After all modules connected", nullptr));
        label_4->setText(QApplication::translate("PrefDialog", "On Exit of BCI2000", nullptr));
        label_5->setText(QApplication::translate("PrefDialog", "On SetConfig", nullptr));
        label_6->setText(QApplication::translate("PrefDialog", "On Resume", nullptr));
        label_13->setText(QApplication::translate("PrefDialog", "On Suspend", nullptr));
        label_14->setText(QApplication::translate("PrefDialog", "On Start", nullptr));
        okButton->setText(QApplication::translate("PrefDialog", "OK", nullptr));
        groupBox_3->setTitle(QApplication::translate("PrefDialog", "Function Buttons", nullptr));
        label_7->setText(QApplication::translate("PrefDialog", "Button 1", nullptr));
        label_8->setText(QApplication::translate("PrefDialog", "Button 2", nullptr));
        label_10->setText(QApplication::translate("PrefDialog", "Button 4", nullptr));
        label_11->setText(QApplication::translate("PrefDialog", "Name", nullptr));
        label_12->setText(QApplication::translate("PrefDialog", "Command", nullptr));
        label_9->setText(QApplication::translate("PrefDialog", "Button 3", nullptr));
        label_2->setText(QApplication::translate("PrefDialog", "Function buttons are also triggered by pressing F1..F4 on the keyboard.", nullptr));
        cancelButton->setText(QApplication::translate("PrefDialog", "Cancel", nullptr));
        groupBox_2->setTitle(QApplication::translate("PrefDialog", "User Level", nullptr));
        label_UserLevel->setText(QApplication::translate("PrefDialog", "Beginner", nullptr));
        label->setText(QApplication::translate("PrefDialog", "Setting the user level to \"Advanced\" will allow you to specify a user level threshold for display of individual parameters in the configuration dialog.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrefDialog: public Ui_PrefDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFDIALOG_H
