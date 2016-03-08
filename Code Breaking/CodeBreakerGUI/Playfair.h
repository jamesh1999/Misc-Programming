#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include <QString>
#include <QWidget>
#include <QThread>
#include "Cipher.h"

namespace Ui {
class Playfair;
}

namespace Cipher
{
    class PlayfairWorker : public QObject, public ICipherWorker
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
        void crack(QString, QString);
        void useKey(QString,QString);
    };

    class Playfair : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit Playfair(QWidget *parent = 0);
        ~Playfair();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::Playfair *ui;
        PlayfairWorker* worker;
        QString getBaseKey(QString);

    signals:
        void crack(QString, QString);
        void useKey(QString,QString);
    private slots:
        void on_key_editingFinished();
    };
}

#endif // PLAYFAIR_H
