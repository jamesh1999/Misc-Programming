#ifndef CADENUS_H
#define CADENUS_H

#include <QString>
#include <QWidget>
#include <QThread>
#include <vector>
#include "Cipher.h"

namespace Ui {
class Cadenus;
}

namespace Cipher
{
    class CadenusWorker : public QObject, public ICipherWorker
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipherWorker)

    signals:
        void finished();
        void progress(int);
        void setPlainText(QString);
        void appendToConsole(QString);

    protected:
        std::string decrypt(const std::vector<unsigned>&, const std::vector<unsigned>&, const std::string&);

    public slots:
        void crack(QString);
        void useKey(QString,QString,QString);
    };

    class Cadenus : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit Cadenus(QWidget *parent = 0);
        ~Cadenus();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::Cadenus *ui;
        CadenusWorker* worker;

    signals:
        void crack(QString);
        void useKey(QString,QString,QString);
    };
}

#endif // CADENUS_H
