#ifndef CIPHER_H
#define CIPHER_H

#include <QThread>
#include <QString>
#include <QtPlugin>

namespace Cipher
{
    //Guess cipher data
    typedef struct
    {
        const char* name;
        int nchars;
        int ic;
        int mic;
        int mka;
        int dic;
        int edi;
        int lr;
        int rod;
        int ldi;
        int sdd;
        bool transposition;
    } CipherData;

    //Signals and slots for cipher worker
    class ICipherWorker
    {
    signals:
        virtual void finished() = 0;
        virtual void progress(int) = 0;
        virtual void setPlainText(QString) = 0;
        virtual void appendToConsole(QString) = 0;
    };

    //Signals/slots for a cipher widget
    class ICipher
    {
    protected:
        bool is_stopped = false;
        QThread worker_thread;

    public:

        CipherData cipher_data;
        virtual void start(QString) = 0;
        virtual ICipherWorker* getWorker() = 0;
        void cancel(){is_stopped = true;}
    };
}

Q_DECLARE_INTERFACE(Cipher::ICipherWorker, "ICipherWorker")
Q_DECLARE_INTERFACE(Cipher::ICipher, "ICipher")

#endif // CIPHER_H
