/********************************************************************************
** Form generated from reading UI file 'Amsco.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AMSCO_H
#define UI_AMSCO_H

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

class Ui_Amsco
{
public:
    QCheckBox *use_key;
    QCheckBox *vertical_swap;
    QPushButton *edit_key;
    QSpinBox *column_count;
    QLabel *label;
    QCheckBox *begin_pair;

    void setupUi(QWidget *Amsco)
    {
        if (Amsco->objectName().isEmpty())
            Amsco->setObjectName(QStringLiteral("Amsco"));
        Amsco->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Amsco->sizePolicy().hasHeightForWidth());
        Amsco->setSizePolicy(sizePolicy);
        Amsco->setMinimumSize(QSize(492, 100));
        Amsco->setMaximumSize(QSize(492, 100));
        use_key = new QCheckBox(Amsco);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(280, 10, 201, 21));
        vertical_swap = new QCheckBox(Amsco);
        vertical_swap->setObjectName(QStringLiteral("vertical_swap"));
        vertical_swap->setGeometry(QRect(280, 40, 201, 21));
        edit_key = new QPushButton(Amsco);
        edit_key->setObjectName(QStringLiteral("edit_key"));
        edit_key->setGeometry(QRect(10, 40, 121, 31));
        column_count = new QSpinBox(Amsco);
        column_count->setObjectName(QStringLiteral("column_count"));
        column_count->setGeometry(QRect(90, 10, 42, 22));
        column_count->setMinimum(1);
        label = new QLabel(Amsco);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 71, 21));
        begin_pair = new QCheckBox(Amsco);
        begin_pair->setObjectName(QStringLiteral("begin_pair"));
        begin_pair->setGeometry(QRect(280, 70, 201, 21));

        retranslateUi(Amsco);

        QMetaObject::connectSlotsByName(Amsco);
    } // setupUi

    void retranslateUi(QWidget *Amsco)
    {
        Amsco->setWindowTitle(QApplication::translate("Amsco", "Form", 0));
        use_key->setText(QApplication::translate("Amsco", "Use Key", 0));
        vertical_swap->setText(QApplication::translate("Amsco", "Force Vertical Swap", 0));
        edit_key->setText(QApplication::translate("Amsco", "Edit Key", 0));
        label->setText(QApplication::translate("Amsco", "Column Count:", 0));
        begin_pair->setText(QApplication::translate("Amsco", "Begin Pair", 0));
    } // retranslateUi

};

namespace Ui {
    class Amsco: public Ui_Amsco {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AMSCO_H
