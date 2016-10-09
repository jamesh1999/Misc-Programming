/********************************************************************************
** Form generated from reading UI file 'Vigenere.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIGENERE_H
#define UI_VIGENERE_H

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

class Ui_Vigenere
{
public:
    QCheckBox *use_crib;
    QLineEdit *crib;
    QLabel *label_5;
    QLabel *label_6;
    QCheckBox *use_key;
    QLineEdit *key;

    void setupUi(QWidget *Vigenere)
    {
        if (Vigenere->objectName().isEmpty())
            Vigenere->setObjectName(QStringLiteral("Vigenere"));
        Vigenere->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Vigenere->sizePolicy().hasHeightForWidth());
        Vigenere->setSizePolicy(sizePolicy);
        Vigenere->setMinimumSize(QSize(0, 100));
        Vigenere->setMaximumSize(QSize(16777215, 100));
        use_crib = new QCheckBox(Vigenere);
        use_crib->setObjectName(QStringLiteral("use_crib"));
        use_crib->setGeometry(QRect(290, 40, 70, 21));
        crib = new QLineEdit(Vigenere);
        crib->setObjectName(QStringLiteral("crib"));
        crib->setGeometry(QRect(50, 40, 201, 20));
        label_5 = new QLabel(Vigenere);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 47, 16));
        label_6 = new QLabel(Vigenere);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 40, 47, 16));
        use_key = new QCheckBox(Vigenere);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(290, 10, 70, 21));
        key = new QLineEdit(Vigenere);
        key->setObjectName(QStringLiteral("key"));
        key->setGeometry(QRect(50, 10, 201, 20));

        retranslateUi(Vigenere);

        QMetaObject::connectSlotsByName(Vigenere);
    } // setupUi

    void retranslateUi(QWidget *Vigenere)
    {
        Vigenere->setWindowTitle(QApplication::translate("Vigenere", "Form", 0));
        use_crib->setText(QApplication::translate("Vigenere", "Use Crib", 0));
        label_5->setText(QApplication::translate("Vigenere", "Key:", 0));
        label_6->setText(QApplication::translate("Vigenere", "Crib:", 0));
        use_key->setText(QApplication::translate("Vigenere", "Use Key", 0));
    } // retranslateUi

};

namespace Ui {
    class Vigenere: public Ui_Vigenere {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIGENERE_H
