#ifndef HILL_H
#define HILL_H

#include <QString>
#include <QWidget>
#include <QThread>
#include <vector>
#include "Cipher.h"

namespace Ui {
class Hill;
}

namespace Cipher
{
    class HillWorker : public QObject, public ICipherWorker
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipherWorker)

    signals:
        void finished();
        void progress(int);
        void setPlainText(QString);
        void appendToConsole(QString);
        void storeKey(QString, double);

    protected:
        std::string decrypt(unsigned,const std::vector<unsigned>&, const std::string&);

    public slots:
        void crack(bool,int,QString,QString);
        void useKey(QString);
    public:
        std::vector<unsigned> stored_key;
    };

    class Hill : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit Hill(QWidget *parent = 0);
        ~Hill();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::Hill *ui;
        HillWorker* worker;
        std::vector<unsigned> stored_key;

    signals:
        void crack(bool,int,QString,QString);
        void useKey(QString);
    };
}

#endif // HILL_H
