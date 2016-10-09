/********************************************************************************
** Form generated from reading UI file 'BinaryConvert.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BINARYCONVERT_H
#define UI_BINARYCONVERT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BinaryConvert
{
public:
    QComboBox *method;
    QLabel *label;
    QSpinBox *letter_length;
    QLabel *label_2;

    void setupUi(QWidget *BinaryConvert)
    {
        if (BinaryConvert->objectName().isEmpty())
            BinaryConvert->setObjectName(QStringLiteral("BinaryConvert"));
        BinaryConvert->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BinaryConvert->sizePolicy().hasHeightForWidth());
        BinaryConvert->setSizePolicy(sizePolicy);
        BinaryConvert->setMinimumSize(QSize(492, 100));
        BinaryConvert->setMaximumSize(QSize(492, 100));
        method = new QComboBox(BinaryConvert);
        method->setObjectName(QStringLiteral("method"));
        method->setGeometry(QRect(90, 10, 81, 22));
        label = new QLabel(BinaryConvert);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 47, 21));
        letter_length = new QSpinBox(BinaryConvert);
        letter_length->setObjectName(QStringLiteral("letter_length"));
        letter_length->setGeometry(QRect(90, 40, 42, 22));
        letter_length->setMinimum(5);
        label_2 = new QLabel(BinaryConvert);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 40, 71, 21));

        retranslateUi(BinaryConvert);

        QMetaObject::connectSlotsByName(BinaryConvert);
    } // setupUi

    void retranslateUi(QWidget *BinaryConvert)
    {
        BinaryConvert->setWindowTitle(QApplication::translate("BinaryConvert", "Form", 0));
        method->clear();
        method->insertItems(0, QStringList()
         << QApplication::translate("BinaryConvert", "ASCII", 0)
         << QApplication::translate("BinaryConvert", "A = 0", 0)
         << QApplication::translate("BinaryConvert", "A = 1", 0)
        );
        label->setText(QApplication::translate("BinaryConvert", "Method:", 0));
        label_2->setText(QApplication::translate("BinaryConvert", "Letter Length:", 0));
    } // retranslateUi

};

namespace Ui {
    class BinaryConvert: public Ui_BinaryConvert {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BINARYCONVERT_H
