/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionPreferences;
    QAction *actionQuit;
    QAction *actionStates;
    QAction *actionOperator_Log;
    QAction *actionConnection_Info;
    QAction *actionBCI2000_Help;
    QAction *actionAbout;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_Btn1;
    QPushButton *pushButton_Btn2;
    QPushButton *pushButton_Btn3;
    QPushButton *pushButton_Btn4;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_Config;
    QLabel *arrow_1;
    QPushButton *pushButton_SetConfig;
    QLabel *arrow_2;
    QPushButton *pushButton_RunSystem;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_Quit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(525, 150);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(525, 125));
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionStates = new QAction(MainWindow);
        actionStates->setObjectName(QString::fromUtf8("actionStates"));
        actionOperator_Log = new QAction(MainWindow);
        actionOperator_Log->setObjectName(QString::fromUtf8("actionOperator_Log"));
        actionOperator_Log->setCheckable(true);
        actionConnection_Info = new QAction(MainWindow);
        actionConnection_Info->setObjectName(QString::fromUtf8("actionConnection_Info"));
        actionConnection_Info->setCheckable(true);
        actionBCI2000_Help = new QAction(MainWindow);
        actionBCI2000_Help->setObjectName(QString::fromUtf8("actionBCI2000_Help"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_Btn1 = new QPushButton(centralWidget);
        pushButton_Btn1->setObjectName(QString::fromUtf8("pushButton_Btn1"));
        pushButton_Btn1->setEnabled(false);

        horizontalLayout->addWidget(pushButton_Btn1);

        pushButton_Btn2 = new QPushButton(centralWidget);
        pushButton_Btn2->setObjectName(QString::fromUtf8("pushButton_Btn2"));
        pushButton_Btn2->setEnabled(false);

        horizontalLayout->addWidget(pushButton_Btn2);

        pushButton_Btn3 = new QPushButton(centralWidget);
        pushButton_Btn3->setObjectName(QString::fromUtf8("pushButton_Btn3"));
        pushButton_Btn3->setEnabled(false);

        horizontalLayout->addWidget(pushButton_Btn3);

        pushButton_Btn4 = new QPushButton(centralWidget);
        pushButton_Btn4->setObjectName(QString::fromUtf8("pushButton_Btn4"));
        pushButton_Btn4->setEnabled(false);

        horizontalLayout->addWidget(pushButton_Btn4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_Config = new QPushButton(centralWidget);
        pushButton_Config->setObjectName(QString::fromUtf8("pushButton_Config"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_Config->sizePolicy().hasHeightForWidth());
        pushButton_Config->setSizePolicy(sizePolicy1);
        pushButton_Config->setMinimumSize(QSize(90, 35));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        pushButton_Config->setFont(font);

        horizontalLayout_2->addWidget(pushButton_Config);

        arrow_1 = new QLabel(centralWidget);
        arrow_1->setObjectName(QString::fromUtf8("arrow_1"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(arrow_1->sizePolicy().hasHeightForWidth());
        arrow_1->setSizePolicy(sizePolicy2);
        QFont font1;
        font1.setPointSize(12);
        arrow_1->setFont(font1);

        horizontalLayout_2->addWidget(arrow_1);

        pushButton_SetConfig = new QPushButton(centralWidget);
        pushButton_SetConfig->setObjectName(QString::fromUtf8("pushButton_SetConfig"));
        sizePolicy1.setHeightForWidth(pushButton_SetConfig->sizePolicy().hasHeightForWidth());
        pushButton_SetConfig->setSizePolicy(sizePolicy1);
        pushButton_SetConfig->setMinimumSize(QSize(100, 35));
        pushButton_SetConfig->setFont(font);

        horizontalLayout_2->addWidget(pushButton_SetConfig);

        arrow_2 = new QLabel(centralWidget);
        arrow_2->setObjectName(QString::fromUtf8("arrow_2"));
        sizePolicy2.setHeightForWidth(arrow_2->sizePolicy().hasHeightForWidth());
        arrow_2->setSizePolicy(sizePolicy2);
        arrow_2->setFont(font1);

        horizontalLayout_2->addWidget(arrow_2);

        pushButton_RunSystem = new QPushButton(centralWidget);
        pushButton_RunSystem->setObjectName(QString::fromUtf8("pushButton_RunSystem"));
        sizePolicy1.setHeightForWidth(pushButton_RunSystem->sizePolicy().hasHeightForWidth());
        pushButton_RunSystem->setSizePolicy(sizePolicy1);
        pushButton_RunSystem->setMinimumSize(QSize(90, 35));
        pushButton_RunSystem->setFont(font);

        horizontalLayout_2->addWidget(pushButton_RunSystem);

        horizontalSpacer_2 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_Quit = new QPushButton(centralWidget);
        pushButton_Quit->setObjectName(QString::fromUtf8("pushButton_Quit"));
        sizePolicy1.setHeightForWidth(pushButton_Quit->sizePolicy().hasHeightForWidth());
        pushButton_Quit->setSizePolicy(sizePolicy1);
        pushButton_Quit->setMinimumSize(QSize(90, 35));
        pushButton_Quit->setFont(font);

        horizontalLayout_2->addWidget(pushButton_Quit);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalLayout->setStretch(1, 1);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 525, 18));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(false);
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionPreferences);
        menuFile->addAction(actionQuit);
        menuView->addAction(actionConnection_Info);
        menuView->addAction(actionStates);
        menuWindow->addAction(actionOperator_Log);
        menuHelp->addAction(actionBCI2000_Help);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        pushButton_Config->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "BCI2000/Operator", nullptr));
        actionPreferences->setText(QApplication::translate("MainWindow", "Preferences...", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionStates->setText(QApplication::translate("MainWindow", "States and Events...", nullptr));
        actionOperator_Log->setText(QApplication::translate("MainWindow", "Operator Log", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOperator_Log->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", nullptr));
#endif // QT_NO_SHORTCUT
        actionConnection_Info->setText(QApplication::translate("MainWindow", "Connection Info", nullptr));
        actionBCI2000_Help->setText(QApplication::translate("MainWindow", "BCI2000 Help", nullptr));
        actionAbout->setText(QApplication::translate("MainWindow", "About ...", nullptr));
        pushButton_Btn1->setText(QApplication::translate("MainWindow", "Button 1", nullptr));
        pushButton_Btn2->setText(QApplication::translate("MainWindow", "Button 2", nullptr));
        pushButton_Btn3->setText(QApplication::translate("MainWindow", "Button 3", nullptr));
        pushButton_Btn4->setText(QApplication::translate("MainWindow", "Button 4", nullptr));
        pushButton_Config->setText(QApplication::translate("MainWindow", "Config", nullptr));
        arrow_1->setText(QApplication::translate("MainWindow", "<h2>&rarr;</h2>", nullptr));
        pushButton_SetConfig->setText(QApplication::translate("MainWindow", "Set Config", nullptr));
        arrow_2->setText(QApplication::translate("MainWindow", "<h2>&rarr;</h2>", nullptr));
        pushButton_RunSystem->setText(QApplication::translate("MainWindow", "Start", nullptr));
        pushButton_Quit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuView->setTitle(QApplication::translate("MainWindow", "View", nullptr));
        menuWindow->setTitle(QApplication::translate("MainWindow", "Window", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
