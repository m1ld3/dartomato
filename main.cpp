#include "dartomatmain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DartomatMain w;
    w.show();
    return a.exec();
}
