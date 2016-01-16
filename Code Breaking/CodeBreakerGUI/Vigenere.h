#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>
#include <QWidget>
#include <QThread>
#include "Cipher.h"

namespace Ui {
class Vigenere;
}

namespace Cipher
{
    class VigenereWorker : public QObject, public ICipherWorker
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipherWorker)

    signals:
        void finished();
        void progress(int);
        void setPlainText(QString);
        void appendToConsole(QString);

    protected:
        std::string decrypt(const std::string&, std::string);

    public slots:
        void crack(QString);
        void useKey(QString,QString);
    };

    class Vigenere : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit Vigenere(QWidget *parent = 0);
        ~Vigenere();

        void start(QString);
        ICipherWorker* getWorker();

    private:
        Ui::Vigenere *ui;
        VigenereWorker* worker;

    signals:
        void crack(QString);
        void useKey(QString,QString);
    };
}

#endif // VIGENERE_H
