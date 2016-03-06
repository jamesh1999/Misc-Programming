/********************************************************************************
** Form generated from reading UI file 'CodeBreakerMain.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CODEBREAKERMAIN_H
#define UI_CODEBREAKERMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QColumnView>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
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
    QAction *actionAdd_Spaces;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QFrame *settings;
    QHBoxLayout *horizontalLayout_2;
    QStackedWidget *stackedWidget;
    QFrame *frame;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *guess_cipher;
    QPushButton *decrypt;
    QComboBox *cipher;
    QLabel *label_2;
    QTextEdit *ciphertext;
    QLabel *label;
    QTextEdit *plaintext;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QColumnView *columnView;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout;
    QLabel *label_4;
    QTextBrowser *textBrowser;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOptions;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CodeBreakerMain)
    {
        if (CodeBreakerMain->objectName().isEmpty())
            CodeBreakerMain->setObjectName(QStringLiteral("CodeBreakerMain"));
        CodeBreakerMain->setEnabled(true);
        CodeBreakerMain->resize(930, 644);
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
        actionAdd_Spaces = new QAction(CodeBreakerMain);
        actionAdd_Spaces->setObjectName(QStringLiteral("actionAdd_Spaces"));
        centralWidget = new QWidget(CodeBreakerMain);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        settings = new QFrame(centralWidget);
        settings->setObjectName(QStringLiteral("settings"));
        settings->setMinimumSize(QSize(0, 100));
        settings->setFrameShape(QFrame::StyledPanel);
        settings->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(settings);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(settings);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));

        horizontalLayout_2->addWidget(stackedWidget);

        frame = new QFrame(settings);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setEnabled(true);
        sizePolicy.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy);
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        guess_cipher = new QPushButton(frame_3);
        guess_cipher->setObjectName(QStringLiteral("guess_cipher"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(guess_cipher->sizePolicy().hasHeightForWidth());
        guess_cipher->setSizePolicy(sizePolicy1);
        guess_cipher->setMinimumSize(QSize(100, 50));

        horizontalLayout_3->addWidget(guess_cipher);

        decrypt = new QPushButton(frame_3);
        decrypt->setObjectName(QStringLiteral("decrypt"));
        sizePolicy1.setHeightForWidth(decrypt->sizePolicy().hasHeightForWidth());
        decrypt->setSizePolicy(sizePolicy1);
        decrypt->setMinimumSize(QSize(100, 50));

        horizontalLayout_3->addWidget(decrypt);


        verticalLayout_4->addWidget(frame_3);

        cipher = new QComboBox(frame);
        cipher->setObjectName(QStringLiteral("cipher"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cipher->sizePolicy().hasHeightForWidth());
        cipher->setSizePolicy(sizePolicy2);
        cipher->setMinimumSize(QSize(0, 22));
        cipher->setStyleSheet(QLatin1String("QComboBox\n"
"{\n"
"margin-left: 10px;\n"
"margin-right: 10px;\n"
"padding-left: 2px;\n"
"}"));

        verticalLayout_4->addWidget(cipher);


        horizontalLayout_2->addWidget(frame);


        verticalLayout_2->addWidget(settings);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        ciphertext = new QTextEdit(centralWidget);
        ciphertext->setObjectName(QStringLiteral("ciphertext"));

        verticalLayout_2->addWidget(ciphertext);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_2->addWidget(label);

        plaintext = new QTextEdit(centralWidget);
        plaintext->setObjectName(QStringLiteral("plaintext"));

        verticalLayout_2->addWidget(plaintext);

        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame_5 = new QFrame(frame_2);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_5);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(frame_5);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        columnView = new QColumnView(frame_5);
        columnView->setObjectName(QStringLiteral("columnView"));

        verticalLayout_3->addWidget(columnView);


        horizontalLayout->addWidget(frame_5);

        frame_4 = new QFrame(frame_2);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_4);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_4 = new QLabel(frame_4);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout->addWidget(label_4);

        textBrowser = new QTextBrowser(frame_4);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout->addWidget(textBrowser);


        horizontalLayout->addWidget(frame_4);


        verticalLayout_2->addWidget(frame_2);

        CodeBreakerMain->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CodeBreakerMain);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 930, 21));
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
        menuOptions->addAction(actionAdd_Spaces);

        retranslateUi(CodeBreakerMain);

        stackedWidget->setCurrentIndex(-1);
        cipher->setCurrentIndex(-1);


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
        actionAdd_Spaces->setText(QApplication::translate("CodeBreakerMain", "Add Spaces", 0));
        guess_cipher->setText(QApplication::translate("CodeBreakerMain", "Guess Cipher", 0));
        decrypt->setText(QApplication::translate("CodeBreakerMain", "Decrypt", 0));
        label_2->setText(QApplication::translate("CodeBreakerMain", "Ciphertext:", 0));
        label->setText(QApplication::translate("CodeBreakerMain", "Plaintext:", 0));
        label_3->setText(QApplication::translate("CodeBreakerMain", "Best Keys:", 0));
        label_4->setText(QApplication::translate("CodeBreakerMain", "Output Log:", 0));
        menuFile->setTitle(QApplication::translate("CodeBreakerMain", "File", 0));
        menuOptions->setTitle(QApplication::translate("CodeBreakerMain", "Options", 0));
    } // retranslateUi

};

namespace Ui {
    class CodeBreakerMain: public Ui_CodeBreakerMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CODEBREAKERMAIN_H
