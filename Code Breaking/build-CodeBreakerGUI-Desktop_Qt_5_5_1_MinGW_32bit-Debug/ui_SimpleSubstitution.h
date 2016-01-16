/********************************************************************************
** Form generated from reading UI file 'SimpleSubstitution.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLESUBSTITUTION_H
#define UI_SIMPLESUBSTITUTION_H

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

class Ui_SimpleSubstitution
{
public:
    QCheckBox *use_key;
    QLineEdit *crib;
    QCheckBox *use_crib;
    QLineEdit *key;
    QLabel *label_5;
    QLabel *label_6;

    void setupUi(QWidget *SimpleSubstitution)
    {
        if (SimpleSubstitution->objectName().isEmpty())
            SimpleSubstitution->setObjectName(QStringLiteral("SimpleSubstitution"));
        SimpleSubstitution->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SimpleSubstitution->sizePolicy().hasHeightForWidth());
        SimpleSubstitution->setSizePolicy(sizePolicy);
        SimpleSubstitution->setMinimumSize(QSize(492, 100));
        SimpleSubstitution->setMaximumSize(QSize(492, 100));
        use_key = new QCheckBox(SimpleSubstitution);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(290, 20, 70, 17));
        crib = new QLineEdit(SimpleSubstitution);
        crib->setObjectName(QStringLiteral("crib"));
        crib->setGeometry(QRect(50, 50, 201, 20));
        use_crib = new QCheckBox(SimpleSubstitution);
        use_crib->setObjectName(QStringLiteral("use_crib"));
        use_crib->setGeometry(QRect(290, 50, 70, 17));
        key = new QLineEdit(SimpleSubstitution);
        key->setObjectName(QStringLiteral("key"));
        key->setGeometry(QRect(50, 20, 201, 20));
        label_5 = new QLabel(SimpleSubstitution);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 20, 47, 16));
        label_6 = new QLabel(SimpleSubstitution);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 50, 47, 16));

        retranslateUi(SimpleSubstitution);

        QMetaObject::connectSlotsByName(SimpleSubstitution);
    } // setupUi

    void retranslateUi(QWidget *SimpleSubstitution)
    {
        SimpleSubstitution->setWindowTitle(QApplication::translate("SimpleSubstitution", "Form", 0));
        use_key->setText(QApplication::translate("SimpleSubstitution", "Use Key", 0));
        use_crib->setText(QApplication::translate("SimpleSubstitution", "Use Crib", 0));
        label_5->setText(QApplication::translate("SimpleSubstitution", "Key:", 0));
        label_6->setText(QApplication::translate("SimpleSubstitution", "Crib:", 0));
    } // retranslateUi

};

namespace Ui {
    class SimpleSubstitution: public Ui_SimpleSubstitution {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLESUBSTITUTION_H
