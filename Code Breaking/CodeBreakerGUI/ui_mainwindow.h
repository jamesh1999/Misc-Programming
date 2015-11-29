/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CodeBreakerMain
{
public:
    QAction *actionOpen;
    QAction *actionCipher;
    QAction *actionClose;
    QAction *actionCipher_2;
    QAction *actionGlobal_Settings;
    QWidget *centralWidget;
    QPushButton *pushButton;
    QComboBox *comboBox;
    QLabel *label;
    QFrame *line;
    QPlainTextEdit *plainTextEdit;
    QLabel *label_2;
    QLabel *label_3;
    QPlainTextEdit *plainTextEdit_2;
    QTextBrowser *textBrowser;
    QFrame *line_2;
    QPushButton *pushButton_2;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;
    QProgressBar *progressBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOptions;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CodeBreakerMain)
    {
        if (CodeBreakerMain->objectName().isEmpty())
            CodeBreakerMain->setObjectName(QStringLiteral("CodeBreakerMain"));
        CodeBreakerMain->resize(833, 589);
        actionOpen = new QAction(CodeBreakerMain);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionCipher = new QAction(CodeBreakerMain);
        actionCipher->setObjectName(QStringLiteral("actionCipher"));
        actionClose = new QAction(CodeBreakerMain);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionCipher_2 = new QAction(CodeBreakerMain);
        actionCipher_2->setObjectName(QStringLiteral("actionCipher_2"));
        actionGlobal_Settings = new QAction(CodeBreakerMain);
        actionGlobal_Settings->setObjectName(QStringLiteral("actionGlobal_Settings"));
        centralWidget = new QWidget(CodeBreakerMain);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(750, 10, 75, 41));
        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(498, 20, 151, 22));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(450, 20, 41, 21));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 50, 811, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 100, 811, 111));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 70, 61, 21));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 220, 61, 21));
        plainTextEdit_2 = new QPlainTextEdit(centralWidget);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(10, 250, 811, 111));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(10, 400, 811, 141));
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(10, 370, 811, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(660, 10, 75, 41));
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(350, 20, 70, 21));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(172, 20, 161, 20));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(20, 20, 131, 21));
        progressBar->setValue(0);
        CodeBreakerMain->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CodeBreakerMain);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 833, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        CodeBreakerMain->setMenuBar(menuBar);
        statusBar = new QStatusBar(CodeBreakerMain);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CodeBreakerMain->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionCipher);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuOptions->addAction(actionCipher_2);
        menuOptions->addAction(actionGlobal_Settings);

        retranslateUi(CodeBreakerMain);

        QMetaObject::connectSlotsByName(CodeBreakerMain);
    } // setupUi

    void retranslateUi(QMainWindow *CodeBreakerMain)
    {
        CodeBreakerMain->setWindowTitle(QApplication::translate("CodeBreakerMain", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("CodeBreakerMain", "Open...", 0));
        actionCipher->setText(QApplication::translate("CodeBreakerMain", "Save...", 0));
        actionClose->setText(QApplication::translate("CodeBreakerMain", "Close", 0));
        actionCipher_2->setText(QApplication::translate("CodeBreakerMain", "Cipher", 0));
        actionGlobal_Settings->setText(QApplication::translate("CodeBreakerMain", "Global Settings", 0));
        pushButton->setText(QApplication::translate("CodeBreakerMain", "Decrypt", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("CodeBreakerMain", "Amsco", 0)
         << QApplication::translate("CodeBreakerMain", "Bifid", 0)
         << QApplication::translate("CodeBreakerMain", "Binary Conversion", 0)
         << QApplication::translate("CodeBreakerMain", "Cadenus", 0)
         << QApplication::translate("CodeBreakerMain", "Columnar Transposition", 0)
         << QApplication::translate("CodeBreakerMain", "Four Square", 0)
         << QApplication::translate("CodeBreakerMain", "Hill", 0)
         << QApplication::translate("CodeBreakerMain", "Playfair", 0)
         << QApplication::translate("CodeBreakerMain", "Polybius Square", 0)
         << QApplication::translate("CodeBreakerMain", "Rail Fence", 0)
         << QApplication::translate("CodeBreakerMain", "Simple Substitution", 0)
         << QApplication::translate("CodeBreakerMain", "Trifid", 0)
         << QApplication::translate("CodeBreakerMain", "Vigenere", 0)
        );
        label->setText(QApplication::translate("CodeBreakerMain", "Cipher:", 0));
        label_2->setText(QApplication::translate("CodeBreakerMain", "Ciphertext:", 0));
        label_3->setText(QApplication::translate("CodeBreakerMain", "Plaintext:", 0));
        pushButton_2->setText(QApplication::translate("CodeBreakerMain", "Guess Cipher", 0));
        checkBox->setText(QApplication::translate("CodeBreakerMain", "Use Key", 0));
        menuFile->setTitle(QApplication::translate("CodeBreakerMain", "File", 0));
        menuOptions->setTitle(QApplication::translate("CodeBreakerMain", "Options", 0));
    } // retranslateUi

};

namespace Ui {
    class CodeBreakerMain: public Ui_CodeBreakerMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
