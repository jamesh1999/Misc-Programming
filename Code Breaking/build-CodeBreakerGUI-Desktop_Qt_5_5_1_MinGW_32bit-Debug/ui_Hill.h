/********************************************************************************
** Form generated from reading UI file 'Hill.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HILL_H
#define UI_HILL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Hill
{
public:
    QLabel *label;
    QSpinBox *key_size;
    QPushButton *edit_key;
    QLabel *label_2;
    QLineEdit *crib;
    QCheckBox *use_key;
    QCheckBox *use_crib;
    QCheckBox *beginning;

    void setupUi(QWidget *Hill)
    {
        if (Hill->objectName().isEmpty())
            Hill->setObjectName(QStringLiteral("Hill"));
        Hill->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Hill->sizePolicy().hasHeightForWidth());
        Hill->setSizePolicy(sizePolicy);
        Hill->setMinimumSize(QSize(492, 100));
        Hill->setMaximumSize(QSize(492, 100));
        label = new QLabel(Hill);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 47, 21));
        key_size = new QSpinBox(Hill);
        key_size->setObjectName(QStringLiteral("key_size"));
        key_size->setGeometry(QRect(60, 10, 42, 22));
        key_size->setMinimum(2);
        edit_key = new QPushButton(Hill);
        edit_key->setObjectName(QStringLiteral("edit_key"));
        edit_key->setGeometry(QRect(130, 10, 75, 23));
        label_2 = new QLabel(Hill);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 47, 21));
        crib = new QLineEdit(Hill);
        crib->setObjectName(QStringLiteral("crib"));
        crib->setGeometry(QRect(40, 50, 161, 20));
        use_key = new QCheckBox(Hill);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(290, 10, 70, 21));
        use_crib = new QCheckBox(Hill);
        use_crib->setObjectName(QStringLiteral("use_crib"));
        use_crib->setGeometry(QRect(290, 50, 70, 21));
        beginning = new QCheckBox(Hill);
        beginning->setObjectName(QStringLiteral("beginning"));
        beginning->setGeometry(QRect(290, 70, 70, 21));

        retranslateUi(Hill);

        QMetaObject::connectSlotsByName(Hill);
    } // setupUi

    void retranslateUi(QWidget *Hill)
    {
        Hill->setWindowTitle(QApplication::translate("Hill", "Form", 0));
        label->setText(QApplication::translate("Hill", "Key Size:", 0));
        edit_key->setText(QApplication::translate("Hill", "Edit Key", 0));
        label_2->setText(QApplication::translate("Hill", "Crib:", 0));
        use_key->setText(QApplication::translate("Hill", "Use Key", 0));
        use_crib->setText(QApplication::translate("Hill", "Use Crib", 0));
        beginning->setText(QApplication::translate("Hill", "Beginning", 0));
    } // retranslateUi

};

namespace Ui {
    class Hill: public Ui_Hill {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HILL_H
