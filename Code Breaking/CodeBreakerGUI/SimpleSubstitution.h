#ifndef SIMPLESUBSTITUTION_H
#define SIMPLESUBSTITUTION_H

#include <string>
#include <QString>
#include <QWidget>
#include "Cipher.h"

namespace Ui {
class SimpleSubstitution;
}

namespace Cipher
{
    class SubstitutionWorker : public QObject, public ICipherWorker
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
        void crack(QString);
        void useKey(QString,QString);
    };

    class SimpleSubstitution : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit SimpleSubstitution(QWidget *parent = 0);
        ~SimpleSubstitution();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::SimpleSubstitution *ui;
        SubstitutionWorker* worker;

    signals:
        void crack(QString);
        void useKey(QString,QString);

    private slots:
        void on_key_editingFinished();
    };
}

#endif // SIMPLESUBSTITUTION_H
