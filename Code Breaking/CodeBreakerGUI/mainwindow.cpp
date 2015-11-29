#include <regex>
#include <iostream>
#include "decrypt.h"
#include "mainwindow.h"
#include "Globals.h"
#include "ui_mainwindow.h"
#include "QThread"
#include "globalsettings.h"
#include "guesscipher.h"

CodeBreakerMain::CodeBreakerMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::CodeBreakerMain)
{
    ui->setupUi(this);
}

CodeBreakerMain::~CodeBreakerMain()
{
    delete ui;
}

void CodeBreakerMain::setPlainTextOutput(QString text)
{
    ui->plainTextEdit_2->setPlainText(text);
}

void CodeBreakerMain::appendToConsole(QString text)
{
    ui->textBrowser->append(text);
}

void CodeBreakerMain::on_pushButton_clicked()
{
    decryptWithCipher(ui->comboBox->currentIndex());
}

void CodeBreakerMain::on_actionGlobal_Settings_triggered()
{
    GlobalSettings* pDialog = new GlobalSettings(this);
    pDialog->exec();
}

void CodeBreakerMain::on_actionClose_triggered()
{
    this->close();
}

void CodeBreakerMain::on_pushButton_2_clicked()
{
    GuessCipher* pDialog = new GuessCipher(this);

    std::regex e("[^A-Z0-9#]");
    std::string text = ui->plainTextEdit->toPlainText().toStdString();
    std::transform(text.begin(), text.end(),text.begin(), toupper);
    pDialog->text = std::regex_replace(text, e, "");

    connect(pDialog, &GuessCipher::decryptWithCipher, this, &CodeBreakerMain::decryptWithCipher);

    pDialog->guessCipher();
    pDialog->exec();
}

void CodeBreakerMain::decryptWithCipher(int code)
{
    ui->textBrowser->setText("");

    std::regex e("[^A-Z0-9#]");
    std::string text = ui->plainTextEdit->toPlainText().toStdString();
    std::transform(text.begin(), text.end(),text.begin(), toupper);

    QThread* pThread = new QThread;
    pWorker = new DecryptionWorker;

    pWorker->text = std::regex_replace(text, e, "");
    pWorker->cipher = code;
    pWorker->moveToThread(pThread);

    connect(pWorker, &DecryptionWorker::appendToConsole, this, &CodeBreakerMain::appendToConsole);
    connect(pWorker, &DecryptionWorker::setPlainTextOutput, this, &CodeBreakerMain::setPlainTextOutput);
    connect(pThread, &QThread::started, pWorker, &DecryptionWorker::decryptCipher);
    connect(pWorker, &DecryptionWorker::Finished, pThread, &QThread::quit);

    pThread->start();
}
