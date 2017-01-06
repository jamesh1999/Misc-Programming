#include "Bifid.h"
#include "ui_Bifid.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include "Globals.h"

using namespace Cipher;

std::string BifidWorker::decrypt(int period, const std::string& key, const std::string& text)
{
    std::string result;
    result.resize(text.size());

    for (unsigned i = 0; i < text.length(); i += period)
    {
        if (i + period > text.length())
            period = text.length() - i;

        for (int j = 0; j < period; ++j)
        {
            //Letters that were encoded by the letter i+j
            char a = text[i + (j / 2)];
            char b = text[i + ((j + period) / 2)];

            //Get the coordinates of the letters that came from letter i+j
            int a_part = key.find_first_of(a) / (j % 2
                                                 ? 1
                                                 : 5) % 5;
            int b_part = key.find_first_of(b) / ((j + period) % 2
                                                 ? 1
                                                 : 5) % 5;

            result[i + j] = key[a_part * 5 + b_part];
        }
     }
     return result;
}

void BifidWorker::crack(int period, QString qtext, QString key)
{
    std::string text = qtext.toStdString();

    if (period == 0)
        period = text.length();

    //Overall max key
    std::string parent_key = key.toStdString();
    std::random_shuffle(parent_key.begin(), parent_key.end());
    double parent_eval = -DBL_MAX;

    //Implement simulated annealing
    for (double T = TEMP; T >= 0 && keep_working; T -= STEP)
    {
        for (unsigned i = 0; i < COUNT; ++i)
        {
            std::string child_key = parent_key;

            //Swap letters
            std::swap(child_key[rand() % 25], child_key[rand() % 25]);

            double eval = evaluate(decrypt(period,child_key, text));

            //If key is better then always switch
            //Otherwise switch if temperature is high enough
            if (eval > parent_eval ||
                (T>0 && exp((eval - parent_eval) / T) > (double)rand() / RAND_MAX))
            {
                parent_eval = eval;
                parent_key = child_key;
            }
        }

        std::stringstream ss;
        ss << "\nPeriod: " << period << "\nCurrent score: " << parent_eval << "\nCurrent Temp.: " << T << "\nBest key:  " << parent_key << std::endl;
        emit appendToConsole(QString::fromStdString(ss.str()));

        emit setPlainText(QString::fromStdString(decrypt(period,parent_key, text)));
    }

    emit finished();
}

void BifidWorker::useKey(int period, QString key, QString text)
{
    if(period == 0)
        period = text.size();
    emit setPlainText(QString::fromStdString(decrypt(period, key.toStdString(), text.toStdString())));
    emit finished();
}

Bifid::Bifid(QWidget *parent) : QWidget(parent), ui(new Ui::Bifid)
{
    cipher_data = {"Bifid", 25, 47, 58, 77, 24, 23, 7, 49, 517, 118, false};
    ui->setupUi(this);

    //Setup worker thread
    worker = new BifidWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(int,QString,QString)), worker, SLOT(useKey(int,QString,QString)));
    connect(this, SIGNAL(crack(int,QString,QString)), worker, SLOT(crack(int,QString,QString)));

    worker_thread.start();
}

Bifid::~Bifid()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* Bifid::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void Bifid::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
        useKey(ui->period->value(), ui->key->text(), text);
    else
        crack(ui->period->value(), text, getBaseKey(text));
}

void Bifid::cancel()
{
    worker->cancel();
}

//Get missing letter and generate base key from it
QString Bifid::getBaseKey(QString text)
{
    char missing = 'A';
    for(; missing <= 'Z'; ++missing)
    {
        if(std::find(text.begin(), text.end(), missing) == text.end())
            break;
    }

    QString key;
    for(char c = 'A'; c <= 'Z'; ++c)
        if(c != missing)
            key += c;
    return key;
}

void Cipher::Bifid::on_key_editingFinished()
{
    //Sanitize key
    std::string text = "";
    for(char c : ui->key->text().toStdString())
    {
        if('a' <= c && c <= 'z')
            c -= 32;

        if('A' <= c && c <= 'Z' &&
            std::find(text.begin(), text.end(), c) == text.end())
            text += c;
    }

    //Identify omitted letter
    char missing;
    if(std::find(text.begin(), text.end(), 'J') == text.end())
        missing = 'J';
    else
    {
        missing = 'A';
        for(; missing <= 'Z'; ++missing)
        {
            if(std::find(text.begin(), text.end(), missing) == text.end())
                break;
        }
    }

    //Add other missing letters
    for(char c = 'A'; c <= 'Z'; ++c)
    {
        if(c == missing) continue;

        if(std::find(text.begin(), text.end(), c) == text.end())
            text += c;
    }

    ui->key->setText(QString::fromStdString(text));
}
