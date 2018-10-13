#include "psmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PSMainWindow w;
    w.show();

    return a.exec();
}
