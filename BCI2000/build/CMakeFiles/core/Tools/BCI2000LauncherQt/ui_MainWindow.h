/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionBCI2000_Help;
    QAction *actionAbout;
    QAction *actionTo_Others;
    QAction *actionTo_Applications;
    QAction *actionTo_Signal_Processing;
    QAction *actionTo_Signal_Source;
    QAction *actionUp;
    QAction *actionDown;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *programsGroup;
    QListWidget *sourceList;
    QListWidget *sigprocList;
    QListWidget *applicationList;
    QGroupBox *otherGroup;
    QListWidget *otherList;
    QPushButton *clearSelectionBtn;
    QHBoxLayout *horizontalLayout;
    QGroupBox *parametersGroup;
    QListWidget *paramfilesList;
    QPushButton *addParmfileBtn;
    QPushButton *removeParmfileBtn;
    QLineEdit *saveDirEdit;
    QPushButton *chooseDirBtn;
    QLineEdit *subjectNameEdit;
    QLineEdit *subjectSessionEdit;
    QGroupBox *statusGroup;
    QTextEdit *statusEdit;
    QPushButton *launchButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(745, 425);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCut->setEnabled(false);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionCopy->setEnabled(false);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionPaste->setEnabled(false);
        actionBCI2000_Help = new QAction(MainWindow);
        actionBCI2000_Help->setObjectName(QString::fromUtf8("actionBCI2000_Help"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionTo_Others = new QAction(MainWindow);
        actionTo_Others->setObjectName(QString::fromUtf8("actionTo_Others"));
        actionTo_Applications = new QAction(MainWindow);
        actionTo_Applications->setObjectName(QString::fromUtf8("actionTo_Applications"));
        actionTo_Signal_Processing = new QAction(MainWindow);
        actionTo_Signal_Processing->setObjectName(QString::fromUtf8("actionTo_Signal_Processing"));
        actionTo_Signal_Source = new QAction(MainWindow);
        actionTo_Signal_Source->setObjectName(QString::fromUtf8("actionTo_Signal_Source"));
        actionUp = new QAction(MainWindow);
        actionUp->setObjectName(QString::fromUtf8("actionUp"));
        actionDown = new QAction(MainWindow);
        actionDown->setObjectName(QString::fromUtf8("actionDown"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        programsGroup = new QGroupBox(centralWidget);
        programsGroup->setObjectName(QString::fromUtf8("programsGroup"));
        programsGroup->setFlat(false);
        sourceList = new QListWidget(programsGroup);
        sourceList->setObjectName(QString::fromUtf8("sourceList"));
        sourceList->setGeometry(QRect(20, 20, 131, 131));
        sourceList->setContextMenuPolicy(Qt::ActionsContextMenu);
        sourceList->setSortingEnabled(false);
        sigprocList = new QListWidget(programsGroup);
        sigprocList->setObjectName(QString::fromUtf8("sigprocList"));
        sigprocList->setGeometry(QRect(170, 20, 131, 131));
        sigprocList->setContextMenuPolicy(Qt::ActionsContextMenu);
        sigprocList->setSortingEnabled(false);
        applicationList = new QListWidget(programsGroup);
        applicationList->setObjectName(QString::fromUtf8("applicationList"));
        applicationList->setGeometry(QRect(320, 20, 131, 131));
        applicationList->setContextMenuPolicy(Qt::ActionsContextMenu);
        applicationList->setSortingEnabled(false);
        otherGroup = new QGroupBox(programsGroup);
        otherGroup->setObjectName(QString::fromUtf8("otherGroup"));
        otherGroup->setGeometry(QRect(490, 10, 161, 160));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(otherGroup->sizePolicy().hasHeightForWidth());
        otherGroup->setSizePolicy(sizePolicy);
        otherGroup->setMinimumSize(QSize(160, 150));
        otherList = new QListWidget(otherGroup);
        otherList->setObjectName(QString::fromUtf8("otherList"));
        otherList->setGeometry(QRect(30, 30, 91, 91));
        otherList->setContextMenuPolicy(Qt::ActionsContextMenu);
        otherList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        otherList->setSortingEnabled(false);
        clearSelectionBtn = new QPushButton(programsGroup);
        clearSelectionBtn->setObjectName(QString::fromUtf8("clearSelectionBtn"));
        clearSelectionBtn->setGeometry(QRect(20, 160, 91, 24));

        verticalLayout->addWidget(programsGroup);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        parametersGroup = new QGroupBox(centralWidget);
        parametersGroup->setObjectName(QString::fromUtf8("parametersGroup"));
        paramfilesList = new QListWidget(parametersGroup);
        paramfilesList->setObjectName(QString::fromUtf8("paramfilesList"));
        paramfilesList->setGeometry(QRect(60, 20, 241, 91));
        addParmfileBtn = new QPushButton(parametersGroup);
        addParmfileBtn->setObjectName(QString::fromUtf8("addParmfileBtn"));
        addParmfileBtn->setGeometry(QRect(310, 20, 31, 24));
        removeParmfileBtn = new QPushButton(parametersGroup);
        removeParmfileBtn->setObjectName(QString::fromUtf8("removeParmfileBtn"));
        removeParmfileBtn->setGeometry(QRect(310, 50, 31, 24));
        saveDirEdit = new QLineEdit(parametersGroup);
        saveDirEdit->setObjectName(QString::fromUtf8("saveDirEdit"));
        saveDirEdit->setGeometry(QRect(60, 120, 151, 20));
        chooseDirBtn = new QPushButton(parametersGroup);
        chooseDirBtn->setObjectName(QString::fromUtf8("chooseDirBtn"));
        chooseDirBtn->setGeometry(QRect(230, 120, 75, 24));
        subjectNameEdit = new QLineEdit(parametersGroup);
        subjectNameEdit->setObjectName(QString::fromUtf8("subjectNameEdit"));
        subjectNameEdit->setGeometry(QRect(60, 150, 113, 20));
        subjectSessionEdit = new QLineEdit(parametersGroup);
        subjectSessionEdit->setObjectName(QString::fromUtf8("subjectSessionEdit"));
        subjectSessionEdit->setGeometry(QRect(230, 150, 113, 20));

        horizontalLayout->addWidget(parametersGroup);

        statusGroup = new QGroupBox(centralWidget);
        statusGroup->setObjectName(QString::fromUtf8("statusGroup"));
        statusEdit = new QTextEdit(statusGroup);
        statusEdit->setObjectName(QString::fromUtf8("statusEdit"));
        statusEdit->setGeometry(QRect(40, 30, 104, 64));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier"));
        statusEdit->setFont(font);
        launchButton = new QPushButton(statusGroup);
        launchButton->setObjectName(QString::fromUtf8("launchButton"));
        launchButton->setGeometry(QRect(40, 120, 91, 31));

        horizontalLayout->addWidget(statusGroup);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 745, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuHelp->addAction(actionBCI2000_Help);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionCut->setText(QApplication::translate("MainWindow", "Cut", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPaste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V, Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionBCI2000_Help->setText(QApplication::translate("MainWindow", "BCI2000 Help", nullptr));
        actionAbout->setText(QApplication::translate("MainWindow", "About ...", nullptr));
        actionTo_Others->setText(QApplication::translate("MainWindow", "to Others", nullptr));
        actionTo_Applications->setText(QApplication::translate("MainWindow", "to Applications", nullptr));
        actionTo_Signal_Processing->setText(QApplication::translate("MainWindow", "to Signal Processing", nullptr));
        actionTo_Signal_Source->setText(QApplication::translate("MainWindow", "to Signal Source", nullptr));
        actionUp->setText(QApplication::translate("MainWindow", "move Up", nullptr));
        actionDown->setText(QApplication::translate("MainWindow", "move Down", nullptr));
        programsGroup->setTitle(QApplication::translate("MainWindow", "BCI Program Modules", nullptr));
        otherGroup->setTitle(QApplication::translate("MainWindow", "Other", nullptr));
        clearSelectionBtn->setText(QApplication::translate("MainWindow", "Clear Selection", nullptr));
        parametersGroup->setTitle(QApplication::translate("MainWindow", "Parameters", nullptr));
        addParmfileBtn->setText(QApplication::translate("MainWindow", "+", nullptr));
        removeParmfileBtn->setText(QApplication::translate("MainWindow", "-", nullptr));
        chooseDirBtn->setText(QApplication::translate("MainWindow", "Choose...", nullptr));
        statusGroup->setTitle(QApplication::translate("MainWindow", "Status", nullptr));
        launchButton->setText(QApplication::translate("MainWindow", "Launch", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
