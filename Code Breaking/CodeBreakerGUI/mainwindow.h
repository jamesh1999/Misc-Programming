#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
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
    void setPlainTextOutput(QString);
    void appendToConsole(QString);
    void decryptWithCipher(int);

private slots:
    void on_pushButton_clicked();

    void on_actionGlobal_Settings_triggered();

    void on_actionClose_triggered();

    void on_pushButton_2_clicked();

private:
    Ui::CodeBreakerMain *ui;
};

#endif // MAINWINDOW_H
