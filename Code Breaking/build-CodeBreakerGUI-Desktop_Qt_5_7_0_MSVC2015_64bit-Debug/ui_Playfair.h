/********************************************************************************
** Form generated from reading UI file 'Playfair.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYFAIR_H
#define UI_PLAYFAIR_H

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

class Ui_Playfair
{
public:
    QLineEdit *key;
    QLabel *label_5;
    QCheckBox *use_key;

    void setupUi(QWidget *Playfair)
    {
        if (Playfair->objectName().isEmpty())
            Playfair->setObjectName(QStringLiteral("Playfair"));
        Playfair->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Playfair->sizePolicy().hasHeightForWidth());
        Playfair->setSizePolicy(sizePolicy);
        Playfair->setMinimumSize(QSize(492, 100));
        Playfair->setMaximumSize(QSize(492, 100));
        key = new QLineEdit(Playfair);
        key->setObjectName(QStringLiteral("key"));
        key->setGeometry(QRect(50, 10, 201, 20));
        label_5 = new QLabel(Playfair);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 47, 16));
        use_key = new QCheckBox(Playfair);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(290, 10, 70, 21));

        retranslateUi(Playfair);

        QMetaObject::connectSlotsByName(Playfair);
    } // setupUi

    void retranslateUi(QWidget *Playfair)
    {
        Playfair->setWindowTitle(QApplication::translate("Playfair", "Form", 0));
        label_5->setText(QApplication::translate("Playfair", "Key:", 0));
        use_key->setText(QApplication::translate("Playfair", "Use Key", 0));
    } // retranslateUi

};

namespace Ui {
    class Playfair: public Ui_Playfair {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYFAIR_H
