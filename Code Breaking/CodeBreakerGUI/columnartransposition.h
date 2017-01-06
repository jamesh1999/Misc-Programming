#ifndef COLUMNARTRANSPOSITION_H
#define COLUMNARTRANSPOSITION_H

#include <QString>
#include <QWidget>
#include <QThread>
#include <vector>
#include "Cipher.h"

namespace Ui {
class ColumnarTransposition;
}

namespace Cipher
{
    class ColumnarTranspositionWorker : public QObject, public ICipherWorker
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
        std::string decrypt(const std::vector<unsigned>&, const std::string&, bool);

    public slots:
        void crack(QString, bool);
        void useKey(QString, bool);
    };

    class ColumnarTransposition : public QWidget, public ICipher
    {
        Q_OBJECT
        Q_INTERFACES(Cipher::ICipher)

    public:
        explicit ColumnarTransposition(QWidget *parent = 0);
        ~ColumnarTransposition();

        void start(QString);
        void cancel();
        ICipherWorker* getWorker();

    public slots:
        void getStoredKey(std::vector<QString>);

    private:
        Ui::ColumnarTransposition *ui;
        ColumnarTranspositionWorker* worker;
        std::vector<unsigned> stored_key;

    signals:
        void crack(QString, bool);
        void useKey(QString, bool);
    private slots:
        void on_edit_key_clicked();
    };
}

#endif // COLUMNARTRANSPOSITION_H
