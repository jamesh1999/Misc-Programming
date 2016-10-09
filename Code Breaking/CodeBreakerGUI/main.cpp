#include "CodeBreakerMain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font = a.font();
    font.setPixelSize(10);
    a.setFont(font);

    CodeBreakerMain w;
    w.show();

    return a.exec();
}
