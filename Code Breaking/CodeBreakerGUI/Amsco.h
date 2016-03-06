#ifndef AMSCO_H
#define AMSCO_H

#include <QString>
#include <QWidget>
#include <QThread>
#include <vector>
#include "Cipher.h"

namespace Ui {
class Amsco;
}

namespace Cipher
{
    class AmscoWorker : public QObject, public ICipherWorker
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipherWorker)
    public:
        std::vector<unsigned> stored_key;

    signals:
        void finished();
        void progress(int);
        void setPlainText(QString);
        void appendToConsole(QString);

    protected:
        std::string decrypt(bool, int, const std::vector<unsigned>&, const std::string&);

    public slots:
        void crack(bool, QString);
        void useKey(bool, bool, QString);
    };

    class Amsco : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit Amsco(QWidget *parent = 0);
        ~Amsco();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    public slots:
        void getStoredKey(std::vector<QString>);

    private:
        Ui::Amsco *ui;
        AmscoWorker* worker;
        std::vector<unsigned> stored_key;

    signals:
        void crack(bool, QString);
        void useKey(bool, bool, QString);
    private slots:
        void on_edit_key_clicked();
    };
}

#endif // AMSCO_H
