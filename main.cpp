#include "cpuconfig.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CPUConfig w;
    w.show();
    return a.exec();
}
