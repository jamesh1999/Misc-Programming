#include "CodeBreakerMain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CodeBreakerMain w;
    w.show();

    return a.exec();
}
