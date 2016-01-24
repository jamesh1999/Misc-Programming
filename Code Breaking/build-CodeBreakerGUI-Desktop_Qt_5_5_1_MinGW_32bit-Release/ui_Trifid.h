/********************************************************************************
** Form generated from reading UI file 'Trifid.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIFID_H
#define UI_TRIFID_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Trifid
{
public:
    QLabel *label;
    QLineEdit *key;
    QCheckBox *use_key;
    QSpinBox *period;
    QLabel *label_2;

    void setupUi(QWidget *Trifid)
    {
        if (Trifid->objectName().isEmpty())
            Trifid->setObjectName(QStringLiteral("Trifid"));
        Trifid->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Trifid->sizePolicy().hasHeightForWidth());
        Trifid->setSizePolicy(sizePolicy);
        Trifid->setMinimumSize(QSize(492, 100));
        Trifid->setMaximumSize(QSize(492, 100));
        label = new QLabel(Trifid);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 47, 16));
        key = new QLineEdit(Trifid);
        key->setObjectName(QStringLiteral("key"));
        key->setGeometry(QRect(50, 10, 201, 20));
        use_key = new QCheckBox(Trifid);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(290, 10, 70, 21));
        period = new QSpinBox(Trifid);
        period->setObjectName(QStringLiteral("period"));
        period->setGeometry(QRect(50, 40, 42, 22));
        label_2 = new QLabel(Trifid);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 40, 47, 21));

        retranslateUi(Trifid);

        QMetaObject::connectSlotsByName(Trifid);
    } // setupUi

    void retranslateUi(QWidget *Trifid)
    {
        Trifid->setWindowTitle(QApplication::translate("Trifid", "Form", 0));
        label->setText(QApplication::translate("Trifid", "Key:", 0));
        use_key->setText(QApplication::translate("Trifid", "Use Key", 0));
        label_2->setText(QApplication::translate("Trifid", "Period:", 0));
    } // retranslateUi

};

namespace Ui {
    class Trifid: public Ui_Trifid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIFID_H
