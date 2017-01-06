#include "hill.h"
#include "ui_hill.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <float.h>
#include "Globals.h"

using namespace Cipher;

std::string HillWorker::decrypt(const std::string& input, unsigned n, std::vector<unsigned> mat)
{
    std::string text;
    text.resize(input.size());

    unsigned x = input.size() / n;
    for(unsigned i = 0; i < x; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            int acc = 0;
            for(int k = 0; k < n; ++k)
            {
                acc += (input[i * n + k] - 'A') * mat[j * n + k];
            }
            text[i * n + j] = 'A' + (acc % 26);
        }
    }

    return text;
}

void HillWorker::crack(QString qtext)
{
    std::string text = qtext.toStdString();

    //Overall max key
    double parent_score = -DBL_MAX;

    std::vector<unsigned> mat;

    for(int i0 = 0; i0 < 26; ++i0)
        for(int i1 = 0; i1 < 26; ++i1)
            for(int i2 = 0; i2 < 26; ++i2)
                for(int i3 = 0; i3 < 26; ++i3)
                {
                    mat.clear();
                    mat.push_back(15);
                    mat.push_back(17);
                    mat.push_back(20);
                    mat.push_back(9);

                    double child_score = evaluate(decrypt(text,2,mat));
                    //if(child_score > parent_score)
                       {
                        parent_score = child_score;

                    emit setPlainText(QString::fromStdString(decrypt(text,2,mat)));
                    }break;
                }

    /*for (unsigned iter = 0; iter < 50 && keep_working; ++iter)
    {
        //Generate and evaluate random parent
        std::random_shuffle(parent_key.begin(),parent_key.end());
        parent_score = evaluate(decrypt(parent_key,text));

        for (int cnt = 0; cnt < 675; ++cnt)
        {
            //Clone the parent key
            std::string child_key = parent_key;

            //Swap next two characters
            std::swap(child_key[cnt / 26],child_key[cnt % 26]);

            //Evaluate new key
            double child_score = evaluate(decrypt(child_key,text));

            //Compare scores changing parent if child is better
            if(child_score > parent_score && keep_working)
            {
                parent_key = child_key;
                parent_score = child_score;
                cnt = -1;
            }
        }

        //Print results if better
        if(parent_score > max_score)
        {
            max_score = parent_score;
            max_key = parent_key;

            std::stringstream ss;
            ss << "\nBest score: " << max_score << "\nBest key:  " << max_key << std::endl;
            emit appendToConsole(QString::fromStdString(ss.str()));

            emit setPlainText(QString::fromStdString(decrypt(max_key, text)));
        }
    }*/

    emit finished();
}

HillCipher::HillCipher(QWidget *parent) : QWidget(parent), ui(new Ui::hill)
{
    cipher_data = {"Hill", 26, 63, 73, 95, 72, 73, 22, 50, 450, 120, false};
    ui->setupUi(this);

    //Setup worker thread
    worker = new HillWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(crack(QString)), worker, SLOT(crack(QString)));

    worker_thread.start();
}

HillCipher::~HillCipher()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* HillCipher::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void HillCipher::start(QString text)
{
    worker->reset();
    crack(text);
}

void HillCipher::cancel()
{
    worker->cancel();
}
