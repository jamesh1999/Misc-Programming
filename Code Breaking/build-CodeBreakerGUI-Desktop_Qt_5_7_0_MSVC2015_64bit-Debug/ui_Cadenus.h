/********************************************************************************
** Form generated from reading UI file 'Cadenus.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CADENUS_H
#define UI_CADENUS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Cadenus
{
public:
    QLabel *label_5;
    QLineEdit *key_1;
    QCheckBox *use_key;
    QLineEdit *key_2;

    void setupUi(QWidget *Cadenus)
    {
        if (Cadenus->objectName().isEmpty())
            Cadenus->setObjectName(QStringLiteral("Cadenus"));
        Cadenus->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Cadenus->sizePolicy().hasHeightForWidth());
        Cadenus->setSizePolicy(sizePolicy);
        Cadenus->setMinimumSize(QSize(492, 100));
        Cadenus->setMaximumSize(QSize(492, 100));
        label_5 = new QLabel(Cadenus);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 47, 16));
        key_1 = new QLineEdit(Cadenus);
        key_1->setObjectName(QStringLiteral("key_1"));
        key_1->setGeometry(QRect(50, 10, 201, 20));
        use_key = new QCheckBox(Cadenus);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(290, 10, 70, 21));
        key_2 = new QLineEdit(Cadenus);
        key_2->setObjectName(QStringLiteral("key_2"));
        key_2->setGeometry(QRect(50, 40, 201, 20));

        retranslateUi(Cadenus);

        QMetaObject::connectSlotsByName(Cadenus);
    } // setupUi

    void retranslateUi(QWidget *Cadenus)
    {
        Cadenus->setWindowTitle(QApplication::translate("Cadenus", "Form", 0));
        label_5->setText(QApplication::translate("Cadenus", "Key:", 0));
        use_key->setText(QApplication::translate("Cadenus", "Use Key", 0));
    } // retranslateUi

};

namespace Ui {
    class Cadenus: public Ui_Cadenus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CADENUS_H
