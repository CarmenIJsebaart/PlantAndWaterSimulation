#include "qtwidget.h"

#include <sstream>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

#include "ui_qtwidget.h"

std::tuple<double,double> get_min_max(const grid& g) {
  const int height = g.get_height();
  const int width = g.get_width();
  assert(width > 0);
  assert(height > 0);
  std::tuple<double,double> minmax =
    std::make_tuple(
      g.get(0,0),g.get(0,0)
    )
  ;
  for (int y=0; y!=height; ++y) {
    for (int x=0; x!=width; ++x) {
      const double z = g.get(x,y);
      if (z < std::get<0>(minmax)) std::get<0>(minmax) = z;
      if (z > std::get<1>(minmax)) std::get<1>(minmax) = z;
    }
  }
  return minmax;
}

QtWidget::QtWidget(
  QWidget *parent
)
  : QWidget(parent),
    ui(new Ui::QtWidget),
    m_pixmap{},
    m_simulation{}
{
  ui->setupUi(this);

  const int width = m_simulation.get_plant_grid().get_width();
  const int height = m_simulation.get_plant_grid().get_height();
  m_pixmap = QPixmap(width,height);

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
  m_simulation.goto_next_timestep();

  {
    std::stringstream s;
    s << "Simulation at time "
      << m_simulation.get_current_time()
      << " (frame" << z << ")"
    ;
    this->setWindowTitle(s.str().c_str());

  }

  const int height{m_simulation.get_plant_grid().get_height()};
  const int width{m_simulation.get_plant_grid().get_width()};

  QImage image(width,height,QImage::Format_RGB32);
  const grid& plant_grid = m_simulation.get_plant_grid();
  const grid& water_grid = m_simulation.get_water_grid();
  const std::tuple<double,double> min_max_plants = get_min_max(plant_grid);
  const double min_plants = std::get<0>(min_max_plants);
  const double max_plants
    = std::get<1>(min_max_plants) == std::get<0>(min_max_plants)
    ? std::get<0>(min_max_plants) + 1.0
    : std::get<0>(min_max_plants)
  ;
  assert(min_plants < max_plants);
  const double plants_range = max_plants - min_plants;
  assert(plants_range > 0.0);
  const std::tuple<double,double> min_max_water = get_min_max(water_grid);
  const double min_water = std::get<0>(min_max_water);
  const double max_water
    = std::get<1>(min_max_water) == std::get<0>(min_max_water)
    ? std::get<1>(min_max_water) + 1.0
    : std::get<1>(min_max_water)
  ;
  assert(min_water < max_water);
  const double water_range = max_water - min_water;
  assert(water_range > 0.0);

  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      const double p = plant_grid.get(x,y);
      const double f_g = ((p - min_plants) / plants_range);
      const int g = static_cast<int>(f_g * 255.0);
      const double w = water_grid.get(x,y);
      const double f_w = ((w - min_water) / water_range);
      const int b = static_cast<int>(f_w * 255.0);
      image.setPixel(x,y,qRgb(0,g,b));
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
