#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <winsock.h>
#include <iostream>
#include <string>
#include <fstream>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setup socket
    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData);
        struct sockaddr_in dest;

        dest.sin_family = AF_INET;
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock < 0)
        {
            std::cout << "Socket Creation FAILED!" << std::endl;
        }

    //Bind socket
    dest.sin_port = htons(1337);
        dest.sin_addr.s_addr = inet_addr("192.168.1.95");
        if (::connect(sock,(struct sockaddr *)&dest,sizeof(dest))!=0){
            std::cout << "Socket Connection FAILED!" << std::endl;
            if (sock)
            {
                closesocket(sock);
                WSACleanup();
            }
        }
        std::cout << "Connected!" << std::endl;

        transmitting = false;
        timer = new QTimer(this);
           connect(timer, SIGNAL(timeout()), this, SLOT(updatePos()));
           timer->start(100);


}

MainWindow::~MainWindow()
{
    delete ui;

    //Close socket
    char message[] = "exit";
    send(sock, message, 4, 0);

    closesocket(sock);
    WSACleanup();
}

void MainWindow::on_pushButton_pressed()
{
    transmitting = true;
}

void MainWindow::updatePos()
{
    if(transmitting)
    {
        QPoint mousePos = mapFromGlobal(QCursor::pos());
        QPoint buttonPos = ui->pushButton->pos();
        int pwr = 0;
        int dir = 1;
        dir = ((mousePos.x() - buttonPos.x()) * 3) / ui->pushButton->width();
        dir = std::min(2, std::max(0, dir));
        pwr = ((mousePos.y() - buttonPos.y()) * 5000) / ui->pushButton->height();
        pwr = 2500 - std::min(5000, std::max(0, pwr));
        char message[] = "000000";
        message[0] = dir + '0';
        message[1] = (pwr >= 0 ? '+' : '-');
        pwr = std::abs(pwr);
        message[2] = pwr / 1000 + '0';
        message[3] = (pwr % 1000) / 100 + '0';
        message[4] = (pwr % 100) / 10 + '0';
        message[5] = pwr % 10 + '0';

        send(sock, message, 6, 0);
    }
}

void MainWindow::on_pushButton_released()
{
    transmitting = false;
    char message[] = "1+0000";
    send(sock, message, 6, 0);
}
