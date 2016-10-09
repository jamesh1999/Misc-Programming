/********************************************************************************
** Form generated from reading UI file 'ColumnView.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLUMNVIEW_H
#define UI_COLUMNVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ColumnView
{
public:
    QDialogButtonBox *buttonBox;
    QTableWidget *tableWidget;

    void setupUi(QDialog *ColumnView)
    {
        if (ColumnView->objectName().isEmpty())
            ColumnView->setObjectName(QStringLiteral("ColumnView"));
        ColumnView->resize(574, 179);
        buttonBox = new QDialogButtonBox(ColumnView);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(220, 140, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tableWidget = new QTableWidget(ColumnView);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(10, 10, 551, 121));
        tableWidget->setRowCount(0);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setVisible(true);

        retranslateUi(ColumnView);
        QObject::connect(buttonBox, SIGNAL(accepted()), ColumnView, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ColumnView, SLOT(reject()));

        QMetaObject::connectSlotsByName(ColumnView);
    } // setupUi

    void retranslateUi(QDialog *ColumnView)
    {
        ColumnView->setWindowTitle(QApplication::translate("ColumnView", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class ColumnView: public Ui_ColumnView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLUMNVIEW_H
