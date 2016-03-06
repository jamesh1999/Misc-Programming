#ifndef FOURSQUARE_H
#define FOURSQUARE_H

#include <QString>
#include <QWidget>
#include <QThread>
#include "Cipher.h"

namespace Ui {
class FourSquare;
}

namespace Cipher
{
    class FourSquareWorker : public QObject, public ICipherWorker
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipherWorker)

    signals:
        void finished();
        void progress(int);
        void setPlainText(QString);
        void appendToConsole(QString);

    protected:
        std::string decrypt(const std::string&, const std::string&, const std::string&, std::string);

    public slots:
        void crack(QString, QString);
        void useKey(QString, QString, QString,QString);
    };

    class FourSquare : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit FourSquare(QWidget *parent = 0);
        ~FourSquare();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::FourSquare *ui;
        FourSquareWorker* worker;
        QString getBaseKey(QString);

    signals:
        void crack(QString, QString);
        void useKey(QString, QString, QString, QString);
    private slots:
        void on_key_1_editingFinished();
        void on_key_2_editingFinished();
    };
}

#endif // FOURSQUARE_H
