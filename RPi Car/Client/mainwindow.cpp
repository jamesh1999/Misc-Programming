#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <winsock.h>
#include <iostream>
#include <string>
#include <QTimer>

#define I_TO_C(X) (X + '0')

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

        int dir;
        dir = ((mousePos.x() - buttonPos.x()) * 3) / ui->pushButton->width();
        dir = std::min(2, std::max(0, dir));

        int pwr;
        pwr = ((mousePos.y() - buttonPos.y()) * 5000) / ui->pushButton->height();
        pwr = 2500 - std::min(5000, std::max(0, pwr));

        char message[] = "000000";
        message[0] = I_TO_C(dir);
        message[1] = (pwr >= 0 ? '+' : '-');
        pwr = std::abs(pwr);
        message[2] = I_TO_C(pwr / 1000);
        message[3] = I_TO_C((pwr % 1000) / 100);
        message[4] = I_TO_C((pwr % 100) / 10);
        message[5] = I_TO_C(pwr % 10);

        send(sock, message, 6, 0);
    }
}

void MainWindow::on_pushButton_released()
{
    transmitting = false;
    char message[] = "1+0000";
    send(sock, message, 6, 0);
}

void MainWindow::on_pushButton_2_clicked()
{
    //Setup socket
    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData);
    struct sockaddr_in dest;

    dest.sin_family = AF_INET;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
    {
        std::cerr << "FAILED: Socket creation" << std::endl;
    }

    //Bind socket
    dest.sin_port = htons(1337);
    dest.sin_addr.s_addr = inet_addr(ui->lineEdit->text().toLatin1().data());
    if (::connect(sock,(struct sockaddr *)&dest, sizeof(dest)) != 0)
    {
        std::cerr << "FAILED: Socket connection" << std::endl;
        closesocket(sock);
        WSACleanup();
    }
    std::cout << "Connected!" << std::endl;

    char port[16];
    strncpy(port, ui->lineEdit_2->text().toLatin1().data(), sizeof(port));
    send(sock, port, sizeof(port), 0);

    std::cout << "Starting camera..." << std::endl;
    std::string cmd = "start cam ";
    cmd += port;
    system(cmd.c_str());

    transmitting = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePos()));
    timer->start(100);

    ui->pushButton_2->setEnabled(false);
}
