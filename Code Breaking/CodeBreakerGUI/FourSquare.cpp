#include "FourSquare.h"
#include "ui_FourSquare.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include "Globals.h"

using namespace Cipher;

std::string FourSquareWorker::decrypt(const std::string& alpha, const std::string& key1, const std::string& key2, std::string text)
{
    for (unsigned i = 0; i < text.size() - 1; i += 2)
    {
        int a_ind = key1.find_first_of(text[i]);
        int b_ind = key2.find_first_of(text[i + 1]);
        text[i]     = alpha[5 * (a_ind / 5) + (b_ind % 5)]; //Work out row and height from keys then decrypt
        text[i + 1] = alpha[5 * (b_ind / 5) + (a_ind % 5)]; //"
    }

    return text;
}

void FourSquareWorker::crack(QString qtext, QString key)
{
    std::string text = qtext.toStdString();
    std::string alpha = key.toStdString();
    std::string parent_key1 = key.toStdString();
    std::string parent_key2 = key.toStdString();
    double parent_eval = -DBL_MAX;

    std::random_shuffle(parent_key1.begin(), parent_key1.end());
    std::random_shuffle(parent_key2.begin(), parent_key2.end());

    //Implement simulated annealing
    for (double T = TEMP; T >= 0 && keep_working; T -= STEP)
    {
        for (unsigned i = 0; i < COUNT; ++i)
        {
            std::string child_key1 = parent_key1;
            std::string child_key2 = parent_key2;

            //Swap letters
            if (i % 2)
                std::swap(child_key1[rand() % 25], child_key1[rand() % 25]);
            else
                std::swap(child_key2[rand() % 25], child_key2[rand() % 25]);

            double eval = evaluate(decrypt(alpha, child_key1, child_key2, text));

            //If key is better or if temperature is high enough switch
            if (eval > parent_eval ||
                (T>0 && exp((eval - parent_eval) / T) > (double)rand() / RAND_MAX))
            {
                parent_eval = eval;
                parent_key1 = child_key1;
                parent_key2 = child_key2;
            }
        }
        std::stringstream ss;
        ss << "\nCurrent score: " << parent_eval << "\nCurrent Temp.: " << T << "\nBest key:  " << parent_key1 << ", " << parent_key2 << std::endl;
        emit appendToConsole(QString::fromStdString(ss.str()));

        emit setPlainText(QString::fromStdString(decrypt(alpha, parent_key1, parent_key2, text)));
    }

    emit finished();
}

void FourSquareWorker::useKey(QString base, QString key1, QString key2, QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(base.toStdString(), key1.toStdString(), key2.toStdString(), text.toStdString())));
    emit finished();
}

FourSquare::FourSquare(QWidget *parent) : QWidget(parent), ui(new Ui::FourSquare)
{
    cipher_data = {"Foursquare", 26, 48, 58, 76, 36, 72, 11, 28, 507, 114, false};
    ui->setupUi(this);

    //Setup worker thread
    worker = new FourSquareWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(QString,QString,QString,QString)), worker, SLOT(useKey(QString,QString,QString,QString)));
    connect(this, SIGNAL(crack(QString,QString)), worker, SLOT(crack(QString,QString)));

    worker_thread.start();
}

FourSquare::~FourSquare()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* FourSquare::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void FourSquare::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
        useKey(getBaseKey(ui->key_1->text()), ui->key_1->text(), ui->key_2->text(), text);
    else
        crack(text, getBaseKey(text));
}

void FourSquare::cancel()
{
    worker->cancel();
}

//Get missing letter and generate base key from it
QString FourSquare::getBaseKey(QString text)
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

void Cipher::FourSquare::on_key_1_editingFinished()
{
    //Sanitize key
    std::string text = "";
    for(char c : ui->key_1->text().toStdString())
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

    ui->key_1->setText(QString::fromStdString(text));
}

//TODO: Check other key for missing letter so keys match
void Cipher::FourSquare::on_key_2_editingFinished()
{
    //Sanitize key
    std::string text = "";
    for(char c : ui->key_2->text().toStdString())
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

    ui->key_2->setText(QString::fromStdString(text));
}
