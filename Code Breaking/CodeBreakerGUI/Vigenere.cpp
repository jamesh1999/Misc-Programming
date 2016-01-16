#include "Vigenere.h"
#include "ui_Vigenere.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
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

void VigenereWorker::crack(QString qtext)
{
    std::string text = qtext.toStdString();
    //Calculate IC for each key length < 50
    std::vector<double> rates;
    for(int i = 2; i<=50; ++i)
    {
        std::vector<std::string> lists;

        unsigned long cnt = 0;
        for(auto letter : text)
        {
            //Add new list if required
            if(lists.size()<(cnt%i)+1)
            {
                lists.push_back("");
            }

            lists[cnt%i]+=letter; //Add to list[key index used to encrypt]

            ++cnt;
        }

        //Calculate ave IC
        double ave_index_coincidence = 0;
        for(auto list : lists)
        {
            ave_index_coincidence+=indexCoincidence(list);
        }
        ave_index_coincidence/=i;

        rates.push_back(ave_index_coincidence);
    }

    //Find the first key whose value is very good
    double max = 1000.0;
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

    std::string prev_key(k_len, 'A');
    std::string cur_key(k_len, 'A');

    //Keep making changes until no more improvements can be made
    bool same = false;
    while(!same)
    {
        //Loop through letters in key
        for(unsigned i = 0; i < k_len; ++i)
        {
            //Find letter that creates best value
            char max_char = 'A';
            double max_char_val = -DBL_MAX;
            for(int j = 0; j<26; ++j)
            {
                cur_key[i] = 'A'+j;
                double eval = evaluate(decrypt(cur_key, text));

                if(eval>max_char_val)
                {
                    max_char = 'A' + j;
                    max_char_val = eval;
                }
            }

            cur_key[i] = max_char;
        }

        if(cur_key==prev_key)
        {
            same = true;
        }
        else
        {
            prev_key = cur_key;
        }
    }
    std::string decrypted = decrypt(cur_key, text);

    std::stringstream ss;
    ss << "\nBest score: " << evaluate(decrypted) << "\nBest key:  " << cur_key << "\nKey length used:  " << k_len << std::endl;
    emit appendToConsole(QString::fromStdString(ss.str()));

    emit setPlainText(QString::fromStdString(decrypt(cur_key, text)));
}

void VigenereWorker::useKey(QString key, QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(key.toStdString(), text.toStdString())));
}

Vigenere::Vigenere(QWidget *parent) : QWidget(parent), ui(new Ui::Vigenere)
{
    cipher_data = {"Vigenere", 26, 42, 65, 74, 22, 26, 8, 42, 438, 106, false};
    ui->setupUi(this);

    worker = new VigenereWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(QString,QString)), worker, SLOT(useKey(QString,QString)));
    connect(this, SIGNAL(crack(QString)), worker, SLOT(crack(QString)));

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
    if(ui->use_key->isChecked())
        useKey(ui->key->text(), text);
    else
        crack(text);
}
