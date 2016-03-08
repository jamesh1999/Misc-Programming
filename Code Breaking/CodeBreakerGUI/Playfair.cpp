#include "Playfair.h"
#include "ui_Playfair.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include "Globals.h"

using namespace Cipher;

std::string PlayfairWorker::decrypt(const std::string& key, std::string text)
{
    for (unsigned i = 0; i < text.size(); i += 2)
    {
        int a_ind = key.find_first_of(text[i]);
        int b_ind = key.find_first_of(text[i + 1]);

        //Same row
        if (a_ind / 5 == b_ind / 5)
        {
            text[i]     = key[(((a_ind % 5) + 4) % 5) + (5 * (a_ind / 5))];
            text[i + 1] = key[(((b_ind % 5) + 4) % 5) + (5 * (b_ind / 5))];
        }
        //Same column
        else if (a_ind % 5 == b_ind % 5)
        {
            text[i]     = key[(a_ind % 5) + (5 * (((a_ind / 5) + 4) % 5))];
            text[i + 1] = key[(b_ind % 5) + (5 * (((b_ind / 5) + 4) % 5))];
        }
        else
        {
            text[i]     = key[5 * (a_ind / 5) + (b_ind % 5)];
            text[i + 1] = key[5 * (b_ind / 5) + (a_ind % 5)];
        }
    }

    for (unsigned i = 0; i < text.size(); ++i)
        if (text[i] == 'X' && i != 0)
            text[i] = text[i - 1];

    return text;
}

void PlayfairWorker::crack(QString qtext, QString key)
{
    std::string text = qtext.toStdString();

    //Overall max key
    std::string parent_key = key.toStdString();

    double parent_eval = -DBL_MAX;

    std::random_shuffle(parent_key.begin(), parent_key.end());

    //Implement simulated annealing
    for (double T = TEMP; T >= 0 && keep_working; T -= STEP)
    {
        for (unsigned i = 0; i < COUNT; ++i)
        {
            std::string child_key = parent_key;

            int transform = rand() % 674;

            //Swap letters
            if (transform < 625)
                std::swap(child_key[transform / 25], child_key[transform % 25]);
            //Swap rows
            else if (transform < 650)
            {
                int row1 = (transform - 625) / 5;
                int row2 = transform % 5;
                for (int i = 0; i < 5; ++i)
                {
                    char temp = child_key[(5 * row1) + i];
                    child_key[(5 * row1) + i] = child_key[(5 * row2) + i];
                    child_key[(5 * row2) + i] = temp;
                }
            }
            //Swap columns
            else
            {
                int col1 = (transform - 650) / 5;
                int col2 = transform % 5;
                for (int i = 0; i < 5; ++i)
                {
                    char temp = child_key[(5 * i) + col1];
                    child_key[(5 * i) + col1] = child_key[(5 * i) + col2];
                    child_key[(5 * i) + col2] = temp;
                }
            }

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

void PlayfairWorker::useKey(QString key, QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(key.toStdString(), text.toStdString())));
    emit finished();
}

Playfair::Playfair(QWidget *parent) : QWidget(parent), ui(new Ui::Playfair)
{
    cipher_data = {"Playfair", 25, 50, 60, 79, 38, 72, 12, 32, 491, 118, false};
    ui->setupUi(this);

    //Setup worker thread
    worker = new PlayfairWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(QString,QString)), worker, SLOT(useKey(QString,QString)));
    connect(this, SIGNAL(crack(QString,QString)), worker, SLOT(crack(QString,QString)));

    worker_thread.start();
}

Playfair::~Playfair()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* Playfair::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void Playfair::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
        useKey(ui->key->text(), text);
    else
        crack(text, getBaseKey(text));
}

void Playfair::cancel()
{
    worker->cancel();
}

//Get missing letter and generate base key from it
QString Playfair::getBaseKey(QString text)
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

void Cipher::Playfair::on_key_editingFinished()
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
