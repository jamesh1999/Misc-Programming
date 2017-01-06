#ifndef HILL_H
#define HILL_H

#include <string>
#include <QString>
#include <QWidget>
#include "Cipher.h"

namespace Ui {
class hill;
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

    protected:
        std::string decrypt(const std::string&, unsigned, std::vector<unsigned>);

    public slots:
        void crack(QString);
    };

    class HillCipher : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit HillCipher(QWidget *parent = 0);
        ~HillCipher();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::hill *ui;
        HillWorker* worker;

    signals:
        void crack(QString);
    };
}

#endif // HILL_H
