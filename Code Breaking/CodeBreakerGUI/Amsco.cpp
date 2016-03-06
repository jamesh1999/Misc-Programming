#include "Amsco.h"
#include "ui_Amsco.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include "Globals.h"
#include "ColumnView.h"

using namespace Cipher;

std::string AmscoWorker::decrypt(bool swapVertical, int start, const std::vector<unsigned>& key, const std::string& text)
{
    //Compute size of each column
    std::vector<unsigned> col_lengths(key.size(), 0);
    unsigned cur_col = 0;
    unsigned index = 0;
    unsigned size = start;
    while (index < text.size())
    {
        col_lengths[cur_col] += std::min(size, (unsigned)text.size() - index);

        index += size;

        if (size == 2)
            size = 1;
        else
            size = 2;

        ++cur_col;
        cur_col %= key.size();

        //If restarted & swapVertical switch size if necessary
        if(cur_col == 0 && swapVertical && key.size() % 2 == 0)
        {
            if (size == 2)
                size = 1;
            else
                size = 2;
        }
    }

    //Split text into columns
    std::vector<std::string> cols(key.size(), "");
    unsigned total = 0;
    for (unsigned i = 0; i < key.size(); ++i)
    {
        cols[key[i]] = text.substr(total, col_lengths[key[i]]);
        total += col_lengths[key[i]];
    }

    //Recombine columns into string
    std::string decoded;
    size = start;
    cur_col = 0;
    while (decoded.size() < text.size())
    {
        decoded += cols[cur_col].substr(0, size);
        cols[cur_col].erase(0, size);

        ++cur_col;
        cur_col %= key.size();

        if (size == 2)
            size = 1;
        else
            size = 2;

        //If restarted & swapVertical switch size if necessary
        if(cur_col == 0 && swapVertical && key.size() % 2 == 0)
        {
            if (size == 2)
                size = 1;
            else
                size = 2;
        }
    }

    return decoded;
}

void AmscoWorker::crack(bool swapVertical, QString qtext)
{
    std::string text = qtext.toStdString();

    //Overall max score
    int top_i = 0;
    std::vector<unsigned> top_key;
    double top_score = -DBL_MAX;
    for (unsigned i = 2; i < text.size(); ++i)
    {
        //Key for iteration
        std::vector<unsigned> parent_key;
        for (unsigned j = 0; j < i; ++j)
            parent_key.push_back(j);

        double child_score;
        while (keep_working && std::next_permutation(parent_key.begin(), parent_key.end()))
        {
            //Evaluate new key
            child_score = evaluate(decrypt(swapVertical, 1, parent_key, text));

            if (child_score > top_score)
            {
                top_i = 1;
                top_key = parent_key;
                top_score = child_score;
            }

            //Evaluate new key
            child_score = evaluate(decrypt(swapVertical, 2, parent_key, text));

            if (child_score > top_score)
            {
                top_i = 2;
                top_key = parent_key;
                top_score = child_score;
            }
        }

        if (top_key.size() == i)
        {
            std::stringstream ss;
            ss << "\nBest key length: " << top_key.size() << "\nBest score: " << top_score << "\nBest key:  ";
            for (unsigned num : top_key)
                ss << num << ", ";
            ss << "\nBest start: " << top_i << std::endl;

            emit appendToConsole(QString::fromStdString(ss.str()));

            emit setPlainText(QString::fromStdString(decrypt(swapVertical, top_i, top_key, text)));
        }
    }

    emit finished();
}

void AmscoWorker::useKey(bool startPair, bool swapVertical, QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(swapVertical, startPair ? 2 : 1, stored_key, text.toStdString())));
    emit finished();
}

Amsco::Amsco(QWidget *parent) : QWidget(parent), ui(new Ui::Amsco)
{
    cipher_data = {"Amsco", 26, 63, 72, 94, 44, 43, 11, 50, 688, 188, true};
    ui->setupUi(this);

    //Setup worker thread
    worker = new AmscoWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(bool,bool,QString)), worker, SLOT(useKey(bool,bool,QString)));
    connect(this, SIGNAL(crack(bool,QString)), worker, SLOT(crack(bool,QString)));

    worker_thread.start();
}

Amsco::~Amsco()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* Amsco::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void Amsco::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
    {
        worker->stored_key = stored_key;
        useKey(ui->begin_pair->isChecked(), ui->vertical_swap->isChecked(), text);
    }
    else
        crack(ui->vertical_swap->isChecked(), text);
}

void Amsco::cancel()
{
    worker->cancel();
}

void Amsco::getStoredKey(std::vector<QString> key)
{
    stored_key.clear();
    for(QString val : key)
        stored_key.push_back(val.toInt());
}

void Cipher::Amsco::on_edit_key_clicked()
{
    ColumnView* editor = new ColumnView(this);
    connect(editor, SIGNAL(returnVector(std::vector<QString>)), this, SLOT(getStoredKey(std::vector<QString>)));

    std::vector<QString> values;
    for(int i = 0; i < ui->column_count->value(); ++i)
        values.push_back(QString::number(i));
    editor->setValues(values);
    editor->exec();
}
