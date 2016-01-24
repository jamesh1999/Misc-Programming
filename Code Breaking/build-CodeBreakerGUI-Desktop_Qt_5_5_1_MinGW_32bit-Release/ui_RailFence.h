/********************************************************************************
** Form generated from reading UI file 'RailFence.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RAILFENCE_H
#define UI_RAILFENCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RailFence
{
public:
    QSpinBox *rail_count;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *starting_rail;
    QCheckBox *use_key;
    QCheckBox *reversed;

    void setupUi(QWidget *RailFence)
    {
        if (RailFence->objectName().isEmpty())
            RailFence->setObjectName(QStringLiteral("RailFence"));
        RailFence->resize(492, 100);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RailFence->sizePolicy().hasHeightForWidth());
        RailFence->setSizePolicy(sizePolicy);
        RailFence->setMinimumSize(QSize(492, 100));
        RailFence->setMaximumSize(QSize(492, 100));
        rail_count = new QSpinBox(RailFence);
        rail_count->setObjectName(QStringLiteral("rail_count"));
        rail_count->setGeometry(QRect(80, 10, 42, 22));
        rail_count->setMinimum(1);
        label = new QLabel(RailFence);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 61, 20));
        label_2 = new QLabel(RailFence);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 40, 71, 20));
        starting_rail = new QSpinBox(RailFence);
        starting_rail->setObjectName(QStringLiteral("starting_rail"));
        starting_rail->setGeometry(QRect(80, 40, 42, 22));
        use_key = new QCheckBox(RailFence);
        use_key->setObjectName(QStringLiteral("use_key"));
        use_key->setGeometry(QRect(150, 10, 70, 21));
        reversed = new QCheckBox(RailFence);
        reversed->setObjectName(QStringLiteral("reversed"));
        reversed->setGeometry(QRect(150, 40, 101, 21));

        retranslateUi(RailFence);

        QMetaObject::connectSlotsByName(RailFence);
    } // setupUi

    void retranslateUi(QWidget *RailFence)
    {
        RailFence->setWindowTitle(QApplication::translate("RailFence", "Form", 0));
        label->setText(QApplication::translate("RailFence", "Rail Count:", 0));
        label_2->setText(QApplication::translate("RailFence", "Starting Rail:", 0));
        use_key->setText(QApplication::translate("RailFence", "Use Key", 0));
        reversed->setText(QApplication::translate("RailFence", "Reversed Start", 0));
    } // retranslateUi

};

namespace Ui {
    class RailFence: public Ui_RailFence {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAILFENCE_H
