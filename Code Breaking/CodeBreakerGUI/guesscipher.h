#ifndef __GUESSCIPHER_INCLUDED__
#define __GUESSCIPHER_INCLUDED__

#include <QDialog>
#include <string>
#include "Cipher.h"

namespace Ui {
class GuessCipher;
}

class GuessCipher : public QDialog
{
    Q_OBJECT

public:
    std::string text;
    Cipher::CipherData** cipher_data;
    int num_ciphers;

    explicit GuessCipher(QWidget *parent = 0);
    ~GuessCipher();
    void guessCipher();

signals:
    void guessCipherReturn(int);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_listWidget_doubleClicked(const QModelIndex&);

private:
    Ui::GuessCipher *ui;
    std::pair<int, int> *scores;
    int guessedID;
};

#endif
