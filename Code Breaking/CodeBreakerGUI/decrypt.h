#ifndef __DECRYPT_INCLUDED__
#define __DECRYPT_INCLUDED__

#include <string>
#include "QObject"

class DecryptionWorker : public QObject
{
    Q_OBJECT
public:
    std::string text;
    int cipher;
signals:
    void Finished();
    void setPlainTextOutput(QString);
    void appendToConsole(QString);

public slots:
    void decryptCipher();
};

extern DecryptionWorker* pWorker;

#endif

