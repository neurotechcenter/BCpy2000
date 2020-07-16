/********************************************************************************
** Form generated from reading UI file 'BCI2000Viewer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BCI2000VIEWER_H
#define UI_BCI2000VIEWER_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "SignalWidget.h"

QT_BEGIN_NAMESPACE

class Ui_BCI2000Viewer
{
public:
    QAction *actionOpen;
    QAction *actionClose;
    QAction *actionQuit;
    QAction *actionCut;
    QAction *actionCopy_to_Clipboard;
    QAction *actionPaste;
    QAction *actionEnlarge_Signal;
    QAction *actionReduce_Signal;
    QAction *actionZoom_Out;
    QAction *actionZoom_In;
    QAction *actionMore_Channels;
    QAction *actionFewer_Channels;
    QAction *actionChoose_Channel_Colors;
    QAction *actionInvert;
    QAction *actionShow_Baselines;
    QAction *actionShow_Unit;
    QAction *actionBCI2000_Help;
    QAction *actionAbout;
    QAction *actionShow_Channel_s;
    QAction *actionHide_Channel_s;
    QAction *actionState_Help;
    QAction *actionHP_Off;
    QAction *actionHP_Remove_Mean;
    QAction *actionHP_1Hz;
    QAction *actionHP_5Hz;
    QAction *actionLP_Off;
    QAction *actionLP_30Hz;
    QAction *actionLP_40Hz;
    QAction *actionLP_70Hz;
    QAction *actionNotch_Off;
    QAction *actionNotch_50Hz;
    QAction *actionNotch_60Hz;
    QAction *actionExportMat5;
    QAction *actionExportAscii;
    QAction *actionExport;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    SignalWidget *signalDisplay;
    QScrollBar *verticalScrollBar;
    QHBoxLayout *horizontalLayout;
    QPushButton *toBeginBtn;
    QPushButton *pageRewBtn;
    QPushButton *blockRewBtn;
    QSpacerItem *horizontalSpacer;
    QPushButton *zoomOutBtn;
    QLineEdit *editPosition;
    QPushButton *zoomInBtn;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *blockFwdBtn;
    QPushButton *pageFwdBtn;
    QPushButton *toEndBtn;
    QListWidget *channelList;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuHigh_Pass;
    QMenu *menuLow_Pass;
    QMenu *menuNotch;
    QMenu *menuHelp;

    void setupUi(QMainWindow *BCI2000Viewer)
    {
        if (BCI2000Viewer->objectName().isEmpty())
            BCI2000Viewer->setObjectName(QString::fromUtf8("BCI2000Viewer"));
        BCI2000Viewer->resize(627, 463);
        BCI2000Viewer->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionOpen = new QAction(BCI2000Viewer);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionClose = new QAction(BCI2000Viewer);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionQuit = new QAction(BCI2000Viewer);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionCut = new QAction(BCI2000Viewer);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCut->setEnabled(false);
        actionCopy_to_Clipboard = new QAction(BCI2000Viewer);
        actionCopy_to_Clipboard->setObjectName(QString::fromUtf8("actionCopy_to_Clipboard"));
        actionPaste = new QAction(BCI2000Viewer);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionPaste->setEnabled(false);
        actionEnlarge_Signal = new QAction(BCI2000Viewer);
        actionEnlarge_Signal->setObjectName(QString::fromUtf8("actionEnlarge_Signal"));
        actionReduce_Signal = new QAction(BCI2000Viewer);
        actionReduce_Signal->setObjectName(QString::fromUtf8("actionReduce_Signal"));
        actionZoom_Out = new QAction(BCI2000Viewer);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        actionZoom_In = new QAction(BCI2000Viewer);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        actionMore_Channels = new QAction(BCI2000Viewer);
        actionMore_Channels->setObjectName(QString::fromUtf8("actionMore_Channels"));
        actionFewer_Channels = new QAction(BCI2000Viewer);
        actionFewer_Channels->setObjectName(QString::fromUtf8("actionFewer_Channels"));
        actionChoose_Channel_Colors = new QAction(BCI2000Viewer);
        actionChoose_Channel_Colors->setObjectName(QString::fromUtf8("actionChoose_Channel_Colors"));
        actionInvert = new QAction(BCI2000Viewer);
        actionInvert->setObjectName(QString::fromUtf8("actionInvert"));
        actionShow_Baselines = new QAction(BCI2000Viewer);
        actionShow_Baselines->setObjectName(QString::fromUtf8("actionShow_Baselines"));
        actionShow_Unit = new QAction(BCI2000Viewer);
        actionShow_Unit->setObjectName(QString::fromUtf8("actionShow_Unit"));
        actionBCI2000_Help = new QAction(BCI2000Viewer);
        actionBCI2000_Help->setObjectName(QString::fromUtf8("actionBCI2000_Help"));
        actionAbout = new QAction(BCI2000Viewer);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionShow_Channel_s = new QAction(BCI2000Viewer);
        actionShow_Channel_s->setObjectName(QString::fromUtf8("actionShow_Channel_s"));
        actionHide_Channel_s = new QAction(BCI2000Viewer);
        actionHide_Channel_s->setObjectName(QString::fromUtf8("actionHide_Channel_s"));
        actionState_Help = new QAction(BCI2000Viewer);
        actionState_Help->setObjectName(QString::fromUtf8("actionState_Help"));
        actionState_Help->setEnabled(false);
        actionHP_Off = new QAction(BCI2000Viewer);
        actionHP_Off->setObjectName(QString::fromUtf8("actionHP_Off"));
        actionHP_Off->setCheckable(false);
        actionHP_Off->setChecked(false);
        actionHP_Remove_Mean = new QAction(BCI2000Viewer);
        actionHP_Remove_Mean->setObjectName(QString::fromUtf8("actionHP_Remove_Mean"));
        actionHP_Remove_Mean->setCheckable(false);
        actionHP_1Hz = new QAction(BCI2000Viewer);
        actionHP_1Hz->setObjectName(QString::fromUtf8("actionHP_1Hz"));
        actionHP_1Hz->setCheckable(false);
        actionHP_5Hz = new QAction(BCI2000Viewer);
        actionHP_5Hz->setObjectName(QString::fromUtf8("actionHP_5Hz"));
        actionHP_5Hz->setCheckable(false);
        actionLP_Off = new QAction(BCI2000Viewer);
        actionLP_Off->setObjectName(QString::fromUtf8("actionLP_Off"));
        actionLP_Off->setCheckable(false);
        actionLP_Off->setChecked(false);
        actionLP_30Hz = new QAction(BCI2000Viewer);
        actionLP_30Hz->setObjectName(QString::fromUtf8("actionLP_30Hz"));
        actionLP_30Hz->setCheckable(false);
        actionLP_40Hz = new QAction(BCI2000Viewer);
        actionLP_40Hz->setObjectName(QString::fromUtf8("actionLP_40Hz"));
        actionLP_40Hz->setCheckable(false);
        actionLP_70Hz = new QAction(BCI2000Viewer);
        actionLP_70Hz->setObjectName(QString::fromUtf8("actionLP_70Hz"));
        actionLP_70Hz->setCheckable(false);
        actionNotch_Off = new QAction(BCI2000Viewer);
        actionNotch_Off->setObjectName(QString::fromUtf8("actionNotch_Off"));
        actionNotch_Off->setCheckable(false);
        actionNotch_Off->setChecked(false);
        actionNotch_50Hz = new QAction(BCI2000Viewer);
        actionNotch_50Hz->setObjectName(QString::fromUtf8("actionNotch_50Hz"));
        actionNotch_50Hz->setCheckable(false);
        actionNotch_60Hz = new QAction(BCI2000Viewer);
        actionNotch_60Hz->setObjectName(QString::fromUtf8("actionNotch_60Hz"));
        actionNotch_60Hz->setCheckable(false);
        actionExportMat5 = new QAction(BCI2000Viewer);
        actionExportMat5->setObjectName(QString::fromUtf8("actionExportMat5"));
        actionExportAscii = new QAction(BCI2000Viewer);
        actionExportAscii->setObjectName(QString::fromUtf8("actionExportAscii"));
        actionExport = new QAction(BCI2000Viewer);
        actionExport->setObjectName(QString::fromUtf8("actionExport"));
        centralWidget = new QWidget(BCI2000Viewer);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(verticalLayoutWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        signalDisplay = new SignalWidget(frame);
        signalDisplay->setObjectName(QString::fromUtf8("signalDisplay"));

        horizontalLayout_2->addWidget(signalDisplay);

        verticalScrollBar = new QScrollBar(frame);
        verticalScrollBar->setObjectName(QString::fromUtf8("verticalScrollBar"));
        verticalScrollBar->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalScrollBar);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        verticalLayout->addWidget(frame);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        toBeginBtn = new QPushButton(verticalLayoutWidget);
        toBeginBtn->setObjectName(QString::fromUtf8("toBeginBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(toBeginBtn->sizePolicy().hasHeightForWidth());
        toBeginBtn->setSizePolicy(sizePolicy);
        toBeginBtn->setMinimumSize(QSize(30, 0));
        toBeginBtn->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(toBeginBtn);

        pageRewBtn = new QPushButton(verticalLayoutWidget);
        pageRewBtn->setObjectName(QString::fromUtf8("pageRewBtn"));
        sizePolicy.setHeightForWidth(pageRewBtn->sizePolicy().hasHeightForWidth());
        pageRewBtn->setSizePolicy(sizePolicy);
        pageRewBtn->setMinimumSize(QSize(30, 0));
        pageRewBtn->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(pageRewBtn);

        blockRewBtn = new QPushButton(verticalLayoutWidget);
        blockRewBtn->setObjectName(QString::fromUtf8("blockRewBtn"));
        sizePolicy.setHeightForWidth(blockRewBtn->sizePolicy().hasHeightForWidth());
        blockRewBtn->setSizePolicy(sizePolicy);
        blockRewBtn->setMinimumSize(QSize(30, 0));
        blockRewBtn->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(blockRewBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        zoomOutBtn = new QPushButton(verticalLayoutWidget);
        zoomOutBtn->setObjectName(QString::fromUtf8("zoomOutBtn"));
        sizePolicy.setHeightForWidth(zoomOutBtn->sizePolicy().hasHeightForWidth());
        zoomOutBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(zoomOutBtn);

        editPosition = new QLineEdit(verticalLayoutWidget);
        editPosition->setObjectName(QString::fromUtf8("editPosition"));
        sizePolicy.setHeightForWidth(editPosition->sizePolicy().hasHeightForWidth());
        editPosition->setSizePolicy(sizePolicy);
        editPosition->setMinimumSize(QSize(90, 0));

        horizontalLayout->addWidget(editPosition);

        zoomInBtn = new QPushButton(verticalLayoutWidget);
        zoomInBtn->setObjectName(QString::fromUtf8("zoomInBtn"));
        sizePolicy.setHeightForWidth(zoomInBtn->sizePolicy().hasHeightForWidth());
        zoomInBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(zoomInBtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        blockFwdBtn = new QPushButton(verticalLayoutWidget);
        blockFwdBtn->setObjectName(QString::fromUtf8("blockFwdBtn"));
        sizePolicy.setHeightForWidth(blockFwdBtn->sizePolicy().hasHeightForWidth());
        blockFwdBtn->setSizePolicy(sizePolicy);
        blockFwdBtn->setMinimumSize(QSize(30, 0));
        blockFwdBtn->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(blockFwdBtn);

        pageFwdBtn = new QPushButton(verticalLayoutWidget);
        pageFwdBtn->setObjectName(QString::fromUtf8("pageFwdBtn"));
        sizePolicy.setHeightForWidth(pageFwdBtn->sizePolicy().hasHeightForWidth());
        pageFwdBtn->setSizePolicy(sizePolicy);
        pageFwdBtn->setMinimumSize(QSize(30, 0));
        pageFwdBtn->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(pageFwdBtn);

        toEndBtn = new QPushButton(verticalLayoutWidget);
        toEndBtn->setObjectName(QString::fromUtf8("toEndBtn"));
        sizePolicy.setHeightForWidth(toEndBtn->sizePolicy().hasHeightForWidth());
        toEndBtn->setSizePolicy(sizePolicy);
        toEndBtn->setMinimumSize(QSize(30, 0));
        toEndBtn->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(toEndBtn);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);
        splitter->addWidget(verticalLayoutWidget);
        channelList = new QListWidget(splitter);
        channelList->setObjectName(QString::fromUtf8("channelList"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(channelList->sizePolicy().hasHeightForWidth());
        channelList->setSizePolicy(sizePolicy1);
        channelList->setMinimumSize(QSize(120, 0));
        channelList->setContextMenuPolicy(Qt::ActionsContextMenu);
        splitter->addWidget(channelList);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);

        BCI2000Viewer->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(BCI2000Viewer);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 627, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuHigh_Pass = new QMenu(menuView);
        menuHigh_Pass->setObjectName(QString::fromUtf8("menuHigh_Pass"));
        menuLow_Pass = new QMenu(menuView);
        menuLow_Pass->setObjectName(QString::fromUtf8("menuLow_Pass"));
        menuNotch = new QMenu(menuView);
        menuNotch->setObjectName(QString::fromUtf8("menuNotch"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        BCI2000Viewer->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionClose);
        menuFile->addAction(actionExport);
        menuFile->addAction(actionQuit);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy_to_Clipboard);
        menuEdit->addAction(actionPaste);
        menuView->addAction(actionEnlarge_Signal);
        menuView->addAction(actionReduce_Signal);
        menuView->addSeparator();
        menuView->addAction(actionZoom_Out);
        menuView->addAction(actionZoom_In);
        menuView->addSeparator();
        menuView->addAction(actionMore_Channels);
        menuView->addAction(actionFewer_Channels);
        menuView->addSeparator();
        menuView->addAction(actionChoose_Channel_Colors);
        menuView->addAction(actionInvert);
        menuView->addAction(actionShow_Baselines);
        menuView->addAction(actionShow_Unit);
        menuView->addSeparator();
        menuView->addAction(menuHigh_Pass->menuAction());
        menuView->addAction(menuLow_Pass->menuAction());
        menuView->addAction(menuNotch->menuAction());
        menuHigh_Pass->addAction(actionHP_Off);
        menuHigh_Pass->addAction(actionHP_Remove_Mean);
        menuHigh_Pass->addAction(actionHP_1Hz);
        menuHigh_Pass->addAction(actionHP_5Hz);
        menuLow_Pass->addAction(actionLP_Off);
        menuLow_Pass->addAction(actionLP_30Hz);
        menuLow_Pass->addAction(actionLP_40Hz);
        menuLow_Pass->addAction(actionLP_70Hz);
        menuNotch->addAction(actionNotch_Off);
        menuNotch->addAction(actionNotch_50Hz);
        menuNotch->addAction(actionNotch_60Hz);
        menuHelp->addAction(actionBCI2000_Help);
        menuHelp->addAction(actionAbout);

        retranslateUi(BCI2000Viewer);

        QMetaObject::connectSlotsByName(BCI2000Viewer);
    } // setupUi

    void retranslateUi(QMainWindow *BCI2000Viewer)
    {
        BCI2000Viewer->setWindowTitle(QApplication::translate("BCI2000Viewer", "BCI2000Viewer", nullptr));
        actionOpen->setText(QApplication::translate("BCI2000Viewer", "Open...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("BCI2000Viewer", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionClose->setText(QApplication::translate("BCI2000Viewer", "Close", nullptr));
#ifndef QT_NO_SHORTCUT
        actionClose->setShortcut(QApplication::translate("BCI2000Viewer", "Ctrl+W", nullptr));
#endif // QT_NO_SHORTCUT
        actionQuit->setText(QApplication::translate("BCI2000Viewer", "Quit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("BCI2000Viewer", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionCut->setText(QApplication::translate("BCI2000Viewer", "Cut", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCut->setShortcut(QApplication::translate("BCI2000Viewer", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        actionCopy_to_Clipboard->setText(QApplication::translate("BCI2000Viewer", "Copy to Clipboard", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy_to_Clipboard->setShortcut(QApplication::translate("BCI2000Viewer", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionPaste->setText(QApplication::translate("BCI2000Viewer", "Paste", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPaste->setShortcut(QApplication::translate("BCI2000Viewer", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        actionEnlarge_Signal->setText(QApplication::translate("BCI2000Viewer", "Enlarge Signal", nullptr));
        actionReduce_Signal->setText(QApplication::translate("BCI2000Viewer", "Reduce Signal", nullptr));
        actionZoom_Out->setText(QApplication::translate("BCI2000Viewer", "Zoom Out", nullptr));
        actionZoom_In->setText(QApplication::translate("BCI2000Viewer", "Zoom In", nullptr));
        actionMore_Channels->setText(QApplication::translate("BCI2000Viewer", "More Channels", nullptr));
        actionFewer_Channels->setText(QApplication::translate("BCI2000Viewer", "Fewer Channels", nullptr));
        actionChoose_Channel_Colors->setText(QApplication::translate("BCI2000Viewer", "Choose Channel Colors...", nullptr));
        actionInvert->setText(QApplication::translate("BCI2000Viewer", "Invert", nullptr));
        actionShow_Baselines->setText(QApplication::translate("BCI2000Viewer", "Show Baselines", nullptr));
        actionShow_Unit->setText(QApplication::translate("BCI2000Viewer", "Show Unit", nullptr));
        actionBCI2000_Help->setText(QApplication::translate("BCI2000Viewer", "BCI2000 Help", nullptr));
        actionAbout->setText(QApplication::translate("BCI2000Viewer", "About...", nullptr));
        actionShow_Channel_s->setText(QApplication::translate("BCI2000Viewer", "Show Channel(s)", nullptr));
        actionHide_Channel_s->setText(QApplication::translate("BCI2000Viewer", "Hide Channel(s)", nullptr));
        actionState_Help->setText(QApplication::translate("BCI2000Viewer", "BCI2000 Help", nullptr));
        actionHP_Off->setText(QApplication::translate("BCI2000Viewer", "Off", nullptr));
        actionHP_Remove_Mean->setText(QApplication::translate("BCI2000Viewer", "Remove Mean", nullptr));
        actionHP_1Hz->setText(QApplication::translate("BCI2000Viewer", "1Hz", nullptr));
        actionHP_5Hz->setText(QApplication::translate("BCI2000Viewer", "5Hz", nullptr));
        actionLP_Off->setText(QApplication::translate("BCI2000Viewer", "Off", nullptr));
        actionLP_30Hz->setText(QApplication::translate("BCI2000Viewer", "30Hz", nullptr));
        actionLP_40Hz->setText(QApplication::translate("BCI2000Viewer", "40Hz", nullptr));
        actionLP_70Hz->setText(QApplication::translate("BCI2000Viewer", "70Hz", nullptr));
        actionNotch_Off->setText(QApplication::translate("BCI2000Viewer", "Off", nullptr));
        actionNotch_50Hz->setText(QApplication::translate("BCI2000Viewer", "50Hz", nullptr));
        actionNotch_60Hz->setText(QApplication::translate("BCI2000Viewer", "60Hz", nullptr));
        actionExportMat5->setText(QApplication::translate("BCI2000Viewer", "as .mat file ...", nullptr));
        actionExportAscii->setText(QApplication::translate("BCI2000Viewer", "in high precision ASCII format ...", nullptr));
        actionExport->setText(QApplication::translate("BCI2000Viewer", "Export displayed data ...", nullptr));
        toBeginBtn->setText(QApplication::translate("BCI2000Viewer", "|<<", nullptr));
        pageRewBtn->setText(QApplication::translate("BCI2000Viewer", "<<", nullptr));
        blockRewBtn->setText(QApplication::translate("BCI2000Viewer", "<", nullptr));
        zoomOutBtn->setText(QApplication::translate("BCI2000Viewer", "Zoom Out", nullptr));
        zoomInBtn->setText(QApplication::translate("BCI2000Viewer", "Zoom In", nullptr));
        blockFwdBtn->setText(QApplication::translate("BCI2000Viewer", ">", nullptr));
        pageFwdBtn->setText(QApplication::translate("BCI2000Viewer", ">>", nullptr));
        toEndBtn->setText(QApplication::translate("BCI2000Viewer", ">>|", nullptr));
        menuFile->setTitle(QApplication::translate("BCI2000Viewer", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("BCI2000Viewer", "Edit", nullptr));
        menuView->setTitle(QApplication::translate("BCI2000Viewer", "View", nullptr));
        menuHigh_Pass->setTitle(QApplication::translate("BCI2000Viewer", "High Pass", nullptr));
        menuLow_Pass->setTitle(QApplication::translate("BCI2000Viewer", "Low Pass", nullptr));
        menuNotch->setTitle(QApplication::translate("BCI2000Viewer", "Notch", nullptr));
        menuHelp->setTitle(QApplication::translate("BCI2000Viewer", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BCI2000Viewer: public Ui_BCI2000Viewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BCI2000VIEWER_H
