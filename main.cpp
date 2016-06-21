#include "zmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    zMainWindow w;
    w.show();

    return a.exec();
}
