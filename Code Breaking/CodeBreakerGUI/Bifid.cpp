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
    std::string result = text;

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
            int a_part = key.find_first_of(a) / (int)std::pow(5, (j % 2)) % 5;
            int b_part = key.find_first_of(b) / (int)std::pow(5, ((j + period) % 2)) % 5;

            result[i + j] = key[a_part + b_part * 5];
        }
    }
    return result;
}

void BifidWorker::crack(int period, QString qtext)
{
    std::string text = qtext.toStdString();

    if (period == 0)
        period = text.length();

    //Overall max key
    std::string parent_key = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    std::random_shuffle(parent_key.begin(), parent_key.end());
    double parent_eval = -DBL_MAX;

    //Implement simulated annealing
    for (double T = TEMP; T >= 0; T -= STEP)
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
                T>0 && exp((eval - parent_eval) / T) > 1.0*rand() / (double)RAND_MAX)
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
}

void BifidWorker::useKey(int period, QString key, QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(period, key.toStdString(), text.toStdString())));
}

Bifid::Bifid(QWidget *parent) : QWidget(parent), ui(new Ui::Bifid)
{
    cipher_data = {"Bifid", 25, 47, 58, 77, 24, 23, 7, 49, 517, 118, false};
    ui->setupUi(this);

    worker = new BifidWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(int,QString,QString)), worker, SLOT(useKey(int,QString,QString)));
    connect(this, SIGNAL(crack(int,QString)), worker, SLOT(crack(int,QString)));

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
    if(ui->use_key->isChecked())
        useKey(ui->period->value(), ui->key->text(), text);
    else
        crack(ui->period->value(), text);
}
