#ifndef BINARYCONVERT_H
#define BINARYCONVERT_H

#include <QString>
#include <QWidget>
#include <QThread>
#include "Cipher.h"

namespace Ui {
class BinaryConvert;
}

namespace Cipher
{
    class BinaryWorker : public QObject, public ICipherWorker
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipherWorker)

    signals:
        void finished();
        void progress(int);
        void setPlainText(QString);
        void appendToConsole(QString);

    public slots:
        void decode(unsigned,unsigned,QString);
    };

    class BinaryConvert : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit BinaryConvert(QWidget *parent = 0);
        ~BinaryConvert();

        void start(QString);
            void cancel();
            ICipherWorker* getWorker();

        private:
            Ui::BinaryConvert *ui;
            BinaryWorker* worker;

        signals:
            void decode(unsigned,unsigned,QString);
    };
}

#endif // BINARYCONVERT_H
