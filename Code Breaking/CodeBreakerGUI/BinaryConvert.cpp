#include "BinaryConvert.h"
#include "ui_BinaryConvert.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include <bitset>
#include "Globals.h"

using namespace Cipher;

void BinaryWorker::decode(unsigned length, unsigned method, QString qtext)
{
    std::string text = qtext.toStdString();
    std::string result;
    for (unsigned i = 0; i < text.size(); i += length)
    {
        std::string next = text.substr(i, length);
        std::bitset<8> val(next);

        switch(method)
        {
        case 0:
            result += static_cast<char>(val.to_ulong());
            break;
        case 1:
            result += static_cast<char>(val.to_ulong() + 'A');
            break;
        case 2:
            result += static_cast<char>(val.to_ulong() + 'A' - 1);
            break;
        }
    }

    emit setPlainText(QString::fromStdString(result));
    emit finished();
}

BinaryConvert::BinaryConvert(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinaryConvert)
{
    cipher_data = {"Binary Conversion", 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, false};
    ui->setupUi(this);

    //Setup worker thread
    worker = new BinaryWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(decode(unsigned,unsigned,QString)), worker, SLOT(decode(unsigned,unsigned,QString)));

    worker_thread.start();
}

BinaryConvert::~BinaryConvert()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* BinaryConvert::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void BinaryConvert::start(QString text)
{
    worker->reset();
    decode(ui->letter_length->value(), ui->method->currentIndex(), text);
}

void BinaryConvert::cancel()
{
    worker->cancel();
}
