#include "GuessCipher.h"
#include "ui_GuessCipher.h"
#include <sstream>
#include <QString>
#include <iostream>
#include "Globals.h"

int LDI_ARR[26][26] = {{4,7,8,7,4,6,7,5,7,3,6,8,7,9,3,7,3,9,8,9,6,7,6,5,7,4},
                         {7,4,2,0,8,1,1,1,6,3,0,7,2,1,7,1,0,6,5,3,7,1,2,0,6,0},
                         {8,2,5,2,7,3,2,8,7,2,7,6,2,1,8,2,2,6,4,7,6,1,3,0,4,0},
                         {7,6,5,6,8,6,5,5,8,4,3,6,6,5,7,5,3,6,7,7,6,5,6,0,6,2},
                         {9,7,8,8,8,7,6,6,7,4,5,8,7,9,7,7,5,9,9,8,5,7,7,6,7,3},
                         {7,4,5,3,7,6,4,4,7,2,2,6,5,3,8,4,0,7,5,7,6,2,4,0,5,0},
                         {7,5,5,4,7,5,5,7,7,3,2,6,5,5,7,5,2,7,6,6,6,3,5,0,5,1},
                         {8,5,4,4,9,4,3,4,8,3,1,5,5,4,8,4,2,6,5,7,6,2,5,0,5,0},
                         {7,5,8,7,7,7,7,4,4,2,5,8,7,9,7,6,4,7,8,8,4,7,3,5,0,5},
                         {5,0,0,0,4,0,0,0,3,0,0,0,0,0,5,0,0,0,0,0,6,0,0,0,0,0},
                         {5,4,3,2,7,4,2,4,6,2,2,4,3,6,5,3,1,3,6,5,3,0,4,0,5,0},
                         {8,5,5,7,8,5,4,4,8,2,5,8,5,4,8,5,2,4,6,6,6,5,5,0,7,1},
                         {8,6,4,3,8,4,2,4,7,1,0,4,6,4,7,6,1,3,6,5,6,1,4,0,6,0},
                         {8,6,7,8,8,6,9,6,8,4,6,6,5,6,8,5,3,5,8,9,6,5,6,3,6,2},
                         {6,6,7,7,6,8,6,6,6,3,6,7,8,9,7,7,3,9,7,8,9,6,8,4,5,3},
                         {7,3,3,3,7,3,2,6,7,2,1,7,3,2,7,6,0,7,6,6,6,0,3,0,4,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0},
                         {8,6,6,7,9,6,6,5,8,3,6,6,6,6,8,6,3,6,8,8,6,5,6,0,7,1},
                         {8,6,7,6,8,6,5,7,8,4,6,6,6,6,8,7,4,5,8,9,7,4,7,0,6,2},
                         {8,6,6,5,8,6,5,9,8,3,3,6,6,5,9,6,2,7,8,8,7,4,7,0,7,2},
                         {6,6,7,6,6,4,6,4,6,2,3,7,7,8,5,6,0,8,8,8,3,3,4,3,4,3},
                         {6,1,0,0,8,0,0,0,7,0,0,0,0,0,5,0,0,0,1,0,2,1,0,0,3,0},
                         {7,3,3,4,7,3,2,8,7,2,2,4,4,6,7,3,0,5,5,5,2,1,4,0,3,1},
                         {4,1,4,2,4,2,0,3,5,1,0,1,1,0,3,5,0,1,2,5,2,0,2,2,3,0},
                         {6,6,6,6,6,6,5,5,6,3,3,5,6,5,8,6,3,5,7,6,4,3,6,2,4,2},
                         {4,0,0,0,5,0,0,0,3,0,0,2,0,0,3,0,0,0,1,0,2,0,0,0,4,4}};

