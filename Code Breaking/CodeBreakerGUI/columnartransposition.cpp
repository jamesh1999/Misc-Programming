#include "columnartransposition.h"
#include "ui_columnartransposition.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include "Globals.h"
#include "ColumnView.h"

using namespace Cipher;

std::string ColumnarTranspositionWorker::decrypt(const std::vector<unsigned>& key, const std::string& text, bool readVertical)
{
    std::string decoded = text;
    unsigned val = text.size() / key.size();

    if(readVertical)
    {
        for (unsigned i = 0; i < val; ++i)
            for (unsigned j = 0; j < key.size(); ++j)
                decoded[i * key.size() + j] = text[i + key[j] * val];
    }
    else
    {
        for (unsigned i = 0; i < val; ++i)
            for (unsigned j = 0; j < key.size(); ++j)
                decoded[i * key.size() + j] = text[i * key.size() + key[j]];
    }

    return decoded;
}

void ColumnarTranspositionWorker::crack(QString qtext, bool readVertical)
{
    std::string text = qtext.toStdString();

    //Overall max score
    std::vector<unsigned> top_key;
    double top_score = -DBL_MAX;

    for (unsigned i = 2; i < text.size(); ++i)
    {
        if (text.size() % i == 0)
        {
            //Key for iteration
            std::vector<unsigned> parent_key;
            for (unsigned j = 0; j < i; ++j)
                parent_key.push_back(j);

            while (keep_working && std::next_permutation(parent_key.begin(), parent_key.end()))
            {
                //Evaluate permutation
                double score = evaluate(decrypt(parent_key, text, readVertical));

                //Compare scores changing parent if child is better
                if (score > top_score)
                {
                    top_key = parent_key;
                    top_score = score;
                }
            }

            if (top_key.size() == i)
            {
                std::stringstream ss;
                ss <<  "\nBest key length: " << top_key.size() << "\nBest score: " << top_score << "\nBest key:  ";
                for (unsigned num : top_key)
                {
                    ss << num << ", ";
                }
                ss << std::endl;
                emit appendToConsole(QString::fromStdString(ss.str()));

                emit setPlainText(QString::fromStdString(decrypt(top_key, text, readVertical)));
            }
        }
    }

    emit finished();
}

void ColumnarTranspositionWorker::useKey(QString text, bool readVertical)
{
    emit setPlainText(QString::fromStdString(decrypt(stored_key, text.toStdString(), readVertical)));
    emit finished();
}

ColumnarTransposition::ColumnarTransposition(QWidget *parent) : QWidget(parent), ui(new Ui::ColumnarTransposition)
{
    cipher_data = {"Columnar Transposition", 26, 63, 73, 96, 41, 41, 11, 50, 653, 128, true};
    ui->setupUi(this);

    //Setup worker thread
    worker = new ColumnarTranspositionWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(QString, bool)), worker, SLOT(useKey(QString, bool)));
    connect(this, SIGNAL(crack(QString, bool)), worker, SLOT(crack(QString, bool)));

    worker_thread.start();
}

ColumnarTransposition::~ColumnarTransposition()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* ColumnarTransposition::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void ColumnarTransposition::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
    {
        worker->stored_key = stored_key;
        useKey(text, ui->checkBox->isChecked());
    }
    else
        crack(text, ui->checkBox->isChecked());
}

void ColumnarTransposition::cancel()
{
    worker->cancel();
}

void ColumnarTransposition::getStoredKey(std::vector<QString> key)
{
    stored_key.clear();
    for(QString val : key)
        stored_key.push_back(val.toInt());
}

void Cipher::ColumnarTransposition::on_edit_key_clicked()
{
    ColumnView* editor = new ColumnView(this);
    connect(editor, SIGNAL(returnVector(std::vector<QString>)), this, SLOT(getStoredKey(std::vector<QString>)));

    std::vector<QString> values;
    for(int i = 0; i < ui->column_count->value(); ++i)
        values.push_back(QString::number(i));
    editor->setValues(values);
    editor->exec();
}
