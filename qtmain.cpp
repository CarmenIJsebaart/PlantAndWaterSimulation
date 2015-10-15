#include "qtwidget.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QtWidget w;
  {
    const QRect screen = QApplication::desktop()->screenGeometry();
    w.setGeometry(0,0,screen.height() * 8 / 10,screen.height() * 8 / 10);
    w.move( screen.center() - w.rect().center() );
  }
  //w.setWindowState(Qt::WindowMaximized);
  w.show();
  return a.exec();
}
