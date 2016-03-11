#include "Hill.h"
#include "ui_Hill.h"
#include <iostream>
#include <sstream>
#include <float.h>
#include "Globals.h"

using namespace Cipher;

std::string HillWorker::decrypt(unsigned section_length, const std::vector<unsigned>& matrix, const std::string& text)
{
    //Perform matrix multiplication
    std::vector<unsigned> decrypt(text.size(), 0);
    for(unsigned i = 0; i < text.size(); i += section_length)
        for(unsigned j = 0; matrix.size(); ++j)
        {
            decrypt[i + (j % section_length)] += (text[i + (j % section_length)] - 'A') * matrix[j];
        }

    //Create return string
    std::string ret;
    ret.reserve(text.size());
    for(auto val : decrypt)
        ret += static_cast<char>((val % 26) + 'A');

    return ret;
}

void HillWorker::crack(bool at_beginning, int size, QString qtext, QString qcrib)
{
    std::string text = qtext.toStdString();
    std::string crib = qcrib.toStdString();

    //Crib is long enough to confirm
    if(at_beginning && crib.size() >= size * size)
    {

    }
    else if(crib.size() > size * size)
    {

    }
    //Crib will need scoring to confirm
    else if(crib.size() == size * size)
    {

    }
    //Partial crib: last column of matrix can be calculated
    else if(at_beginning && crib.size() > size)
    {

    }
    //Brute force
    else
    {

    }

    emit finished();
}

void HillWorker::useKey(QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(static_cast<int>(std::sqrt(stored_key.size())), stored_key, text.toStdString())));
    emit finished();
}

Hill::Hill(QWidget *parent) : QWidget(parent), ui(new Ui::Hill)
{
    cipher_data = {"Hill", 26, 63, 73, 95, 72, 73, 22, 50, 756, 303, false};
    ui->setupUi(this);

    //Setup worker thread
    worker = new HillWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(QString)), worker, SLOT(useKey(QString)));
    connect(this, SIGNAL(crack(bool,int,QString,QString)), worker, SLOT(crack(bool,int,QString,QString)));

    worker_thread.start();
}

Hill::~Hill()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* Hill::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void Hill::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
        useKey(text);
    else
        crack(ui->use_crib->isChecked(),ui->key_size->value(),ui->crib->text(),text);
}

void Hill::cancel()
{
    worker->cancel();
}
