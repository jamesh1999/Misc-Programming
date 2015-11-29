#ifndef GUESSCIPHER_H
#define GUESSCIPHER_H

#include <QDialog>
#include <string>

namespace Ui {
class GuessCipher;
}

class GuessCipher : public QDialog
{
    Q_OBJECT

public:
    std::string text;
    int guessedID;

    explicit GuessCipher(QWidget *parent = 0);
    ~GuessCipher();
    void guessCipher();

signals:
    void decryptWithCipher(int);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::GuessCipher *ui;
};

#endif // GUESSCIPHER_H
