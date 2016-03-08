#ifndef POLYBIUSSQUARE_H
#define POLYBIUSSQUARE_H

#include <QString>
#include <QWidget>
#include <QThread>
#include "Cipher.h"

namespace Ui {
class PolybiusSquare;
}

namespace Cipher
{
    class PolybiusWorker : public QObject, public ICipherWorker
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipherWorker)

    signals:
        void finished();
        void progress(int);
        void setPlainText(QString);
        void appendToConsole(QString);

    protected:
        std::string decrypt(const std::string&, const std::string&);

    public slots:
        void crack(QString);
        void useKey(QString, QString);
    };

    class Polybius : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit Polybius(QWidget *parent = 0);
        ~Polybius();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::PolybiusSquare *ui;
        PolybiusWorker* worker;

    signals:
        void crack(QString);
        void useKey(QString, QString);
    private slots:
        void on_key_editingFinished();
    };
}

#endif // POLYBIUSSQUARE_H
