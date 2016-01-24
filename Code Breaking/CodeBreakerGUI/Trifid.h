#ifndef TRIFID_H
#define TRIFID_H

#include <QString>
#include <QWidget>
#include <QThread>
#include "Cipher.h"

namespace Ui {
class Trifid;
}

namespace Cipher
{
    class TrifidWorker : public QObject, public ICipherWorker
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipherWorker)

    signals:
        void finished();
        void progress(int);
        void setPlainText(QString);
        void appendToConsole(QString);

    protected:
        std::string decrypt(int, const std::string&, const std::string&);

    public slots:
        void crack(int, QString);
        void useKey(int, QString,QString);
    };

    class Trifid : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit Trifid(QWidget *parent = 0);
        ~Trifid();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::Trifid *ui;
        TrifidWorker* worker;

    signals:
        void crack(int,QString);
        void useKey(int,QString,QString);

    private slots:
        void on_key_editingFinished();
    };
}

#endif // TRIFID_H
