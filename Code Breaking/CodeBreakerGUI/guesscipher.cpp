#include "guesscipher.h"
#include "ui_guesscipher.h"
#include "Globals.h"
#include <iostream>
#include <sstream>
#include "QString"

GuessCipher::GuessCipher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GuessCipher)
{
    ui->setupUi(this);
}

GuessCipher::~GuessCipher()
{
    delete ui;
}

void GuessCipher::on_pushButton_clicked()
{
    close();
}

void GuessCipher::guessCipher()
{
    //Identify most likely ciphers
    std::vector<char> identified;
    unsigned lcnt = 0; //Number of different characters
    unsigned etacnt = 0; //Total frequency of ETA
    double ic = 0;
    double mic = 0;
    double mka = 0;
    double dic = 0;
    double edi = 0;
    double lr = 0;
    int rod = 0;
    int ldi = 0;
    int sdd = 0;

    for (auto c : text)
    {
        if (std::find(identified.begin(), identified.end(), c) == identified.end())
        {
            ++lcnt;
            identified.push_back(c);
        }

        if (c == 'E' || c == 'T' || c == 'A')
            ++etacnt;
    }

    ic = indexCoincidence(text);

    mic = ic;
    for(int i = 2; i <= 15; ++i)
    {
        double current = 0;
        for(int j = 0; j < i; ++j)
        {
            std::string nstring = "";
            for(int k = 0; k < (text.size() - j) / i; ++k)
                nstring.push_back(text[k * i + j]);

            current += indexCoincidence(nstring) / i;
        }

        if(current > mic)
            mic = current;
    }

    for(int i = 1; i <= 15; ++i)
    {
        double current = 0;

        for(int j = 0; j < text.size() - i; ++j)
        {
            if(text[j] == text[j + i])
                current += 1;
        }

        current /= text.size() - i;

        if(current > mka)
            mka = current;
    }

    for(int i = 0; i < 729; ++i)
    {
        char c0 = 'A' + (i / 27);
        char c1 = 'A' + (i % 27);

        int cnt = 0;
        for(int j = 0; j < text.size() - 1; ++j)
        {
            if(text[j] == c0 && text[j + 1] == c1)
                ++cnt;
        }
        dic += cnt * (cnt - 1);
    }
    dic /= (text.size() - 1) * (text.size() - 2);

    for(int i = 0; i < 729; ++i)
    {
        char c0 = 'A' + (i / 27);
        char c1 = 'A' + (i % 27);

        int cnt = 0;
        for(int j = 0; j < text.size() - 1; j += 2)
        {
            if(text[j] == c0 && text[j + 1] == c1)
                ++cnt;
        }
        edi += cnt * (cnt - 1);
    }
    int n = text.size() / 2;
    edi /= n * (n - 1);

    for(unsigned i = 0; i < text.length() - 2; ++i)
        for(unsigned j = i + 1; j < text.length() - 2; ++j)
            for(unsigned k = 0; k < 3; ++k)
            {
                if(text[i + k] != text[j + k])
                    break;
                else if(k == 2)
                    ++lr;
            }
    lr = std::sqrt(lr) / text.length();

    unsigned sum_all = 0;
    unsigned sum_odd = 0;
    for(unsigned i = 0; i < text.length() - 1; ++i)
        for(unsigned j = i + 1; j < text.length() - 1; ++j)
        {
            if(text[i] == text[j] && text[i + 1] == text[j + 1])
            {
                ++sum_all;
                if((j - i) & 1)
                    ++sum_odd;
            }
        }
    if(sum_all == 0)
        rod = 50.0;
    else
        rod = 100.0 * sum_odd / sum_all;

    for(unsigned i = 0; i < text.length() - 1; ++i)
    {
        ldi += LDI_ARR[text[i] - 'A'][text[i + 1] - 'A'];
    }
    ldi *= 100.0 / (text.length() - 1);

    for(unsigned i = 0; i < text.length() - 1; ++i)
    {
        sdd += SDD_ARR[text[i] - 'A'][text[i+1] - 'A'];
    }
    sdd *= 100.0 / (text.length() - 1);

    //Score ciphers
    std::pair<int, int> scores[MAX_CIPHER] = {std::pair<int,int>(0,0)};
    for(int i = 0; i < MAX_CIPHER; ++i)
        scores[i].second = i;

    for(unsigned i = 0; i < MAX_CIPHER; ++i)
    {
        if(lcnt > CIPHERS[i].nchars)
        {
            scores[i].first = INT_MAX;
            continue;
        }

        int diff = (CIPHERS[i].nchars - lcnt) * 10;

        if(etacnt >= text.length() / 24 && !(CIPHERS[i].transposition))
            diff += 20;

        diff += std::abs(ic * 1000 - CIPHERS[i].ic);
        diff += std::abs(mic * 1000 - CIPHERS[i].mic);
        diff += std::abs(mka * 1000 - CIPHERS[i].mka);
        diff += std::abs(dic * 10000 - CIPHERS[i].dic);
        diff += std::abs(edi * 10000 - CIPHERS[i].edi);
        diff += std::abs(lr * 1000 - CIPHERS[i].lr);
        diff += std::abs(rod - CIPHERS[i].rod);
        diff += std::abs(ldi - CIPHERS[i].ldi);
        diff += std::abs(sdd - CIPHERS[i].sdd);

        scores[i].first = diff;
    }

    std::sort(std::begin(scores), std::end(scores));

    for(int i = 0; i < MAX_CIPHER; ++i)
    {
        std::stringstream info;
        info << CIPHERS[scores[i].second].name << " - " << scores[i].first;
        ui->listWidget->addItem(QString::fromStdString(info.str()));
    }
    guessedID = scores[0].second;
}

void GuessCipher::on_pushButton_2_clicked()
{
    if(guessedID != -1)
        decryptWithCipher(guessedID);
    close();
}
