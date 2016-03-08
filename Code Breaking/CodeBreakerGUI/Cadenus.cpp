#include "Cadenus.h"
#include "ui_Cadenus.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include "Globals.h"

using namespace Cipher;

std::string CadenusWorker::decrypt(const std::vector<unsigned>& order_key, const std::vector<unsigned>& shift_key, const std::string& text)
{
    //Reconstruct columns
    std::vector<std::string> columns(order_key.size(), "");
    for (unsigned i = 0; i < text.size(); ++i)
        columns[order_key[i % order_key.size()]] += text[i];

    //Combine columns factoring in shift
    std::string result;
    for (int i = 0; i < 25; ++i)
        for (unsigned j = 0; j < order_key.size(); ++j)
            result += columns[j][(shift_key[order_key[j]] + i) % 25];

    return result;
}

void CadenusWorker::crack(QString qtext)
{
    std::string text = qtext.toStdString();

    std::vector<unsigned> top_key1;
    std::vector<unsigned> top_key2;
    double top_eval = -DBL_MAX;

    std::vector<unsigned> parent_key1;
    for (unsigned i = 0; i < text.size() / 25; ++i)
        parent_key1.push_back(i);
    std::random_shuffle(parent_key1.begin(), parent_key1.end());

    std::vector<unsigned> parent_key2;
    for (unsigned i = 0; i < text.size() / 25; ++i)
        parent_key2.push_back(rand() % 25);

    double parent_eval = -DBL_MAX;

    //Implement simulated annealing
    for (double T = TEMP; T >= 0 && keep_working; T -= STEP)
    {
        for (unsigned i = 0; i < COUNT; ++i)
        {
            std::vector<unsigned> child_key1 = parent_key1;
            std::vector<unsigned> child_key2 = parent_key2;

            //Swap columns or change shift in one column
            if (rand() % child_key1.size())
                child_key2[rand() % child_key2.size()] = rand() % 25;
            else
                std::swap(child_key1[rand() % child_key1.size()], child_key1[rand() % child_key1.size()]);

            double eval = evaluate(decrypt(child_key1, child_key2, text));

            //Otherwise switch if temperature is high enough
            if (eval > parent_eval ||
                (T>0 && exp((eval - parent_eval) / T) > (double)rand() / RAND_MAX))
            {
                parent_eval = eval;
                parent_key1 = child_key1;
                parent_key2 = child_key2;

                if (eval > top_eval)
                {
                    top_eval = eval;
                    top_key1 = child_key1;
                    top_key2 = child_key2;
                }
            }
        }
        std::stringstream ss;
        ss << "\nCurrent score: " << top_eval << "\nCurrent Temp.: " << T << "\nBest Key: ";
        for (auto v : top_key1)
            ss << v << " ";
        ss << ", ";
        for (auto v : top_key2)
            ss << v << " ";
        ss << std::endl;
        emit appendToConsole(QString::fromStdString(ss.str()));

        emit setPlainText(QString::fromStdString(decrypt(top_key1, top_key2, text)));
    }

    emit finished();
}

void CadenusWorker::useKey(QString qkey, QString qalpha, QString text)
{
    std::string key = qkey.toStdString();
    std::string alpha = qalpha.toStdString();

    //Construct decrypt keys
    std::vector<unsigned> order_key;
    std::vector<unsigned> shift_key;
    for(char a = 'A'; a <= 'Z'; ++a)
        for(unsigned i = 0; i < key.size(); ++i)
            if(key[i] == a)
            {
                order_key.push_back(i);
                shift_key.push_back(25 - alpha.find_first_of(a));
            }

    emit setPlainText(QString::fromStdString(decrypt(order_key, shift_key, text.toStdString())));
    emit finished();
}

Cadenus::Cadenus(QWidget *parent) : QWidget(parent), ui(new Ui::Cadenus)
{
    cipher_data = {"Cadenus", 26, 63, 74, 95, 40, 41, 10, 49, 657, 134, true};
    ui->setupUi(this);

    //Setup worker thread
    worker = new CadenusWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(QString,QString,QString)), worker, SLOT(useKey(QString,QString,QString)));
    connect(this, SIGNAL(crack(QString)), worker, SLOT(crack(QString)));

    worker_thread.start();
}

Cadenus::~Cadenus()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* Cadenus::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void Cadenus::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
        useKey(ui->key_1->text(), ui->key_2->text(), text);
    else
        crack(text);
}

void Cadenus::cancel()
{
    worker->cancel();
}
