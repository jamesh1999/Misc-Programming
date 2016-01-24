/********************************************************************************
** Form generated from reading UI file 'GuessCipher.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUESSCIPHER_H
#define UI_GUESSCIPHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GuessCipher
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QListWidget *listWidget;
    QLabel *label;
    QLabel *selection;

    void setupUi(QDialog *GuessCipher)
    {
        if (GuessCipher->objectName().isEmpty())
            GuessCipher->setObjectName(QStringLiteral("GuessCipher"));
        GuessCipher->resize(400, 300);
        pushButton = new QPushButton(GuessCipher);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(310, 270, 75, 23));
        pushButton_2 = new QPushButton(GuessCipher);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(184, 270, 111, 23));
        listWidget = new QListWidget(GuessCipher);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(10, 10, 381, 251));
        label = new QLabel(GuessCipher);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 270, 47, 16));
        selection = new QLabel(GuessCipher);
        selection->setObjectName(QStringLiteral("selection"));
        selection->setGeometry(QRect(60, 270, 121, 16));

        retranslateUi(GuessCipher);

        QMetaObject::connectSlotsByName(GuessCipher);
    } // setupUi

    void retranslateUi(QDialog *GuessCipher)
    {
        GuessCipher->setWindowTitle(QApplication::translate("GuessCipher", "Dialog", 0));
        pushButton->setText(QApplication::translate("GuessCipher", "Cancel", 0));
        pushButton_2->setText(QApplication::translate("GuessCipher", "Decrypt With Cipher", 0));
        label->setText(QApplication::translate("GuessCipher", "Selected:", 0));
        selection->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GuessCipher: public Ui_GuessCipher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUESSCIPHER_H
