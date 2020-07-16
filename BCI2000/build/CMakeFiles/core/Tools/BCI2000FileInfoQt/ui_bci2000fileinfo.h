/********************************************************************************
** Form generated from reading UI file 'bci2000fileinfo.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BCI2000FILEINFO_H
#define UI_BCI2000FILEINFO_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BCI2000FileInfo
{
public:
    QAction *actionBCI2000_Help;
    QAction *actionAbout;
    QAction *actionOpen;
    QAction *actionQuit;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *filenameLabel;
    QLabel *filename;
    QFormLayout *formLayout;
    QLabel *sampleBlockLabel;
    QLabel *blockSize;
    QLabel *samplingRateLabel;
    QLabel *samplingRate;
    QLabel *feedbackUpdateLabel;
    QLabel *feedback;
    QLabel *fileFormatVersionLabel;
    QLabel *formatVer;
    QLabel *dataFormatLabel;
    QLabel *dataFormat;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *parmButton;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuEdit;

    void setupUi(QMainWindow *BCI2000FileInfo)
    {
        if (BCI2000FileInfo->objectName().isEmpty())
            BCI2000FileInfo->setObjectName(QString::fromUtf8("BCI2000FileInfo"));
        BCI2000FileInfo->resize(324, 187);
        BCI2000FileInfo->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionBCI2000_Help = new QAction(BCI2000FileInfo);
        actionBCI2000_Help->setObjectName(QString::fromUtf8("actionBCI2000_Help"));
        actionAbout = new QAction(BCI2000FileInfo);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionOpen = new QAction(BCI2000FileInfo);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionQuit = new QAction(BCI2000FileInfo);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionCut = new QAction(BCI2000FileInfo);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCut->setEnabled(false);
        actionCopy = new QAction(BCI2000FileInfo);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionCopy->setEnabled(false);
        actionPaste = new QAction(BCI2000FileInfo);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionPaste->setEnabled(false);
        centralWidget = new QWidget(BCI2000FileInfo);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setAcceptDrops(true);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        filenameLabel = new QLabel(centralWidget);
        filenameLabel->setObjectName(QString::fromUtf8("filenameLabel"));
        filenameLabel->setMargin(0);
        filenameLabel->setIndent(-1);

        horizontalLayout_2->addWidget(filenameLabel);

        filename = new QLabel(centralWidget);
        filename->setObjectName(QString::fromUtf8("filename"));
        filename->setWordWrap(true);

        horizontalLayout_2->addWidget(filename);

        horizontalLayout_2->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        sampleBlockLabel = new QLabel(centralWidget);
        sampleBlockLabel->setObjectName(QString::fromUtf8("sampleBlockLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, sampleBlockLabel);

        blockSize = new QLabel(centralWidget);
        blockSize->setObjectName(QString::fromUtf8("blockSize"));

        formLayout->setWidget(0, QFormLayout::FieldRole, blockSize);

        samplingRateLabel = new QLabel(centralWidget);
        samplingRateLabel->setObjectName(QString::fromUtf8("samplingRateLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, samplingRateLabel);

        samplingRate = new QLabel(centralWidget);
        samplingRate->setObjectName(QString::fromUtf8("samplingRate"));

        formLayout->setWidget(1, QFormLayout::FieldRole, samplingRate);

        feedbackUpdateLabel = new QLabel(centralWidget);
        feedbackUpdateLabel->setObjectName(QString::fromUtf8("feedbackUpdateLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, feedbackUpdateLabel);

        feedback = new QLabel(centralWidget);
        feedback->setObjectName(QString::fromUtf8("feedback"));

        formLayout->setWidget(2, QFormLayout::FieldRole, feedback);

        fileFormatVersionLabel = new QLabel(centralWidget);
        fileFormatVersionLabel->setObjectName(QString::fromUtf8("fileFormatVersionLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, fileFormatVersionLabel);

        formatVer = new QLabel(centralWidget);
        formatVer->setObjectName(QString::fromUtf8("formatVer"));

        formLayout->setWidget(3, QFormLayout::FieldRole, formatVer);

        dataFormatLabel = new QLabel(centralWidget);
        dataFormatLabel->setObjectName(QString::fromUtf8("dataFormatLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, dataFormatLabel);

        dataFormat = new QLabel(centralWidget);
        dataFormat->setObjectName(QString::fromUtf8("dataFormat"));

        formLayout->setWidget(4, QFormLayout::FieldRole, dataFormat);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        parmButton = new QPushButton(centralWidget);
        parmButton->setObjectName(QString::fromUtf8("parmButton"));
        parmButton->setEnabled(false);
        parmButton->setCheckable(false);

        horizontalLayout->addWidget(parmButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        BCI2000FileInfo->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(BCI2000FileInfo);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 324, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        BCI2000FileInfo->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionBCI2000_Help);
        menuHelp->addAction(actionAbout);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);

        retranslateUi(BCI2000FileInfo);
        QObject::connect(actionQuit, SIGNAL(triggered()), BCI2000FileInfo, SLOT(close()));

        QMetaObject::connectSlotsByName(BCI2000FileInfo);
    } // setupUi

    void retranslateUi(QMainWindow *BCI2000FileInfo)
    {
        BCI2000FileInfo->setWindowTitle(QApplication::translate("BCI2000FileInfo", "BCI2000FileInfo", nullptr));
        actionBCI2000_Help->setText(QApplication::translate("BCI2000FileInfo", "BCI2000 Help", nullptr));
        actionAbout->setText(QApplication::translate("BCI2000FileInfo", "About...", nullptr));
        actionOpen->setText(QApplication::translate("BCI2000FileInfo", "Open...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("BCI2000FileInfo", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionQuit->setText(QApplication::translate("BCI2000FileInfo", "Quit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("BCI2000FileInfo", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionCut->setText(QApplication::translate("BCI2000FileInfo", "Cut", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCut->setShortcut(QApplication::translate("BCI2000FileInfo", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        actionCopy->setText(QApplication::translate("BCI2000FileInfo", "Copy", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy->setShortcut(QApplication::translate("BCI2000FileInfo", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionPaste->setText(QApplication::translate("BCI2000FileInfo", "Paste", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPaste->setShortcut(QApplication::translate("BCI2000FileInfo", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        filenameLabel->setText(QApplication::translate("BCI2000FileInfo", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Filename</span></p></body></html>", nullptr));
        filename->setText(QString());
        sampleBlockLabel->setText(QApplication::translate("BCI2000FileInfo", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Size of Sample Block</span></p></body></html>", nullptr));
        blockSize->setText(QString());
        samplingRateLabel->setText(QApplication::translate("BCI2000FileInfo", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Sampling Rate</span></p></body></html>", nullptr));
        samplingRate->setText(QString());
        feedbackUpdateLabel->setText(QApplication::translate("BCI2000FileInfo", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Feedback Update</span></p></body></html>", nullptr));
        feedback->setText(QString());
        fileFormatVersionLabel->setText(QApplication::translate("BCI2000FileInfo", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">File Format Version</span></p></body></html>", nullptr));
        formatVer->setText(QString());
        dataFormatLabel->setText(QApplication::translate("BCI2000FileInfo", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Data Format</span></p></body></html>", nullptr));
        dataFormat->setText(QString());
        parmButton->setText(QApplication::translate("BCI2000FileInfo", "View Parameters", nullptr));
        menuFile->setTitle(QApplication::translate("BCI2000FileInfo", "File", nullptr));
        menuHelp->setTitle(QApplication::translate("BCI2000FileInfo", "Help", nullptr));
        menuEdit->setTitle(QApplication::translate("BCI2000FileInfo", "Edit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BCI2000FileInfo: public Ui_BCI2000FileInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BCI2000FILEINFO_H
