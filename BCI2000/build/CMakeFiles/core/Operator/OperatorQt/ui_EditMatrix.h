/********************************************************************************
** Form generated from reading UI file 'EditMatrix.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITMATRIX_H
#define UI_EDITMATRIX_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EditMatrix
{
public:
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QSpinBox *colsSpinBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QSpinBox *rowsSpinBox;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QTableWidget *tableWidget;

    void setupUi(QDialog *EditMatrix)
    {
        if (EditMatrix->objectName().isEmpty())
            EditMatrix->setObjectName(QString::fromUtf8("EditMatrix"));
        EditMatrix->resize(444, 323);
        EditMatrix->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout_3 = new QVBoxLayout(EditMatrix);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label = new QLabel(EditMatrix);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        label->setWordWrap(true);

        verticalLayout_3->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(EditMatrix);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        colsSpinBox = new QSpinBox(EditMatrix);
        colsSpinBox->setObjectName(QString::fromUtf8("colsSpinBox"));
        colsSpinBox->setMinimum(1);
        colsSpinBox->setMaximum(65535);

        verticalLayout->addWidget(colsSpinBox);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_3 = new QLabel(EditMatrix);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        rowsSpinBox = new QSpinBox(EditMatrix);
        rowsSpinBox->setObjectName(QString::fromUtf8("rowsSpinBox"));
        rowsSpinBox->setMaximum(65535);

        verticalLayout_2->addWidget(rowsSpinBox);


        horizontalLayout->addLayout(verticalLayout_2);

        pushButton = new QPushButton(EditMatrix);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout);

        tableWidget = new QTableWidget(EditMatrix);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        if (tableWidget->rowCount() < 4)
            tableWidget->setRowCount(4);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->setRowCount(4);
        tableWidget->setColumnCount(4);

        verticalLayout_3->addWidget(tableWidget);


        retranslateUi(EditMatrix);

        QMetaObject::connectSlotsByName(EditMatrix);
    } // setupUi

    void retranslateUi(QDialog *EditMatrix)
    {
        EditMatrix->setWindowTitle(QApplication::translate("EditMatrix", "Dialog", nullptr));
        label->setText(QApplication::translate("EditMatrix", "Comment", nullptr));
        label_2->setText(QApplication::translate("EditMatrix", "# of columns", nullptr));
        label_3->setText(QApplication::translate("EditMatrix", "# of rows", nullptr));
        pushButton->setText(QApplication::translate("EditMatrix", "Set new matrix size", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditMatrix: public Ui_EditMatrix {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITMATRIX_H
