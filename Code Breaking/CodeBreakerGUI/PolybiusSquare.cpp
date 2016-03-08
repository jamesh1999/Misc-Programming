#include "PolybiusSquare.h"
#include "ui_PolybiusSquare.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include "Globals.h"

using namespace Cipher;

std::string PolybiusWorker::decrypt(const std::string& key, const std::string& text)
{
    std::string result;
    for (unsigned i = 0; i < text.size(); i += 2)
    {
        //Letters that represent coordinates of letter i
        char a = text[i];
        char b = text[i + 1];

        result += key[5 * (a - 'A') + (b - 'A')];
    }

    return result;
}

void PolybiusWorker::crack(QString qtext)
{
    std::string text = qtext.toStdString();

    //Replace text letters / numbers with ABCDE
    char current = 'A';
    for (char t = '0'; t <= 'Z'; ++t)
    {
        if (text.find(t) != std::string::npos)
        {
            std::size_t found = text.find_first_of(t);
            while (found != std::string::npos)
            {
                text[found] = current;
                found = text.find_first_of(t, found + 1);
            }

            ++current;
        }

        if(t == '9')
            t = 'E';
    }

    //Overall max key
    std::string parent_key = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    double parent_eval = -DBL_MAX;

    std::random_shuffle(parent_key.begin(), parent_key.end());

    //Implement simulated annealing
    for (double T = TEMP; T >= 0 && keep_working; T -= STEP)
    {
        for (unsigned i = 0; i < COUNT; ++i)
        {
            std::string child_key = parent_key;

            //Swap letters
            std::swap(child_key[rand() % 25], child_key[rand() % 25]);

            double eval = evaluate(decrypt(child_key, text));

            //If key is better or if temperature is high enough switch
            if (eval > parent_eval ||
                (T>0 && exp((eval - parent_eval) / T) > (double)rand() / RAND_MAX))
            {
                parent_eval = eval;
                parent_key = child_key;
            }
        }
        std::stringstream ss;
        ss << "\nCurrent score: " << parent_eval << "\nCurrent Temp.: " << T << "\nBest key:  " << parent_key << std::endl;
        emit appendToConsole(QString::fromStdString(ss.str()));

        emit setPlainText(QString::fromStdString(decrypt(parent_key, text)));
    }

    emit finished();
}

void PolybiusWorker::useKey(QString key, QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(key.toStdString(), text.toStdString())));
    emit finished();
}

Polybius::Polybius(QWidget *parent) : QWidget(parent), ui(new Ui::PolybiusSquare)
{
    cipher_data = {"Polybius Square", 25, 63, 72, 94, 44, 43, 11, 50, 688, 188, false}; //
    ui->setupUi(this);

    //Setup worker thread
    worker = new PolybiusWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(QString,QString)), worker, SLOT(useKey(QString,QString)));
    connect(this, SIGNAL(crack(QString)), worker, SLOT(crack(QString)));

    worker_thread.start();
}

Polybius::~Polybius()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* Polybius::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void Polybius::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
        useKey(ui->key->text(), text);
    else
        crack(text);
}

void Polybius::cancel()
{
    worker->cancel();
}

void Cipher::Polybius::on_key_editingFinished()
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