int SDD_ARR[26][26] = {{0,3,4,2,0,0,1,0,0,0,4,5,2,6,0,2,0,4,4,3,0,6,0,0,3,5},
                        {0,0,0,0,6,0,0,0,0,9,0,7,0,0,0,0,0,0,0,0,7,0,0,0,7,0},
                        {3,0,0,0,2,0,0,6,0,0,8,0,0,0,6,0,5,0,0,0,3,0,0,0,0,0},
                        {1,6,0,0,1,0,0,0,4,4,0,0,0,0,0,0,0,0,0,1,0,0,4,0,1,0},
                        {0,0,4,5,0,0,0,0,0,3,0,0,3,2,0,3,6,5,4,0,0,4,3,8,0,0},
                        {3,0,0,0,0,5,0,0,2,1,0,0,0,0,5,0,0,2,0,4,1,0,0,0,0,0},
                        {2,0,0,0,1,0,0,6,1,0,0,0,0,0,2,0,0,1,0,0,2,0,0,0,0,0},
                        {5,0,0,0,7,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,5,0,0,0,4,0,0,0,1,1,3,7,0,0,0,0,5,3,0,5,0,0,0,8},
                        {0,0,0,0,6,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,9,0,0,0,0,0},
                        {0,0,0,0,6,0,0,0,5,0,0,0,0,4,0,0,0,0,0,0,0,0,1,0,0,0},
                        {2,0,0,4,2,0,0,0,3,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,7,0},
                        {5,5,0,0,5,0,0,0,2,0,0,0,0,0,2,6,0,0,0,0,2,0,0,0,6,0},
                        {0,0,4,7,0,0,8,0,0,2,2,0,0,0,0,0,3,0,0,4,0,0,0,0,0,0},
                        {0,2,0,0,0,8,0,0,0,0,4,0,5,5,0,2,0,4,0,0,7,4,5,0,0,0},
                        {3,0,0,0,3,0,0,0,0,0,0,5,0,0,5,7,0,6,0,0,3,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0},
                        {1,0,0,0,4,0,0,0,2,0,4,0,0,0,2,0,0,0,0,0,0,0,0,0,5,0},
                        {1,1,0,0,0,0,0,1,2,0,0,0,0,0,1,4,4,0,1,4,2,0,4,0,0,0},
                        {0,0,0,0,0,0,0,8,3,0,0,0,0,0,3,0,0,0,0,0,0,0,2,0,0,0},
                        {0,4,3,0,0,0,5,0,0,0,0,6,2,3,0,6,0,6,5,3,0,0,0,0,0,6},
                        {0,0,0,0,8,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {6,0,0,0,2,0,0,6,6,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
                        {3,0,7,0,1,0,0,0,2,0,0,0,0,0,0,9,0,0,0,5,0,0,0,6,0,0},
                        {1,6,2,0,0,2,0,0,0,6,0,0,2,0,6,2,1,0,2,1,0,0,6,0,0,0},
                        {2,0,0,0,8,0,0,0,0,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,9}};

GuessCipher::GuessCipher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GuessCipher)
{
    ui->setupUi(this);
}

GuessCipher::~GuessCipher()
{
    //Clean up
    delete ui;
    delete[] scores;
    delete[] cipher_data;
}

void GuessCipher::guessCipher()
{
    if(text.length() == 0)
        return;

    //Identify most likely ciphers
    std::vector<char> letters; //Unique characters
    unsigned etacnt = 0; //Total frequency of ETA
    double ic = 0; //Index of coincidence
    double mic = 0; //Maximum periodic index of coincidence
    double mka = 0; //Maximum kappa
    double dic = 0; //Digraphic index of coincidence
    double edi = 0; //Even digraphic index of coincidence
    double lr = 0; //Long repeat
    int rod = 0; //%age of odd spaced repeats
    int ldi = 0; //ave English log digraph score
    int sdd = 0; //ave English single letter score

    //Unique letter count/numer of e/t/a
    for (auto c : text)
    {
        if (std::find(letters.begin(), letters.end(), c) == letters.end())
            letters.push_back(c);

        if (c == 'E' || c == 'T' || c == 'A')
            ++etacnt;
    }

    ic = indexCoincidence(text);

    //Maximum periodic index of coincidence
    mic = ic;
    for(int i = 2; i <= 15; ++i)
    {
        double current = 0;
        for(int j = 0; j < i; ++j)
        {
            std::string nstring = "";
            for(unsigned k = 0; k < (text.size() - j) / i; ++k)
                nstring.push_back(text[k * i + j]);

            current += indexCoincidence(nstring) / i;
        }

        if(current > mic)
            mic = current;
    }

    //Maximum number of n(1-15) spaced repeats
    for(int i = 1; i <= 15; ++i)
    {
        double current = 0;

        for(unsigned j = 0; j < text.size() - i; ++j)
            if(text[j] == text[j + i])
                current += 1;

        current /= text.size() - i;

        if(current > mka)
            mka = current;
    }

    //Index of coincidence of all/evenly spaced digraphs
    for(int i = 0; i < 729; ++i)
    {
        char c0 = 'A' + (i / 27);
        char c1 = 'A' + (i % 27);

        int cnt = 0;
        int cnt_even = 0;
        for(unsigned j = 0; j < text.size() - 1; ++j)
        {
            if(text[j] == c0 && text[j + 1] == c1)
            {
                ++cnt;
                if(j % 2 == 0)
                    ++cnt_even;
            }
        }
        dic += cnt * (cnt - 1);
        edi += cnt * (cnt - 1);
    }
    dic /= (text.size() - 1) * (text.size() - 2);
    edi /= (text.size() / 2) * ((text.size() / 2) - 1);

    //%age of sqrt of trigraph repeats
    for(unsigned i = 0; i < text.length() - 2; ++i)
        for(unsigned j = i + 1; j < text.length() - 2; ++j)
            for(unsigned k = 0; k < 3; ++k)
            {
                if(text[i + k] != text[j + k])
                    break;

                if(k == 2)
                    ++lr;
            }
    lr = std::sqrt(lr) / text.length();

    //%age of oddly spaced digraph repeats
    unsigned sum_all = 0;
    unsigned sum_odd = 0;
    for(unsigned i = 0; i < text.length() - 1; ++i)
        for(unsigned j = i + 1; j < text.length() - 1; ++j)
        {
            if(text[i] == text[j] && text[i + 1] == text[j + 1])
            {
                ++sum_all;
                if((j - i) % 2)
                    ++sum_odd;
            }
        }
    if(sum_all == 0)
        rod = 50.0;
    else
        rod = 100.0 * sum_odd / sum_all;

    for(unsigned i = 0; i < text.length() - 1; ++i)
    {
        if(text[i] >= 'A' && text[i] <= 'Z' &&
           text[i + 1] >= 'A' && text[i + 1] <= 'Z')
        ldi += LDI_ARR[text[i] - 'A'][text[i + 1] - 'A'];
        sdd += SDD_ARR[text[i] - 'A'][text[i + 1] - 'A'];
    }
    ldi *= 100.0 / (text.length() - 1);
    sdd *= 100.0 / (text.length() - 1);

    //Score ciphers
    scores = new std::pair<int, int>[num_ciphers];
    for(int i = 0; i < num_ciphers; ++i)
        scores[i].second = i;

    for(unsigned i = 0; i < num_ciphers; ++i)
    {
        if(letters.size() > cipher_data[i]->nchars)
        {
            scores[i].first = INT_MAX;
            continue;
        }

        //Calculate differences
        int diff = (cipher_data[i]->nchars - letters.size()) * 10;
        if(etacnt >= text.length() / 4 && !(cipher_data[i]->transposition))
            diff += 20;
        diff += std::abs(ic * 1000 - cipher_data[i]->ic);
        diff += std::abs(mic * 1000 - cipher_data[i]->mic);
        diff += std::abs(mka * 1000 - cipher_data[i]->mka);
        diff += std::abs(dic * 10000 - cipher_data[i]->dic);
        diff += std::abs(edi * 10000 - cipher_data[i]->edi);
        diff += std::abs(lr * 1000 - cipher_data[i]->lr);
        diff += std::abs(rod - cipher_data[i]->rod);
        diff += std::abs(ldi - cipher_data[i]->ldi);
        diff += std::abs(sdd - cipher_data[i]->sdd);

        scores[i].first = diff;
    }

    //Sort the ciphers by score
    std::sort(scores, scores + num_ciphers);

    //Add results to ui
    for(int i = 0; i < num_ciphers; ++i)
    {
        std::stringstream info;
        info << cipher_data[scores[i].second]->name << " - " << scores[i].first;
        ui->listWidget->addItem(QString::fromStdString(info.str()));
    }
    guessedID = scores[0].second;
    ui->selection->setText(QString(cipher_data[guessedID]->name));
}

void GuessCipher::on_pushButton_clicked()
{
    close();
}

void GuessCipher::on_pushButton_2_clicked()
{
    if(guessedID != -1)
        guessCipherReturn(guessedID);
    close();
}

void GuessCipher::on_listWidget_doubleClicked(const QModelIndex &index)
{
    guessedID = scores[index.row()].second;
    ui->selection->setText(QString(cipher_data[guessedID]->name));
}
