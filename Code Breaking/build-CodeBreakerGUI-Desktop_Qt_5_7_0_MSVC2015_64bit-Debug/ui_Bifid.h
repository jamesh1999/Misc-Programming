/********************************************************************************
** Form generated from reading UI file 'Bifid.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BIFID_H
#define UI_BIFID_H

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

class Ui_Bifid
{
public:
    QCheckBox *use_key;
    QLineEdit *key;
    QLabel *label_5;
    QLabel *label_7;
    QSpinBox *period;

    void setupUi(QWidget *Bifid)
    {
        if (Bifid->objectName().isEmpty())
            Bifid->setObjectName(QStringLiteral("Bifid"));
        Bifid->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Bifid->sizePolicy().hasHeightForWidth());
        Bifid->setSizePolicy(sizePolicy);
        Bifid->setMinimumSize(QSize(492, 100));
        Bifid->setMaximumSize(QSize(492, 100));
        use_key = new QCheckBox(Bifid);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(290, 10, 70, 21));
        key = new QLineEdit(Bifid);
        key->setObjectName(QStringLiteral("key"));
        key->setGeometry(QRect(50, 10, 201, 20));
        label_5 = new QLabel(Bifid);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 47, 16));
        label_7 = new QLabel(Bifid);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 35, 47, 31));
        period = new QSpinBox(Bifid);
        period->setObjectName(QStringLiteral("period"));
        period->setGeometry(QRect(50, 40, 42, 22));

        retranslateUi(Bifid);

        QMetaObject::connectSlotsByName(Bifid);
    } // setupUi

    void retranslateUi(QWidget *Bifid)
    {
        Bifid->setWindowTitle(QApplication::translate("Bifid", "Form", 0));
        use_key->setText(QApplication::translate("Bifid", "Use Key", 0));
        label_5->setText(QApplication::translate("Bifid", "Key:", 0));
        label_7->setText(QApplication::translate("Bifid", "Period:", 0));
    } // retranslateUi

};

namespace Ui {
    class Bifid: public Ui_Bifid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIFID_H
