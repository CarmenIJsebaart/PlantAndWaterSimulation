#ifndef QTWIDGET_H
#define QTWIDGET_H

#include <QWidget>
#include <QPixmap>

#include "simulation.h"

namespace Ui {
  class QtWidget;
}

struct QImage;

class QtWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QtWidget(
    QWidget *parent = nullptr
  );
  QtWidget(const QtWidget&) = delete;
  QtWidget& operator=(const QtWidget&) = delete;
  ~QtWidget();

protected:
  void paintEvent(QPaintEvent *);
private:
  Ui::QtWidget *ui;
  QPixmap m_pixmap;
  Simulation m_simulation;

private slots:
  void OnTimer();
};

#endif // QTWIDGET_H
