#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include "RailFence.h"
#include "Globals.h"
#include "ui_RailFence.h"

using namespace Cipher;

std::string RailWorker::decrypt(unsigned rails, unsigned start, const std::string& text)
{

    std::vector<std::vector<unsigned>> rail_list(2 * rails, std::vector<unsigned>(0));

    //Work out starting rail/direction
    unsigned current = start < rails ? start : 2 * rails - start - 1;
    bool plus = start < rails;
    for (unsigned i = 0; i < text.size(); ++i)
    {
        rail_list[current].push_back(i);

        //Change direction
        if (current + 1 == rails)
            plus = false;
        else if (current == 0)
            plus = true;

        current += plus ? 1 : -1;
    }

    //Concatenate vectors
    std::vector<unsigned> order;
    for (auto v : rail_list)
        order.insert(order.end(), v.begin(), v.end());

    //Insert char in corresponding position
    std::string decoded(text.size(), 'a');
    for (unsigned i = 0; i < text.size(); ++i)
        decoded[order[i]] = text[i];

    return decoded;
}

void RailWorker::crack(QString qtext)
{
    std::string text = qtext.toStdString();

    //Overall max score
    unsigned max_cnt;
    double max_score = -DBL_MAX;
    unsigned max_start = 0;

    //Brute force rails
    for (unsigned i = 2; i <= MAX_RAILS; ++i)
    {
        for (unsigned j = 0; j < 2 * i - 1 && keep_working; ++j)
        {
            if (j == i) continue;

            double score = evaluate(decrypt(i, j, text));

            //Print results if better
            if (score > max_score)
            {
                max_score = score;
                max_start = j;
                max_cnt = i;

                std::stringstream ss;
                ss << "\nBest rail number: " << max_cnt << "\nBest starting rail: " << max_start << "\nBest score: " << max_score << std::endl;
                emit appendToConsole(QString::fromStdString(ss.str()));

                emit setPlainText(QString::fromStdString(decrypt(max_cnt, max_start, text)));
            }
        }
    }

    emit finished();
}

void RailWorker::useKey(unsigned rails, unsigned start, QString text)
{
    emit setPlainText(QString::fromStdString(decrypt(rails, start, text.toStdString())));
    emit finished();
}

RailFence::RailFence(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RailFence)
{
    cipher_data = {"Rail Fence", 26, 63, 72, 94, 44, 43, 11, 50, 688, 188, true}; //
    ui->setupUi(this);
    ui->rail_count->setMaximum(MAX_RAILS);
    ui->starting_rail->setMaximum(MAX_RAILS);

    //Setup worker thread
    worker = new RailWorker();
    worker->moveToThread(&worker_thread);

    connect(&worker_thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(useKey(unsigned,unsigned,QString)), worker, SLOT(useKey(unsigned,unsigned,QString)));
    connect(this, SIGNAL(crack(QString)), worker, SLOT(crack(QString)));

    worker_thread.start();
}

RailFence::~RailFence()
{
    delete ui;

    worker_thread.quit();
    worker_thread.wait();
}

ICipherWorker* RailFence::getWorker()
{
    return qobject_cast<ICipherWorker*>(worker);
}

void RailFence::start(QString text)
{
    worker->reset();
    if(ui->use_key->isChecked())
        useKey(ui->rail_count->value(),
               ui->reversed->isChecked()
               ? ui->rail_count->value() + ui->starting_rail->value()
               : ui->starting_rail->value(), text);
    else
        crack(text);
}

void RailFence::cancel()
{
    worker->cancel();
}

//Check that starting rail is within bounds
void RailFence::on_starting_rail_editingFinished()
{
    if(ui->starting_rail->value() >= ui->rail_count->value())
        ui->starting_rail->setValue(ui->rail_count->value() - 1);
}

void RailFence::on_rail_count_editingFinished()
{
    if(ui->starting_rail->value() >= ui->rail_count->value())
        ui->starting_rail->setValue(ui->rail_count->value() - 1);
}
