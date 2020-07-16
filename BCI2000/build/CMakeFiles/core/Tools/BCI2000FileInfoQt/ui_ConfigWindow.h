/********************************************************************************
** Form generated from reading UI file 'ConfigWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGWINDOW_H
#define UI_CONFIGWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *cfgTabControl;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_4;
    QPushButton *bSaveParameters;
    QPushButton *bLoadParameters;
    QSpacerItem *verticalSpacer_3;
    QPushButton *bConfigureSave;
    QPushButton *bConfigureLoad;
    QSpacerItem *verticalSpacer_2;
    QPushButton *bHelp;
    QSpacerItem *verticalSpacer;
    QPushButton *bClose;

    void setupUi(QDialog *ConfigWindow)
    {
        if (ConfigWindow->objectName().isEmpty())
            ConfigWindow->setObjectName(QString::fromUtf8("ConfigWindow"));
        ConfigWindow->resize(691, 519);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConfigWindow->sizePolicy().hasHeightForWidth());
        ConfigWindow->setSizePolicy(sizePolicy);
        ConfigWindow->setMinimumSize(QSize(0, 0));
        ConfigWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        ConfigWindow->setSizeGripEnabled(true);
        horizontalLayout = new QHBoxLayout(ConfigWindow);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cfgTabControl = new QTabWidget(ConfigWindow);
        cfgTabControl->setObjectName(QString::fromUtf8("cfgTabControl"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(cfgTabControl->sizePolicy().hasHeightForWidth());
        cfgTabControl->setSizePolicy(sizePolicy1);
        cfgTabControl->setMinimumSize(QSize(552, 100));
        cfgTabControl->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        cfgTabControl->setTabShape(QTabWidget::Rounded);
        cfgTabControl->setElideMode(Qt::ElideNone);
        cfgTabControl->setUsesScrollButtons(true);
        cfgTabControl->setDocumentMode(false);
        cfgTabControl->setTabsClosable(false);
        cfgTabControl->setMovable(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        cfgTabControl->addTab(tab, QString());

        horizontalLayout->addWidget(cfgTabControl);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_4);

        bSaveParameters = new QPushButton(ConfigWindow);
        bSaveParameters->setObjectName(QString::fromUtf8("bSaveParameters"));

        verticalLayout->addWidget(bSaveParameters);

        bLoadParameters = new QPushButton(ConfigWindow);
        bLoadParameters->setObjectName(QString::fromUtf8("bLoadParameters"));

        verticalLayout->addWidget(bLoadParameters);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        bConfigureSave = new QPushButton(ConfigWindow);
        bConfigureSave->setObjectName(QString::fromUtf8("bConfigureSave"));

        verticalLayout->addWidget(bConfigureSave);

        bConfigureLoad = new QPushButton(ConfigWindow);
        bConfigureLoad->setObjectName(QString::fromUtf8("bConfigureLoad"));

        verticalLayout->addWidget(bConfigureLoad);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        bHelp = new QPushButton(ConfigWindow);
        bHelp->setObjectName(QString::fromUtf8("bHelp"));

        verticalLayout->addWidget(bHelp);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        bClose = new QPushButton(ConfigWindow);
        bClose->setObjectName(QString::fromUtf8("bClose"));
        bClose->setAutoDefault(false);

        verticalLayout->addWidget(bClose);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(ConfigWindow);

        cfgTabControl->setCurrentIndex(0);
        bClose->setDefault(true);


        QMetaObject::connectSlotsByName(ConfigWindow);
    } // setupUi

    void retranslateUi(QDialog *ConfigWindow)
    {
        ConfigWindow->setWindowTitle(QApplication::translate("ConfigWindow", "Parameter Configuration", nullptr));
        cfgTabControl->setTabText(cfgTabControl->indexOf(tab), QApplication::translate("ConfigWindow", "Tab 1", nullptr));
        bSaveParameters->setText(QApplication::translate("ConfigWindow", "Save Parameters ...", nullptr));
        bLoadParameters->setText(QApplication::translate("ConfigWindow", "Load Parameters ...", nullptr));
        bConfigureSave->setText(QApplication::translate("ConfigWindow", "Configure Save ...", nullptr));
        bConfigureLoad->setText(QApplication::translate("ConfigWindow", "Configure Load ...", nullptr));
        bHelp->setText(QApplication::translate("ConfigWindow", "Help", nullptr));
        bClose->setText(QApplication::translate("ConfigWindow", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigWindow: public Ui_ConfigWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGWINDOW_H
