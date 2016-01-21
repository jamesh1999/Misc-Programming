#include "Vigenere.h"
#include "ui_Vigenere.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include <regex>
#include "Globals.h"

using namespace Cipher;

std::string VigenereWorker::decrypt(const std::string& key, std::string text)
{
    unsigned cnt = 0;
    for (auto& letter : text)
    {
        letter -= key[cnt] - 'A';
        if (letter < 'A')
            letter += 26;

        ++cnt;
        if (cnt == key.size())
            cnt = 0;
    }
    return text;
}

void VigenereWorker::crack(QString qtext, bool useCrib, QString qcrib)
{
    std::string text = qtext.toStdString();
    std::string crib = qcrib.toStdString();

    //Calculate IC for each key length < 50
    std::vector<double> rates;
    for(int i = useCrib ? crib.size() : 2; i<=50; ++i)
    {
        std::vector<std::string> lists;

        for(unsigned j = 0; j < text.size(); ++j)
        {
            //Add new list if required
            if(lists.size() < (j % i) + 1)
                lists.push_back("");

            lists[j % i]+= text[j]; //Add to list[key index used to encrypt]
        }

        //Calculate ave IC
        double ave_ic = 0;
        for(auto list : lists)
            ave_ic += indexCoincidence(list);
        ave_ic /= i;

        rates.push_back(ave_ic);
    }

    //Find the best key
    double max = rates[0];
    unsigned k_len = 2;

    unsigned cnt = 2;
    for(auto val : rates)
    {
        if (std::abs(ENGLISH_IC - val) < std::abs(ENGLISH_IC - max))
        {
            max = val;
            k_len = cnt;
        }

        ++cnt;
    }

    std::string prev_key;
    std::string cur_key(k_len, 'A');

    //Make changes from crib if set
    if(useCrib)
    {
        for(unsigned i = 0; i < crib.size(); ++i)
        {
            cur_key[i] = 'A' + text[i] - crib[i];
            if(cur_key[i] < 'A')
                cur_key[i] += 26;
        }
    }

    //Keep making changes until no more improvements can be made
    do
    {
        prev_key = cur_key;

        //Loop through letters in key
        for(unsigned i = useCrib ? crib.size() : 0; i < k_len; ++i)
        {
            //Find letter that creates best value
            char max_char = 'A';
            double max_val = -DBL_MAX;
            for(int j = 0; j < 26; ++j)
            {
                cur_key[i] = 'A' + j;
                double val = evaluate(decrypt(cur_key, text));

                if(val > max_val)
                {
                    max_char = 'A' + j;
                    max_val = val;
                }
            }

            cur_key[i] = max_char;
        }
    } while(prev_key != cur_key  && keep_working);

    std::stringstream ss;
    ss << "\nBest score: " << evaluate(decrypt(cur_key, text)) << "\nBest key:  " << cur_key << std::endl;
    emit appendToConsole(QString::fromStdString(ss.str()));

    emit setPlainText(QString::fromStdString(decrypt(cur_key, text)));

    emit finished();
}

void VigenereWorker::useKey(QString key, QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(key.toStdString(), text.toStdString())));
    emit finished();
}

Vigenere::Vigenere(QWidget *parent) : QWidget(parent), ui(new Ui::Vigenere)
{
    cipher_data = {"Vigenere", 26, 42, 65, 74, 22, 26, 8, 42, 438, 106, false};
    ui->setupUi(this);

    //Setup worker thread
    worker = new VigenereWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(QString,QString)), worker, SLOT(useKey(QString,QString)));
    connect(this, SIGNAL(crack(QString,bool,QString)), worker, SLOT(crack(QString,bool,QString)));

    worker_thread.start();
}

Vigenere::~Vigenere()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* Vigenere::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void Vigenere::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
        useKey(ui->key->text(), text);
    else
        crack(text, ui->use_crib->isChecked(), ui->crib->text());
}

void Vigenere::cancel()
{
    worker->cancel();
}

void Cipher::Vigenere::on_key_editingFinished()
{
    //Sanitize key
    std::regex e("[^A-Z]");
    std::string text = ui->key->text().toStdString();
    std::transform(text.begin(), text.end(),text.begin(), toupper);
    ui->key->setText(QString::fromStdString(std::regex_replace(text, e, "")));
}

void Cipher::Vigenere::on_crib_editingFinished()
{
    //Sanitize crib
    std::regex e("[^A-Z]");
    std::string text = ui->crib->text().toStdString();
    std::transform(text.begin(), text.end(),text.begin(), toupper);
    ui->crib->setText(QString::fromStdString(std::regex_replace(text, e, "")));
}
