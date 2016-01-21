#ifndef BIFID_H
#define BIFID_H

#include <QString>
#include <QWidget>
#include <QThread>
#include "Cipher.h"

namespace Ui {
class Bifid;
}

namespace Cipher
{
    class BifidWorker : public QObject, public ICipherWorker
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
        void crack(int, QString, QString);
        void useKey(int, QString,QString);
    };

    class Bifid : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit Bifid(QWidget *parent = 0);
        ~Bifid();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::Bifid *ui;
        BifidWorker* worker;
        QString getBaseKey(QString);

    signals:
        void crack(int,QString, QString);
        void useKey(int,QString,QString);
    private slots:
        void on_key_editingFinished();
    };
}

#endif // BIFID_H
