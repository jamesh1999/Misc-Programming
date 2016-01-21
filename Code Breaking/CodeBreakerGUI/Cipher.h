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
    protected:
        bool keep_working = true;

    signals:
        virtual void finished() = 0;
        virtual void progress(int) = 0;
        virtual void setPlainText(QString) = 0;
        virtual void appendToConsole(QString) = 0;

    public:
        void cancel(){keep_working = false;}
        void reset(){keep_working = true;}
    };

    //Signals/slots for a cipher widget
    class ICipher
    {
    protected:
        QThread worker_thread;

    public:

        CipherData cipher_data;
        virtual void start(QString) = 0;
        virtual ICipherWorker* getWorker() = 0;
        virtual void cancel() = 0;
    };
}

Q_DECLARE_INTERFACE(Cipher::ICipherWorker, "ICipherWorker")
Q_DECLARE_INTERFACE(Cipher::ICipher, "ICipher")

#endif // CIPHER_H
