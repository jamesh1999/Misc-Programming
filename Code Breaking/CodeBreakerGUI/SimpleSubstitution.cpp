#include "SimpleSubstitution.h"
#include "ui_SimpleSubstitution.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <float.h>
#include "Globals.h"

using namespace Cipher;

std::string SubstitutionWorker::decrypt(const std::string& key, std::string text)
{
    for (auto& letter : text)
        letter = key[letter - 'A'];

    return text;
}

void SubstitutionWorker::crack(QString qtext)
{
    std::string text = qtext.toStdString();
    //Overall max key
    std::string max_key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    double max_score = -DBL_MAX;

    //Key for iteration
    std::string parent_key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    double parent_score = -DBL_MAX;

    for (unsigned iter = 0; iter < 50; ++iter)
    {
        //Generate and evaluate random parent
        std::random_shuffle(parent_key.begin(),parent_key.end());
        parent_score = evaluate(decrypt(parent_key,text));

        for (int cnt = 0; cnt < 675; ++cnt)
        {
            //Clone the parent key
            std::string child_key = parent_key;

            //Swap next two characters
            std::swap(child_key[cnt/26],child_key[cnt%26]);

            //Evaluate new key
            double child_score = evaluate(decrypt(child_key,text));

            //Compare scores changing parent if child is better
            if(child_score>parent_score)
            {
                parent_key = child_key;
                parent_score = child_score;
                cnt = -1;
            }
        }

        //Print results if better
        if(parent_score>max_score)
        {
            max_score = parent_score;
            max_key = parent_key;

            std::stringstream ss;
            ss << "\nBest score: " << max_score << "\nBest key:  " << max_key << std::endl;
            emit appendToConsole(QString::fromStdString(ss.str()));

            emit setPlainText(QString::fromStdString(decrypt(max_key, text)));
        }
    }

    emit finished();
}

void SubstitutionWorker::useKey(QString key, QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(key.toStdString(), text.toStdString())));
    emit finished();
}

SimpleSubstitution::SimpleSubstitution(QWidget *parent) : QWidget(parent), ui(new Ui::SimpleSubstitution)
{
    cipher_data = {"Simple Substitution", 26, 63, 73, 95, 72, 73, 22, 50, 450, 120, false};
    ui->setupUi(this);

    worker = new SubstitutionWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(QString,QString)), worker, SLOT(useKey(QString,QString)));
    connect(this, SIGNAL(crack(QString)), worker, SLOT(crack(QString)));

    worker_thread.start();
}

SimpleSubstitution::~SimpleSubstitution()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* SimpleSubstitution::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void SimpleSubstitution::start(QString text)
{
    if(ui->use_key->isChecked())
        useKey(ui->key->text(), text);
    else
        crack(text);
}

