#include "dartomat_mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  CDartomatMain w;
  w.show();
  return a.exec();
}