#include "Trifid.h"
#include "ui_Trifid.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include "Globals.h"

using namespace Cipher;

std::string TrifidWorker::decrypt(int period, const std::string& key, const std::string& text)
{
    std::string result = text;

    for (unsigned i = 0; i < text.length(); i += period)
    {
        if (i + period > text.length())
            period = text.length() - i;

        for (int j = 0; j < period; ++j)
        {
            //Letters that were encoded by the letter i+j
            char a = text[i + (j / 3)];
            char b = text[i + ((j + period) / 3)];
            char c = text[i + ((j + (2 * period)) / 3)];

            //Get the coordinates of the letters that came from letter i+j
            int a_part = key.find_first_of(a) / (int)std::pow(3, (j % 3)) % 3;
            int b_part = key.find_first_of(b) / (int)std::pow(3, ((j + period) % 3)) % 3;
            int c_part = key.find_first_of(c) / (int)std::pow(3, ((j + (2 * period)) % 3)) % 3;

            result[i + j] = key[a_part + b_part * 3 + c_part * 9];
        }
    }

    return result;
}

void TrifidWorker::crack(int period, QString qtext)
{
    std::string text = qtext.toStdString();

    if (period == 0)
        period = text.length();

    //Overall max key
    std::string parent_key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ#";
    std::random_shuffle(parent_key.begin(), parent_key.end());
    double parent_eval = -DBL_MAX;

    //Implement simulated annealing
    for (double T = TEMP; T >= 0 && keep_working; T -= STEP)
    {
        for (unsigned i = 0; i < COUNT; ++i)
        {
            std::string child_key = parent_key;

            //Swap letters
            std::swap(child_key[rand() % 27], child_key[rand() % 27]);

            double eval = evaluate(decrypt(period,child_key, text));

            //If key is better or if temperature is high enough switch
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

void TrifidWorker::useKey(int period, QString key, QString text)
{
    if(period == 0)
        period = text.size();
    emit setPlainText(QString::fromStdString(decrypt(period, key.toStdString(), text.toStdString())));
    emit finished();
}

Trifid::Trifid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Trifid)
{
    cipher_data = {"Trifid", 27, 42, 53, 68, 18, 18, 6, 51, 462, 112, false};
    ui->setupUi(this);

    //Setup worker thread
    worker = new TrifidWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(int,QString,QString)), worker, SLOT(useKey(int,QString,QString)));
    connect(this, SIGNAL(crack(int,QString)), worker, SLOT(crack(int,QString)));

    worker_thread.start();
}

Trifid::~Trifid()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* Trifid::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void Trifid::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
        useKey(ui->period->value(), ui->key->text(), text);
    else
        crack(ui->period->value(), text);
}

void Trifid::cancel()
{
    worker->cancel();
}

void Cipher::Trifid::on_key_editingFinished()
{
    //Sanitize key
    std::string text = "";
    for(char c : ui->key->text().toStdString())
    {
        if('a' <= c && c <= 'z')
            c -= 32;

        if((('A' <= c && c <= 'Z') || c == '#') &&
            std::find(text.begin(), text.end(), c) == text.end())
            text += c;
    }

    //Add missing letters
    for(char c = 'A'; c <= 'Z'; ++c)
    {
        if(std::find(text.begin(), text.end(), c) == text.end())
            text += c;
    }

    if(std::find(text.begin(), text.end(), '#') == text.end())
        text += '#';

    ui->key->setText(QString::fromStdString(text));
}
