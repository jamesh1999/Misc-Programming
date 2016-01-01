#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QTcpSocket>
#define BUFFERSIZE 256

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void updatePos();
private slots:
    void on_pushButton_pressed();
    void on_pushButton_released();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    int sock;
    bool transmitting;
    QTimer* timer;
    char buffer[BUFFERSIZE];
};

#endif // MAINWINDOW_H
