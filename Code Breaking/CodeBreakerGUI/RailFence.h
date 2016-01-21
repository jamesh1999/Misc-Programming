#ifndef RAILFENCE_H
#define RAILFENCE_H

#include <QString>
#include <QWidget>
#include <QThread>
#include "Cipher.h"

namespace Ui {
class RailFence;
}

namespace Cipher
{
    class RailWorker : public QObject, public ICipherWorker
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipherWorker)

    signals:
        void finished();
        void progress(int);
        void setPlainText(QString);
        void appendToConsole(QString);

    protected:
        std::string decrypt(unsigned, unsigned, const std::string&);

    public slots:
        void crack(QString);
        void useKey(unsigned, unsigned, QString);
    };

    class RailFence : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit RailFence(QWidget *parent = 0);
        ~RailFence();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    private:
        Ui::RailFence *ui;
        RailWorker* worker;

    signals:
        void crack(QString);
        void useKey(unsigned, unsigned, QString);

    private slots:
        void on_starting_rail_editingFinished();
        void on_rail_count_editingFinished();
    };
}

#endif // RAILFENCE_H
