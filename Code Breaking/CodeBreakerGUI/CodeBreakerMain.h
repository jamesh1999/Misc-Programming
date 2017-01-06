#ifndef __CODEBREAKERMAIN_INCLUDED__
#define __CODEBREAKERMAIN_INCLUDED__

#include <QMainWindow>
#include <QShortcut>

namespace Ui {
class CodeBreakerMain;
}

class CodeBreakerMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit CodeBreakerMain(QWidget *parent = 0);
    ~CodeBreakerMain();

public slots:
    //Access functions
    void finished();
    void progress(int);
    void setPlainTextOutput(QString);
    void appendToConsole(QString);
    void guessCipherReturn(int);

private slots:
    void on_decrypt_clicked();
    void on_guess_cipher_clicked();
    void on_actionGlobal_Settings_triggered();
    void on_actionClose_triggered();
    void on_cipher_currentIndexChanged(int);
    void on_stackedWidget_currentChanged(int);

    void on_actionAdd_Spaces_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

private:
    Ui::CodeBreakerMain *ui;
    int connected_cipher = -1;
    bool is_working = false;

    QShortcut* zoomIn;
    QShortcut* zoomOut;
    unsigned fontSize = 10;

    void decryptWithCipher(int);
};

#endif // CODEBREAKERMAIN_H
