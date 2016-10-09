/********************************************************************************
** Form generated from reading UI file 'FourSquare.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOURSQUARE_H
#define UI_FOURSQUARE_H

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

class Ui_FourSquare
{
public:
    QLineEdit *key_1;
    QLabel *label_5;
    QCheckBox *use_key;
    QLabel *label_6;
    QLineEdit *key_2;

    void setupUi(QWidget *FourSquare)
    {
        if (FourSquare->objectName().isEmpty())
            FourSquare->setObjectName(QStringLiteral("FourSquare"));
        FourSquare->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FourSquare->sizePolicy().hasHeightForWidth());
        FourSquare->setSizePolicy(sizePolicy);
        FourSquare->setMinimumSize(QSize(492, 100));
        FourSquare->setMaximumSize(QSize(492, 100));
        key_1 = new QLineEdit(FourSquare);
        key_1->setObjectName(QStringLiteral("key_1"));
        key_1->setGeometry(QRect(50, 10, 201, 20));
        label_5 = new QLabel(FourSquare);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 47, 16));
        use_key = new QCheckBox(FourSquare);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(290, 10, 70, 21));
        label_6 = new QLabel(FourSquare);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 40, 47, 16));
        key_2 = new QLineEdit(FourSquare);
        key_2->setObjectName(QStringLiteral("key_2"));
        key_2->setGeometry(QRect(50, 40, 201, 20));

        retranslateUi(FourSquare);

        QMetaObject::connectSlotsByName(FourSquare);
    } // setupUi

    void retranslateUi(QWidget *FourSquare)
    {
        FourSquare->setWindowTitle(QApplication::translate("FourSquare", "Form", 0));
        label_5->setText(QApplication::translate("FourSquare", "Key 1:", 0));
        use_key->setText(QApplication::translate("FourSquare", "Use Key", 0));
        label_6->setText(QApplication::translate("FourSquare", "Key 2:", 0));
    } // retranslateUi

};

namespace Ui {
    class FourSquare: public Ui_FourSquare {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOURSQUARE_H
