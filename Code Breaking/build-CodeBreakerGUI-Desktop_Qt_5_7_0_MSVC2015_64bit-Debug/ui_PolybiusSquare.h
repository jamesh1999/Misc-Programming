/********************************************************************************
** Form generated from reading UI file 'PolybiusSquare.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POLYBIUSSQUARE_H
#define UI_POLYBIUSSQUARE_H

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

class Ui_PolybiusSquare
{
public:
    QLineEdit *key;
    QLabel *label_5;
    QCheckBox *use_key;

    void setupUi(QWidget *PolybiusSquare)
    {
        if (PolybiusSquare->objectName().isEmpty())
            PolybiusSquare->setObjectName(QStringLiteral("PolybiusSquare"));
        PolybiusSquare->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PolybiusSquare->sizePolicy().hasHeightForWidth());
        PolybiusSquare->setSizePolicy(sizePolicy);
        PolybiusSquare->setMinimumSize(QSize(492, 100));
        PolybiusSquare->setMaximumSize(QSize(492, 100));
        key = new QLineEdit(PolybiusSquare);
        key->setObjectName(QStringLiteral("key"));
        key->setGeometry(QRect(50, 10, 201, 20));
        label_5 = new QLabel(PolybiusSquare);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 47, 16));
        use_key = new QCheckBox(PolybiusSquare);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(290, 10, 70, 21));

        retranslateUi(PolybiusSquare);

        QMetaObject::connectSlotsByName(PolybiusSquare);
    } // setupUi

    void retranslateUi(QWidget *PolybiusSquare)
    {
        PolybiusSquare->setWindowTitle(QApplication::translate("PolybiusSquare", "Form", 0));
        label_5->setText(QApplication::translate("PolybiusSquare", "Key:", 0));
        use_key->setText(QApplication::translate("PolybiusSquare", "Use Key", 0));
    } // retranslateUi

};

namespace Ui {
    class PolybiusSquare: public Ui_PolybiusSquare {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POLYBIUSSQUARE_H
