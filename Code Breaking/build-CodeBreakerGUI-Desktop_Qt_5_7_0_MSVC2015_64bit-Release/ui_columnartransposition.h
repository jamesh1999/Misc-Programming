/********************************************************************************
** Form generated from reading UI file 'columnartransposition.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLUMNARTRANSPOSITION_H
#define UI_COLUMNARTRANSPOSITION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColumnarTransposition
{
public:
    QSpinBox *column_count;
    QCheckBox *use_key;
    QLabel *label;
    QPushButton *edit_key;

    void setupUi(QWidget *ColumnarTransposition)
    {
        if (ColumnarTransposition->objectName().isEmpty())
            ColumnarTransposition->setObjectName(QStringLiteral("ColumnarTransposition"));
        ColumnarTransposition->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ColumnarTransposition->sizePolicy().hasHeightForWidth());
        ColumnarTransposition->setSizePolicy(sizePolicy);
        ColumnarTransposition->setMinimumSize(QSize(492, 100));
        ColumnarTransposition->setMaximumSize(QSize(492, 100));
        column_count = new QSpinBox(ColumnarTransposition);
        column_count->setObjectName(QStringLiteral("column_count"));
        column_count->setGeometry(QRect(90, 10, 42, 22));
        column_count->setMinimum(1);
        use_key = new QCheckBox(ColumnarTransposition);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(280, 10, 70, 21));
        label = new QLabel(ColumnarTransposition);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 71, 21));
        edit_key = new QPushButton(ColumnarTransposition);
        edit_key->setObjectName(QStringLiteral("edit_key"));
        edit_key->setGeometry(QRect(10, 40, 121, 31));

        retranslateUi(ColumnarTransposition);

        QMetaObject::connectSlotsByName(ColumnarTransposition);
    } // setupUi

    void retranslateUi(QWidget *ColumnarTransposition)
    {
        ColumnarTransposition->setWindowTitle(QApplication::translate("ColumnarTransposition", "Form", 0));
        use_key->setText(QApplication::translate("ColumnarTransposition", "Use Key", 0));
        label->setText(QApplication::translate("ColumnarTransposition", "Column Count:", 0));
        edit_key->setText(QApplication::translate("ColumnarTransposition", "Edit Key", 0));
    } // retranslateUi

};

namespace Ui {
    class ColumnarTransposition: public Ui_ColumnarTransposition {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLUMNARTRANSPOSITION_H
