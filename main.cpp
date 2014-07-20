#include "mapwindow.h"
#include "outwindow.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setApplicationName("ProjMapper");
  a.setOrganizationName("ProjMapper");
  MapWindow w;

  w.show();

  return a.exec();
}
