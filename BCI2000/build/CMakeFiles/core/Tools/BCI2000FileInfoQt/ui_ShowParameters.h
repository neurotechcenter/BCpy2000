/********************************************************************************
** Form generated from reading UI file 'ShowParameters.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWPARAMETERS_H
#define UI_SHOWPARAMETERS_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ShowParameters
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTreeWidget *treeWidget;

    void setupUi(QDialog *ShowParameters)
    {
        if (ShowParameters->objectName().isEmpty())
            ShowParameters->setObjectName(QString::fromUtf8("ShowParameters"));
        ShowParameters->resize(249, 469);
        ShowParameters->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(ShowParameters);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(ShowParameters);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout->addWidget(label);

        treeWidget = new QTreeWidget(ShowParameters);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setColumnCount(1);
        treeWidget->header()->setVisible(false);

        verticalLayout->addWidget(treeWidget);


        retranslateUi(ShowParameters);

        QMetaObject::connectSlotsByName(ShowParameters);
    } // setupUi

    void retranslateUi(QDialog *ShowParameters)
    {
        ShowParameters->setWindowTitle(QApplication::translate("ShowParameters", "Dialog", nullptr));
        label->setText(QApplication::translate("ShowParameters", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowParameters: public Ui_ShowParameters {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWPARAMETERS_H
