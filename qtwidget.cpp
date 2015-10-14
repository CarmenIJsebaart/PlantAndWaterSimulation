#include "qtwidget.h"

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

#include "ui_qtwidget.h"

QtWidget::QtWidget(
  QWidget *parent
)
  : QWidget(parent),
    ui(new Ui::QtWidget),
    m_pixmap{},
    m_simulation{}
{
  ui->setupUi(this);

  m_pixmap = QPixmap(m_simuwidth,height)},

  //Start a timer
  {
    QTimer * const timer{new QTimer(this)};
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    timer->setInterval(2);
    timer->start();
  }
}

QtWidget::~QtWidget()
{
  delete ui;
}

void QtWidget::OnTimer()
{
  static int z = 0;
  ++z;
  const int height{m_pixmap.height()};
  const int width{m_pixmap.width()};

  QImage image(width,height,QImage::Format_RGB32);


  //const grid& get_water_grid() const noexcept { return m_water_grid; }
  //const grid& get_plant_grid() const noexcept { return m_water_grid; }
  //void goto_next_timestep();
  //double get_current_time() const noexcept { return m_time; }


  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      image.setPixel(x,y,qRgb(z+10*x,z+20 * y,z+14*(x+y)));
    }
  }
  m_pixmap = QPixmap::fromImage(image);
  update();
}

void QtWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(
    this->rect(),
    m_pixmap//QPixmap::fromImage(*m_image)
  );
}
